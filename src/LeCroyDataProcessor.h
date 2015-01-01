#ifndef LECROY_DATA_PROCESSOR_H
#define LECROY_DATA_PROCESSOR_H

#include "ApplicationBase.h"

#include <string>

class LeCroyDataProcessor: public ApplicationBase {
 public:
  LeCroyDataProcessor(long numberOfEvents,
		      std::string fileName,
		      bool debug);   
  
  virtual ~LeCroyDataProcessor();
  
  int initialize(void); // Setup the 'services'
  int run(void); // The main event loop
  int finalize(void); // Clean up and flush memory to file.

 private:
  long m_numberOfEvents;
  std::string m_fileName;
};

#endif
