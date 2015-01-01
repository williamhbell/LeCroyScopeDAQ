#include "LeCroyDataProcessor.h"
#include "LeCroyTraceDecoder.h"
#include "LeCroyDsoBinaryDecoder.h"
#include "LeCroyDsoTrace.h"
#include "BinaryIO.h"
#include "ScopeSettings.h"
#include "LeCroyScope.h"
#include "RootTreeOutput.h" 

#include <iostream>
#include <cmath>

//-------------------------------------------------------------------

LeCroyDataProcessor::LeCroyDataProcessor(long numberOfEvents, 
					 std::string fileName,
					 bool debug): ApplicationBase(),
						      m_numberOfEvents(numberOfEvents), 
						      m_fileName(fileName) {
  m_debug = debug;
}

//-------------------------------------------------------------------

LeCroyDataProcessor::~LeCroyDataProcessor() {
}

//-------------------------------------------------------------------

int LeCroyDataProcessor::initialize(void) {
  std::string rootFileName;
  int pos;

  if((pos = m_fileName.find(".bin")) == std::string::npos) {
    std::cerr << "Error: input binary file does not contain the suffix \".bin\"" << std::endl;
    return 1;
  }
  rootFileName = m_fileName;
  rootFileName.replace(pos,4,".root");

  // Create a trace decoder.
  m_traceDecoder = new LeCroyTraceDecoder(this);

  // Open binary input file
  m_binaryIO = new BinaryIO(false);
  if(m_binaryIO->initialize(m_fileName)) {
    std::cerr << "Error: BinaryIO initialize() failed." << std::endl;
    return 2;
  }

  // Open root output file
  m_rootTreeOutput = new RootTreeOutput();
  if(m_rootTreeOutput->initialize(rootFileName)) {
    std::cerr << "Error: RootTreeOutput initialize() failed." << std::endl;
    return 3;
  }

  return 0;
}

//--------------------------------------------------------------------

// read some or all events from binary file, process and save in root form.

int LeCroyDataProcessor::run(void) {
  unsigned long eventsProcessed, triggerNumber = 0;
  unsigned long lastTriggerNumber = 1; // Set to 1 to get into loop correctly

  long numBytes;
  bool endOfFile;
  
  char *traceBuffer = new char[LeCroyDsoBinaryDecoder::traceBufferSize];
  LeCroyDsoTrace *trace;

  std::cout << std::endl << ">> Running until " << m_numberOfEvents << " triggers have been processed or the end of file has been reached." << std::endl;
  
  eventsProcessed = 0;
  endOfFile = false;
  while(eventsProcessed < m_numberOfEvents && triggerNumber != lastTriggerNumber && !endOfFile) {
    numBytes = m_binaryIO->readTrace(&triggerNumber, traceBuffer, 
				     LeCroyDsoBinaryDecoder::traceBufferSize);
    if(numBytes <= 0) {
      endOfFile = true;
      continue;
    }
    
    trace = m_traceDecoder->decodeTrace(traceBuffer, 
					LeCroyDsoBinaryDecoder::traceBufferSize,
					0, numBytes);

    trace->triggerNumber = triggerNumber;

    m_rootTreeOutput->saveTrace(trace);

    // Memory clean up
    delete trace;

    // If this trace belongs to a new trigger number increment the
    // event counter
    if(triggerNumber != lastTriggerNumber) {
      eventsProcessed++;
    }
  }

  delete traceBuffer;
  
  return 0;
}

//--------------------------------------------------------------------

int LeCroyDataProcessor::finalize() {
  int retVal = 0;
  if((retVal = m_rootTreeOutput->finalize()) != 0) return retVal;
  if((retVal = m_binaryIO->finalize()) != 0) return retVal;
  std::cout << std::endl << ">> Finished." << std::endl;
  return 0;
}

//--------------------------------------------------------------------
