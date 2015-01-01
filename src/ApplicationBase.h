#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

class ScopeSettings;
class LeCroyScope;
class RootTreeOutput;
class LeCroyTraceDecoder;
class BinaryIO;

class ApplicationBase {
 public:

  ApplicationBase();
  virtual ~ApplicationBase();

  virtual int initialize(void) = 0;  // Setup the 'services'
  virtual int run(void) = 0; // The main event loop
  virtual int finalize(void) = 0; // Clean up

  // Member functions to return pointers to the 'services'
  ScopeSettings* scopeSettings(void);

  bool isDebug() { return m_debug; }

 protected:

  // Pointers to 'services'
  ScopeSettings *m_scopeSettings;
  LeCroyScope *m_lecroyScope;
  RootTreeOutput *m_rootTreeOutput;
  LeCroyTraceDecoder *m_traceDecoder;
  BinaryIO *m_binaryIO;
 
  // Debug flag
  bool m_debug;
};
#endif
