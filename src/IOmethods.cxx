#include "IOmethods.h"
#include <sstream>
#include <ctime>

//--------------------------------------------------------------------

std::string IOmethods::itoa(int i) {
  std::ostringstream outStr;
  outStr << i;
  return outStr.str();
}

//--------------------------------------------------------------------

std::string IOmethods::ftoa(float f) {
  std::ostringstream outStr;
  outStr << f;
  return outStr.str();
}

//-----------------------------------------------------------------

long IOmethods::atol(std::string str) {
  long i;
  std::istringstream inStr(str);
  inStr >> i;
  return i;
}

//-----------------------------------------------------------------

std::string IOmethods::currentTimeString() {
  std::string timeString;

  time_t rawtime;
  struct tm* timeinfo;
  char tmpstr[80];
  
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(tmpstr,80,"%Y%m%d_%H%M%S",timeinfo);
  
  timeString = tmpstr;

  return timeString;
}

//-----------------------------------------------------------------

std::string IOmethods::createFileName(std::string filePrefix,
				      std::string startTime, 
				      long fileNumber,
				      std::string suffix) {
  std::string fileName = filePrefix;

  // Append the start time.
  fileName.append("_");
  fileName.append(startTime);
  fileName.append("_");
  
  // Convert fileCounter to string 4 wide
  std::ostringstream outStr;
  outStr.fill('0');
  outStr.width(4);
  outStr << fileNumber;
  fileName.append(outStr.str());

  fileName.append(suffix);

  return fileName;
}

//-----------------------------------------------------------------
