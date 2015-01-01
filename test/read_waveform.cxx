#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "LeCroyTraceDecoder.h"
#include "LeCroyScopeDaq.h"

// 10M character buffer
static const int m_charBufferSize = 10485760;
char m_charBuffer[10485760];
bool m_highest_first = true;

//--------------------------------------------------------------------

int main() {
  int i;
  for(i=0; i<m_charBufferSize; i++) { 
    m_charBuffer[i] = (char)0; 
  }

  std::ifstream binFile ("waveform.bin", std::ios::in | std::ios::binary);
  if(!binFile) {
    std::cerr << "Error: could not open waveform.bin" << std::endl;
    return 1;
  }
  binFile.read(m_charBuffer, m_charBufferSize);
  binFile.close();

  int firstBytePos;
  long numBytes;
  
  LeCroyTraceDecoder *m_traceDecoder = new LeCroyTraceDecoder(new LeCroyScopeDaq(1,"test","/dev/null",true,true));
  
  LeCroyDsoTrace *trace;

  numBytes = m_traceDecoder->decodeHeader(m_charBuffer, 
					  &firstBytePos, 
					  m_charBufferSize);
  if(numBytes <= 0) return 1;
  
  // Process and save in Root format
  trace = m_traceDecoder->decodeTrace(m_charBuffer, 
				      m_charBufferSize,
				      firstBytePos, numBytes);
  //  trace->triggerNumber = eventsRecorded;
  //      m_rootTreeOutput->saveTrace(trace);

  
  return 0;
}
