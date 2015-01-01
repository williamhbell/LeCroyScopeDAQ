#ifndef LECROY_TRACE_DECODER_H
#define LECROY_TRACE_DECODER_H

#include <string>

class LeCroyDsoTrace;
class ApplicationBase;
class LeCroyDsoBinaryDecoder;

class LeCroyTraceDecoder {
 public:
  LeCroyTraceDecoder(ApplicationBase *appBase);
  ~LeCroyTraceDecoder();

  long decodeHeader(char *charBuffer, 
		    int *firstBytePos, 
		    long charBufferSize);
  
  LeCroyDsoTrace* decodeTrace(char *charBuffer, 
			      long charBufferSize,
			      int first_byte,
			      long number_of_bytes);

 private:
  int adcToVoltage(LeCroyDsoTrace *trace);

  ApplicationBase *m_appBase;
  bool m_debug;
  LeCroyDsoBinaryDecoder *m_binaryDecoder;
};

#endif
