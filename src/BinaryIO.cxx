#include "BinaryIO.h"
#include "IOmethods.h"
#include <iostream>

BinaryIO::BinaryIO(bool writeMode): m_writeMode(writeMode) {
}

//--------------------------------------------------------------------

int BinaryIO::initialize(std::string fileName) {
  if(m_writeMode) {
    m_outputFile.open(fileName.c_str(), std::ios::out | std::ios::binary);
    if(!m_outputFile.is_open()) {
      std::cerr << "Error: could not open " << fileName << " as an output file." << std::endl;
      return 1;
    }
  }
  else {
    m_inputFile.open (fileName.c_str(), std::ios::in | std::ios::binary);
    if(!m_inputFile.is_open()) {
      std::cerr << "Error: could not open " << fileName << " as an input file." << std::endl;
      return 1;
    }
  }
  
  return 0;
}

//--------------------------------------------------------------------

int BinaryIO::saveTrace(unsigned long triggerNumber, char* traceBuffer, long numBytes) {

  // Check for null pointer
  if(traceBuffer == 0) {
    std::cerr << "Error null trace pointer found" << std::endl;
    return 1;
  }
  
  // Write the trigger number in front of every trace
  writeTriggerNumber(triggerNumber);
  
  // Write binary data, including prefix and header.
  m_outputFile.write(traceBuffer, numBytes);
  
  return 0;
}

//--------------------------------------------------------------------

long BinaryIO::readTrace(unsigned long *triggerNumber, char *traceBuffer, long bufferSize) {
  long numBytes;
  char byte;
  int i;
  bool found;
  char tmpstr[20];

  // Check to see if the end of file has been reached
  m_inputFile.peek(); // Causes eof to be set if eof is next.
  if(m_inputFile.eof()) {
    return -1;
  }

  // Read the trigger number, leaving the cursor pointing at the trace.
  *triggerNumber = readTriggerNumber();

  // Find the beginning of the header
  found = false;
  while(!found && m_inputFile.good()) {
    byte = m_inputFile.get();
    if(byte == '#') {
      found = true;
    }
  }
  if(!m_inputFile.good() || !found) { 
    std::cerr << "Error: beginning of trace header could not be found." << std::endl;
    return 0;
  }

  // Skip over the number of ascii integers (normally 9)
  m_inputFile.get();

  // Find the beginning of the trace "WAVEDESC..."
  found = false;
  i=0;
  while(!found && m_inputFile.good() && i<20) {
    byte = m_inputFile.get();
    if(byte == 'W') {
      found = true;
    }
    else {
      tmpstr[i] = byte;
      i++;
    }
  }
  if(!m_inputFile.good() || !found) { 
    std::cerr << "Error: beginning of trace could not be found." << std::endl;
    return 0;
  }
  if(i >= 20) {
    std::cerr << "Error: buffer used for header is too short." << std::endl;
    return 0;
  }

  // Rewind such that 'W' is the next character to be read
  m_inputFile.unget();

  // Append string terminator to allow cstring to be parsed.
  tmpstr[i] = '\0';
  numBytes = IOmethods::atol(std::string(tmpstr));

  if(numBytes > bufferSize) { 
    std::cerr << "Error: number of bytes in trace is greater than buffer size." << std::endl;
    return 0;
  }

  // Read the trace information
  m_inputFile.read(traceBuffer, numBytes);

  return numBytes;
}

//--------------------------------------------------------------------

int BinaryIO::finalize() {
  if(m_writeMode) {
    m_outputFile.close();
  }
  else {
    m_inputFile.close();
  }

  return 0;
}

//--------------------------------------------------------------------

void BinaryIO::writeTriggerNumber(unsigned long triggerNumber) {
  unsigned char *byte;
  char charBuffer[4];
  
  int ibyte, max_byte = 4;
  int mask = 255;

  // Pack the ulong into the buffer
  for(ibyte=0; ibyte<max_byte; ibyte++) {
    byte = (unsigned char *)&charBuffer[ibyte];
    *byte = (triggerNumber >> ibyte) & mask;
  }

  m_outputFile.write(charBuffer, max_byte);
}

//--------------------------------------------------------------------

unsigned long BinaryIO::readTriggerNumber() {
  unsigned char *byte;
  char charBuffer[4];
  unsigned long triggerNumber = 0;
  
  int ibyte, max_byte = 4;
  int mask = 255;
  
  m_inputFile.read(charBuffer, max_byte);
  
  // Convert the buffer back to the ulong
  for(ibyte=0; ibyte<max_byte; ibyte++) {
    byte = (unsigned char *)&charBuffer[ibyte];
    triggerNumber += (long)*byte << (ibyte * 8);
  }
  
  return triggerNumber;
}

//--------------------------------------------------------------------
