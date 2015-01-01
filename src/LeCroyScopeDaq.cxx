#include "LeCroyScopeDaq.h"
#include "LeCroyTraceDecoder.h"
#include "LeCroyDsoBinaryDecoder.h"
#include "LeCroyDsoTrace.h"
#include "IOmethods.h"
#include "BinaryIO.h"
#include "ScopeSettings.h"
#include "LeCroyScope.h"
#include "RootTreeOutput.h"

#include <iostream>
#include <cmath>

LeCroyScopeDaq::LeCroyScopeDaq(long numberOfEvents, 
			       std::string parametersFileName, 
			       std::string outputFileNamePrefix,
			       bool processData,
			       bool debug): ApplicationBase(),
					    m_numberOfEvents(numberOfEvents), 
					    m_parametersFileName(parametersFileName),
					    m_outputFileNamePrefix(outputFileNamePrefix),
					    m_processData(processData),
					    m_fileCounter(1),
					    m_keyboard() {
  m_debug = debug;
}

//-------------------------------------------------------------------

LeCroyScopeDaq::~LeCroyScopeDaq() {
}

//-------------------------------------------------------------------

int LeCroyScopeDaq::initialize(void) {
  std::string fileName;

  // Record the start time for use within output file names
  m_startTimeStr = IOmethods::currentTimeString();

  // Create a trace decoder.
  m_traceDecoder = new LeCroyTraceDecoder(this);

  // Read the parameters from a text file.
  m_scopeSettings = new ScopeSettings(m_parametersFileName);
  if(m_scopeSettings->initialize()) {
    std::cerr << "Error: ScopeSettings initialize() failed." << std::endl;
    return 1;
  }
  m_scopeSettings->printParameterValues();

  // Setup scope for run
  m_lecroyScope = new LeCroyScope(this);
  if(m_lecroyScope->initialize()) {
    std::cerr << "Error: LeCroyScope initialize() failed." << std::endl;
    return 2;
  }

  if(m_processData) {
    // Open Root output file
    m_rootTreeOutput = new RootTreeOutput();
    fileName = IOmethods::createFileName(m_outputFileNamePrefix,
					 m_startTimeStr, 
					 m_fileCounter,
					 ".root");
    if(m_rootTreeOutput->initialize(fileName)) {
      std::cerr << "Error: RootTreeOutput initialize() failed." << std::endl;
      return 3;
    }
  }
  else {
    // Open binary output file
    m_binaryIO = new BinaryIO(true); // Binary output 
    fileName = IOmethods::createFileName(m_outputFileNamePrefix,
					 m_startTimeStr, 
					 m_fileCounter,
					 ".bin");
    if(m_binaryIO->initialize(fileName)) {
      std::cerr << "Error: BinaryIO initialize() failed." << std::endl;
      return 3;
    }
  }
  
  return 0;
}

//--------------------------------------------------------------------

int LeCroyScopeDaq::run(void) {
  int retVal;

  unsigned long eventsRecorded;
  unsigned long eventsPerFile = m_scopeSettings->events_per_file();
  int fileCounter = 1;
  std::string fileName;

  long numBytes;
  int firstBytePos;

  std::vector<char *>::iterator itr;
  std::vector<char *>::iterator itr_end;
  LeCroyDsoTrace *trace = 0;

  // Reduce the number of events per file when a sequence of triggers
  // is requested.
  if(m_scopeSettings->sequence() > 0) {
    eventsPerFile /= m_scopeSettings->sequence();
  }
  if(eventsPerFile == 0) {
    eventsPerFile = 1;
  }

  std::cout << std::endl << ">> Running until " << m_numberOfEvents << " triggers/trigger sequences have been acquired." << std::endl;
  std::cout << ">> Recording " << eventsPerFile << " triggers per file." << std::endl;  
  std::cout << ">> Press 'Q' to cleanly cut short the data acquistion." << std::endl;

  eventsRecorded = 0;
  while(eventsRecorded < m_numberOfEvents) {
    // Key if there has been a keyboard action
    if(m_keyboard.kbhit()) {
      if(m_keyboard.getch() == 'Q') {
        std::cout << ">> Exiting after " << eventsRecorded << " triggers." << std::endl;
        eventsRecorded = m_numberOfEvents; // break out of loop
	continue;
      }
    }

    // Close the current file and open a new one if required
    if((eventsRecorded % eventsPerFile) == 0 && eventsRecorded > 0) {
      if(m_processData) {
	std::cout << eventsRecorded << "%" << eventsPerFile << "=" << (eventsRecorded % eventsPerFile) << std::endl;
	if((retVal = m_rootTreeOutput->finalize()) != 0) return retVal;
	m_fileCounter++;
	fileName = IOmethods::createFileName(m_outputFileNamePrefix,
					     m_startTimeStr, 
					     m_fileCounter,
					     ".root");
	if((retVal = m_rootTreeOutput->initialize(fileName)) != 0) return retVal;
      }
      else {
	if((retVal = m_binaryIO->finalize()) != 0) return retVal;
	m_fileCounter++;
	fileName = IOmethods::createFileName(m_outputFileNamePrefix,
					     m_startTimeStr, 
					     m_fileCounter,
					     ".bin");
	if((retVal = m_binaryIO->initialize(fileName)) != 0) return retVal;
      }
    }

    // Arm trigger and then read 0 to 4 traces from scope.
    std::vector<char *> traces = m_lecroyScope->readTraces();

    itr = traces.begin();
    itr_end = traces.end();
    while(itr != itr_end) {
      numBytes = m_traceDecoder->decodeHeader((*itr), 
					      &firstBytePos, 
					      LeCroyDsoBinaryDecoder::traceBufferSize);
      if(numBytes <= 0) return 1;
      
      // Process and save in Root format
      if(m_processData) {
	trace = m_traceDecoder->decodeTrace((*itr), 
					    LeCroyDsoBinaryDecoder::traceBufferSize,
					    firstBytePos, numBytes);
        trace->triggerNumber = eventsRecorded;
	m_rootTreeOutput->saveTrace(trace);
      }   
      // Save as binary output
      else {
	m_binaryIO->saveTrace(eventsRecorded, (*itr), firstBytePos+numBytes);
      }
      
      // Memory clean up
      delete (*itr);
      if(trace) delete trace;

      itr++;
    }
    if(traces.size() == 0) {
      std::cerr << "Warning: no traces were read from the scope" << std::endl;
    }
    
    eventsRecorded++;
  }
  
  return 0;
}

//--------------------------------------------------------------------

int LeCroyScopeDaq::finalize() {
  int retVal = 0;
  if((retVal = m_lecroyScope->finalize()) != 0) return retVal;
  if(m_processData) {
    if((retVal = m_rootTreeOutput->finalize()) != 0) return retVal;
  }
  else {
    if((retVal = m_binaryIO->finalize()) != 0) return retVal;
  }
  std::cout << std::endl << ">> Finished." << std::endl;
  return 0;
}

//--------------------------------------------------------------------
