#include "LeCroyScope.h"

#include "ScopeSettings.h"
#include "ApplicationBase.h"
#include "LeCroyDsoBinaryDecoder.h"
#include "LeCroyDsoTrace.h"
#include "IOmethods.h"

extern "C" {
  #include "lecroy_tcp.h"
}

#include <iostream>
#include <sstream>

// for debugging only:
#include <fstream> 

LeCroyScope::LeCroyScope(ApplicationBase *appBase): m_appBase(appBase),
						    m_scopeSettings(0),
						    m_sockfd(0),
						    m_enabledChannels(0),
						    m_debug(false) {
}

//--------------------------------------------------------------------

LeCroyScope::~LeCroyScope() {
}

//--------------------------------------------------------------------

int LeCroyScope::initialize(void) {
  std::string commandStr;

  m_debug = m_appBase->isDebug();

  m_scopeSettings = m_appBase->scopeSettings();
  if(!m_scopeSettings) {
    std::cerr << "Error: ScopeSettings pointer from ApplicationBase is null" << std::endl;
    return 1;
  }
  
  // Connect to the scope.
  m_sockfd=LECROY_TCP_connect(m_scopeSettings->ip_address().c_str(), m_scopeSettings->tcp_timeout());
  if (m_sockfd<0) {
    std::cerr << "Error in initialize: could not connect to the scope on IP " << m_scopeSettings->ip_address() << std::endl;
    return 1;
  }
  
  // Reset the scope
  commandStr = "*RST\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Print a string to the bottom of the scope display.
  commandStr = "MSG 'Setting up the scope...'\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Setup the offset constant in volts rather than div
  commandStr = "OFCT VOLTS\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  setupChannels();
  setupTrigger();
  setupDataFormat();

  commandStr = "TIME_DIV ";
  commandStr.append(IOmethods::ftoa(m_scopeSettings->time_base()));
  commandStr.append("S\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }
  
  // Sequence mode
  commandStr = "SEQ ";
  if(m_scopeSettings->sequence() == 0) {
    commandStr.append("OFF");
  }
  else {
    commandStr.append("ON,");
    commandStr.append(IOmethods::itoa(m_scopeSettings->sequence()));
  }
  commandStr.append("\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  commandStr = "MSG 'Scope DAQ Running'\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Turn the display off to increase speed.
  if(!m_debug) { 
    commandStr = "DISP OFF\n";
    if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
      std::cerr << "Error LECROY_TCP_write failed" << std::endl;
      return 1;
    }
  }


  //if(m_debug) return readSettings();

  LECROY_TCP_disconnect(m_sockfd);
  
  return 0;
}


//--------------------------------------------------------------------

int LeCroyScope::finalize(void) {
  std::string commandStr;
  
  // Connect to the scope.
  m_sockfd=LECROY_TCP_connect(m_scopeSettings->ip_address().c_str(), m_scopeSettings->tcp_timeout());
  if (m_sockfd<0) {
    std::cerr << "Error in finalize: could not connect to the scope on IP " << m_scopeSettings->ip_address() << std::endl;
    return 1;
  }
  
  // Turn the display back on.
  if(!m_debug) { 
    commandStr = "DISP ON\n";
    if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
      std::cerr << "Error LECROY_TCP_write failed" << std::endl;
      return 1;
    }
  }
  
  // Turn the communication header back on.
  commandStr = "CHDR SHORT\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }
  
  commandStr = "MSG 'Scope DAQ Finished'\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }
  
  LECROY_TCP_disconnect(m_sockfd);

  return 0;
}

//--------------------------------------------------------------------

int LeCroyScope::setupChannels(void) {
  int retVal = 0;
  
  for(int i=0; i<4; i++) {
    if(m_scopeSettings->channelConfig().isEnabled[i]) {
      if((retVal = setupAchannel(i+1)) != 0) return retVal;
    }
    else {
      if((retVal = disableAchannel(i+1)) != 0) return retVal;
    }
  }
  
  return 0;
}

//--------------------------------------------------------------------

int LeCroyScope::setupAchannel(unsigned int channelNumber) {
  if(channelNumber < 1 || channelNumber > 4) {
    std::cerr << "Error channel number is out of range" << channelNumber << std::endl;
    return 1;
  }

  int channelIndex = channelNumber-1;

  std::string commandStr;
  std::string channelStr = "C";
  channelStr.append(IOmethods::itoa(channelNumber));

  // Turn the trace on
  commandStr = channelStr;
  commandStr.append(":TRA ON\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Label the trace.
  //  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
  //    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
  //    return 1;
  //  }

  // Coupling of 50 Ohms.
  commandStr = channelStr;
  commandStr.append(":CPL ");
  commandStr.append(m_scopeSettings->channelConfig().coupling[channelIndex]);
  commandStr.append("\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Setup the volts per division;
  commandStr = channelStr;
  commandStr.append(":VOLT_DIV ");
  commandStr.append(IOmethods::ftoa(m_scopeSettings->channelConfig().voltage_divisions[channelIndex]));
  commandStr.append("V\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Setup the offset
  commandStr = channelStr;
  commandStr.append(":OFST ");
  commandStr.append(IOmethods::ftoa(m_scopeSettings->channelConfig().offset[channelIndex]));
  commandStr.append("V\n"); // Command will not recognise MV
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Flag the channel as enabled.
  m_enabledChannels.set(channelNumber-1, true);

  return 0;
}

//--------------------------------------------------------------------

int LeCroyScope::disableAchannel(unsigned int channelNumber) {
  if(channelNumber < 1 || channelNumber > 4) {
    std::cerr << "Error channel number is out of range" << channelNumber << std::endl;
    return 1;
  }

  std::string commandStr;
  std::string channelStr = "C";
  channelStr.append(IOmethods::itoa(channelNumber));

  // Turn the trace off
  commandStr = channelStr;
  commandStr.append(":TRA OFF\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Flag the channel as disabled.
  m_enabledChannels.set(channelNumber-1, false);

  return 0;
}

//--------------------------------------------------------------------

int LeCroyScope::setupTrigger(void) {
  std::string commandStr;

  // Check if the channel is valid.
  if(m_scopeSettings->trigger_source() != "C1" &&
     m_scopeSettings->trigger_source() != "C2" &&
     m_scopeSettings->trigger_source() != "C3" &&
     m_scopeSettings->trigger_source() != "C4") {
    std::cerr << "Error trigger source " << m_scopeSettings->trigger_source() <<  " invalid" << std::endl;
  }

  // Trigger slope
  commandStr = m_scopeSettings->trigger_source();
  commandStr.append(":TRIG_SLOPE ");
  commandStr.append(m_scopeSettings->trigger_slope());
  commandStr.append("\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Trigger level
  commandStr = m_scopeSettings->trigger_source();
  commandStr.append(":TRLV ");
  commandStr.append(IOmethods::ftoa(m_scopeSettings->trigger_level()));
  commandStr.append("V\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Trigger type
  commandStr = "TRSE ";
  commandStr.append(m_scopeSettings->trigger_type());
  commandStr.append(",SR,");
  commandStr.append(m_scopeSettings->trigger_source());
  commandStr.append("\n");
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  return 0;
}

//--------------------------------------------------------------------

int LeCroyScope::setupDataFormat(void) {
  std::string commandStr;

  // Most significant bit first.
  commandStr = "CORD HI\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }
  
  // Binary communication format
  commandStr = "CFMT DEF9,WORD,BIN\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Waveform setup: no sparsing, all data points, start from first
  // point, all sequences.
  commandStr = "WFSU SP,0,NP,0,FP,0,SN,0\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  // Turn the communication header off for the run.
  commandStr = "CHDR OFF\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  return 0;
}

//--------------------------------------------------------------------

// This is just for debug

int LeCroyScope::readSettings(void) {
  std::string commandStr;

  char *debugBuffer = new char[LeCroyDsoBinaryDecoder::debugBufferSize];

  // Get the waveform template
  commandStr = "TMPL?\n";
  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
    return 1;
  }

  LECROY_TCP_read(m_sockfd, debugBuffer, LeCroyDsoBinaryDecoder::debugBufferSize, 
		  LeCroyDsoBinaryDecoder::debugReadTimeOut);

  std::cout << debugBuffer << std::endl;
  
  delete debugBuffer;

  return 0;
}

//--------------------------------------------------------------------

std::vector<char*> LeCroyScope::readTraces() {
  std::string commandStr, channelStr;
  std::vector<char*> traces;
  std::vector<char*> nullTraces; // Empty error for return upon error.
  char *traceBuffer;

  bool triggerArmed = false;
  for(int channelNumber = 1; channelNumber <= 4; channelNumber++) {
    if(m_enabledChannels.test(channelNumber-1)) {
      channelStr = "C";
      channelStr.append(IOmethods::itoa(channelNumber));

      if(!triggerArmed) {
	// Connect to the scope once per trigger
	m_sockfd=LECROY_TCP_connect(m_scopeSettings->ip_address().c_str(), m_scopeSettings->tcp_timeout());
	if (m_sockfd<0) {
	  std::cerr << "Could not connect to the scope on IP: " << m_scopeSettings->ip_address() << std::endl;
	  return nullTraces;
	}
	
	// Arm the trigger once per event.
	commandStr = "ARM; WAIT; ";
	commandStr.append(channelStr);
	commandStr.append(":WF? ALL\n");
	if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
	  std::cerr << "Error LECROY_TCP_write failed" << std::endl;
	  return nullTraces;
	}

	triggerArmed = true;
      }
      else {
	// Read the remaining channels.
	commandStr = channelStr;
	commandStr.append(":WF? ALL\n");
	if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
	  std::cerr << "Error LECROY_TCP_write failed" << std::endl;
	  return nullTraces;
	}
      }
      
      // Create an array for this trace.
      traceBuffer = new char[LeCroyDsoBinaryDecoder::traceBufferSize];

      for(int i=0; i<LeCroyDsoBinaryDecoder::traceBufferSize; i++) { 
	traceBuffer[i] = (char)0; 
      }
      LECROY_TCP_read(m_sockfd, traceBuffer, 
		      LeCroyDsoBinaryDecoder::traceBufferSize,
		      LeCroyDsoBinaryDecoder::traceReadTimeOut);
      traces.push_back(traceBuffer);
      
      if(m_debug) {
        char *debugBuffer = new char[LeCroyDsoBinaryDecoder::debugBufferSize];
	
	// Call inspect
	commandStr = channelStr;
	commandStr.append(":INSP? WAVEDESC\n");
	if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
	  std::cerr << "Error LECROY_TCP_write failed" << std::endl;
	  return nullTraces;
	}
	
	LECROY_TCP_read(m_sockfd, debugBuffer, LeCroyDsoBinaryDecoder::debugBufferSize,
			LeCroyDsoBinaryDecoder::debugReadTimeOut);

	std::cout << debugBuffer << std::endl;

	// Call Inspect if valid
	if(m_scopeSettings->sequence()>0) {
	  commandStr = channelStr;
	  commandStr.append(":INSP? TRIGTIME\n");
	  if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
	    std::cerr << "Error LECROY_TCP_write failed" << std::endl;
	    return nullTraces;
	  }
	  
	  LECROY_TCP_read(m_sockfd, debugBuffer, LeCroyDsoBinaryDecoder::debugBufferSize,
			  LeCroyDsoBinaryDecoder::debugReadTimeOut);

	std::cout << debugBuffer << std::endl;
	}

	// Call inspect
	commandStr = channelStr;
	commandStr.append(":INSP? SIMPLE,WORD\n");
	if(LECROY_TCP_write(m_sockfd, commandStr.c_str(), m_debug) != 0) {
	  std::cerr << "Error LECROY_TCP_write failed" << std::endl;
	  return nullTraces;
	}
	
	LECROY_TCP_read(m_sockfd, debugBuffer,LeCroyDsoBinaryDecoder::debugBufferSize,
			LeCroyDsoBinaryDecoder::debugReadTimeOut);

	std::cout << debugBuffer << std::endl;

	delete debugBuffer;
      }
    }
  }
  
  if(!triggerArmed) {
    std::cerr << "Warning no channels were enabled" << std::endl;
  }
  else {
    LECROY_TCP_disconnect(m_sockfd);
  }
  
  return traces;
}
