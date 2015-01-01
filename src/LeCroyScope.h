#ifndef LECROY_SCOPE_H
#define LECROY_SCOPE_H

#include <string>
#include <bitset>
#include <vector>

class ApplicationBase;
class ScopeSettings;

class LeCroyScope {
 public:
  
  // Construct the class with a pointer to the Analysis class to allow
  // retrieval of pointers to 'services'
  LeCroyScope(ApplicationBase *appBase);

  ~LeCroyScope();

  // Collect pointers to the 'services' needed.
  // Setup the scope channels, trigger and data format
  int initialize(void);

  // Clean up
  int finalize(void);

  std::bitset<4> enabledChannels(void) {
    return m_enabledChannels;
  }

  std::vector<char *> readTraces(void);
 

 private:
  int setupChannels(void);
  int setupAchannel(unsigned int channelNumber);
  int disableAchannel(unsigned int channelNumber);
  int setupTrigger(void);
  int setupDataFormat(void);

  // Debug code for cross checking the settings.
  int readSettings(void);

  // A pointer to the analysis class, containing any 'service'
  // pointers that are in use.q
  ApplicationBase *m_appBase;

  // Temporary pointers to 'services' retrieved from the analysis
  // class.
  ScopeSettings *m_scopeSettings;

  // For checking track of the TCPIP connection
  int m_sockfd;

  std::bitset<4> m_enabledChannels;

  bool m_debug;
};

#endif
