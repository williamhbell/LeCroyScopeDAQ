#ifndef LECROY_DSO_BINARY_DECODER_H
#define LECROY_DSO_BINARY_DECODER_H

#include <string>
#include <vector>
#include "LeCroyDsoTimeStamp.h"

class LeCroyDsoBinaryDecoder {
 public:
  LeCroyDsoBinaryDecoder(): m_highestFirst(true),
                            m_comType(1) {
  }
  
  void highestFirst(bool setting) {
    m_highestFirst = setting;
  }

  void communicationType(unsigned short comType) {
    m_comType = comType;
  }

  std::string bytesToString(char *charBuffer, unsigned long offset, unsigned short maxLength);
  short byteToShort(char *charBuffer, unsigned long offset);
  unsigned short bytesToEnum(char *charBuffer, unsigned long offset);
  int bytesToInt(char *charBuffer, unsigned long offset);
  long bytesToLong(char *charBuffer, unsigned long offset);
  float bytesToFloat(char *charBuffer, unsigned long offset);
  double bytesToDouble(char *charBuffer, unsigned long offset);
  
  LeCroyDsoTimeStamp bytesToTimeStamp(char *charBuffer, unsigned long offset);

  std::vector<double> readTrigTimeArray(char *charBuffer, unsigned long offset, 
					long numBytes, 
					std::vector<double> *triggerOffsetVector);

  std::vector<int> readDataArray(char *charBuffer, unsigned long offset, long numPoints);

  // Buffer sizes and read time outs.
  static const long traceBufferSize = 10485760; // 10M
  static const long traceReadTimeOut = 100; // Time in s
  static const long debugBufferSize = 104857600; // 100M
  static const long debugReadTimeOut = 1000; // Time in s

 private:
  bool m_highestFirst;
  unsigned short m_comType;
};

#endif
