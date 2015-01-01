#ifndef BINARY_IO_H
#define BINARY_IO_H

#include <string>
#include <fstream>
#include <vector>

class BinaryIO {
 public:
  BinaryIO(bool writeMode);
  
  // Open ROOT output file, create tree and add branches.
  int initialize(std::string fileName);

  // Returns 0 on success or non-zero on error
  int saveTrace(unsigned long triggerNumber, char* traceBuffer, long numBytes);

  // Returns the number of bytes in the trace or 0 on error
  long readTrace(unsigned long *triggerNumber, char *traceBuffer, long bufferSize);

  // Flush buffers.
  int finalize();

 private:
  void writeTriggerNumber(unsigned long triggerNumber);
  unsigned long readTriggerNumber();

  bool m_writeMode;

  std::ofstream m_outputFile;
  std::ifstream m_inputFile;
};

#endif
