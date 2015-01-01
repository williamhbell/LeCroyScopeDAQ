#ifndef IOMETHODS_H
#define IOMETHODS_H

#include <string>

class IOmethods {
 public:
  static std::string itoa(int i);
  static std::string ftoa(float f);
  static long atol(std::string str);
  static std::string currentTimeString(void);
  static std::string createFileName(std::string filePrefix,
				    std::string startTime, 
				    long fileNumber,
				    std::string suffix);
};

#endif
