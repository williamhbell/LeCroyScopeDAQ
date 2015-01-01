#include "LeCroyDsoTimeStamp.h"

LeCroyDsoTimeStamp::LeCroyDsoTimeStamp(): seconds(0),
    minutes(0),
    hours(0),
    days(0),
    months(0),
    years(0) {
}

std::ostream& operator << (std::ostream& os, LeCroyDsoTimeStamp& timeStamp) {
  return os << timeStamp.hours << ":" << timeStamp.minutes << ":" << timeStamp.seconds 
	    << " " 
	    << timeStamp.years << "-" << timeStamp.months << "-" << timeStamp.days;
}
