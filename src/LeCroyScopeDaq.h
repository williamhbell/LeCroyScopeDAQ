#ifndef LECROY_SCOPE_DAQ_H
#define LECROY_SCOPE_DAQ_H

#include "ApplicationBase.h"

#include <string>
#include "Keyboard.h"

/** 
 * Where possible functionality should be collected together into other classes
 * rather than appended to this class.
 */
class LeCroyScopeDaq: public ApplicationBase {
 public:
  LeCroyScopeDaq(long numberOfEvents,
		 std::string parametersFileName,
		 std::string outputFileNamePrefix,
		 bool processData,
		 bool debug);

  virtual ~LeCroyScopeDaq();

  int initialize(void); // Setup the 'services'
  int run(void); // The main event loop
  int finalize(void); // Clean up and flush memory to file.

 private:
  long m_numberOfEvents;
  std::string m_parametersFileName;
  std::string m_outputFileNamePrefix;
  bool m_processData;

  // Variables used for file name suffix.
  std::string m_startTimeStr;
  int m_fileCounter;

  // Used for testing for keyboard hit.
  Keyboard m_keyboard;
};

#endif
