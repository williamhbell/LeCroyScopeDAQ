#include "ApplicationBase.h"

#include "ScopeSettings.h"
#include "LeCroyScope.h"
#include "RootTreeOutput.h"
#include "LeCroyTraceDecoder.h"
#include "BinaryIO.h"

ApplicationBase::ApplicationBase(): m_scopeSettings(0) { 
}

ApplicationBase::~ApplicationBase() {
  // Delete 'services' pointers
  delete m_scopeSettings;
}

ScopeSettings* ApplicationBase::scopeSettings(void) {
  return m_scopeSettings;
}
