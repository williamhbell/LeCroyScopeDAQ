#ifndef LECROY_DSO_TIME_STAMP
#define LECROY_DSO_TIME_STAMP

#include <ostream>

class LeCroyDsoTimeStamp {
 public:
  LeCroyDsoTimeStamp();

  double seconds;
  unsigned short minutes;
  unsigned short hours;
  unsigned short days;
  unsigned short months;
  unsigned long years;
};

std::ostream& operator << (std::ostream& os, LeCroyDsoTimeStamp& timeStamp);

#endif
