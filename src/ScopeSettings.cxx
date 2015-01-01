#include "ScopeSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>


ChannelConfig::ChannelConfig(void) {
  for(int i=0;i<4;i++) {
    isEnabled[i] = false;
    name[i] = "None";
    coupling[i] = "GND";
    offset[i] = 0.0; 
    voltage_divisions[i] = 1.0;
  }
}

ScopeSettings::ScopeSettings(std::string parameterFileName): m_parameterFileName(parameterFileName),
							     m_ip_address("0.0.0.0"),
							     m_tcp_timeout(10),
							     m_channelConfig(),
							     m_time_base(1.0E-9),
                                                             m_sequence(0),
							     m_events_per_file(500),
							     m_trigger_source("C1"),
							     m_trigger_slope("NEG"),
							     m_trigger_level(0.1),
							     m_trigger_type("EDGE,SR") {
}  

//----------------------------------------------------------------------

int ScopeSettings::initialize() {
  // Parameters setting the size of the character buffers
  int MAX_LINE_LENGTH = 500;
  int MAX_WORD_LENGTH = 500;
  int MAX_FILE_LENGTH = 500;

  // Character buffers
  char str[MAX_LINE_LENGTH];
  char tmpstr[MAX_WORD_LENGTH];

  // Variables used while parsing the file.
  int lineLength,i,j,wordNumber,lineNumber;

  // Strings to contain the parameter and value as a string
  std::string parameterStr, valueStr;

  // Open an input file
  std::ifstream file(m_parameterFileName.c_str());
  if(!file) {
    std::cerr << "Error: could not open " << m_parameterFileName << std::endl;
    return 1;
  }
  
  // Loop over lines in the text file.
  lineNumber=0;
  while(!file.eof() && lineNumber<MAX_FILE_LENGTH) {
    file.getline(str,MAX_LINE_LENGTH);
    lineLength = strlen(str);
    
    // Loop over characters in the line
    parameterStr.clear();
    valueStr.clear();
    i=0;
    wordNumber=0;
    while(i<lineLength && i<MAX_LINE_LENGTH) {
      // Skip over white spaces.
      while((str[i]==' ' || str[i]=='\t' ) && i<lineLength && i<MAX_LINE_LENGTH) {
	i++;
      }

      // This line is a comment
      if(str[i]=='#') {
	i = lineLength; // break out of the loop
	continue;
      }

      // Copy characters while it it not a white space or comment.
      j=0;
      while(str[i]!=' ' && str[i]!='\t' && str[i]!='#' &&
	    i<lineLength && i<MAX_LINE_LENGTH && j<MAX_WORD_LENGTH) {
	tmpstr[j] = str[i];
	i++;
	j++;
      }
      tmpstr[j] = '\0'; // Add the C string terminator

      if(strlen(tmpstr)>0) {	
	if(wordNumber==0) {
	  parameterStr.append(tmpstr);
	}
	else if(wordNumber==1) {
	  valueStr.append(tmpstr);
	}
	else {
	  i = lineLength; // break out of the loop
	}
      }
      wordNumber++;
    }
    
    std::istringstream inStr(valueStr);
    if(parameterStr.length()>0 && valueStr.length()>0) {

      // ------------ Communication Settings ---------
      if(parameterStr == "ip_address") {
	inStr >> m_ip_address;
      }
      else if(parameterStr == "tcp_timeout") {
	inStr >> m_tcp_timeout;
      }

      // ------------ Channel Settings ---------------

      // Connections
      else if(parameterStr == "channel_1") {
        inStr >> m_channelConfig.name[0];
      }
      else if(parameterStr == "channel_2") {
        inStr >> m_channelConfig.name[1];
      }
      else if(parameterStr == "channel_3") {
        inStr >> m_channelConfig.name[2];
      }
      else if(parameterStr == "channel_4") {
        inStr >> m_channelConfig.name[3];
      }
      
      // Coupling
      else if(parameterStr == "coupling_ch1") {
        inStr >> m_channelConfig.coupling[0];
      }
      else if(parameterStr == "coupling_ch2") {
        inStr >> m_channelConfig.coupling[1];
      }
      else if(parameterStr == "coupling_ch3") {
        inStr >> m_channelConfig.coupling[2];
      }
      else if(parameterStr == "coupling_ch4") {
        inStr >> m_channelConfig.coupling[3];
      }
      
      // Offset
      else if(parameterStr == "offset_ch1") {
        inStr >> m_channelConfig.offset[0];
      }
      else if(parameterStr == "offset_ch2") {
        inStr >> m_channelConfig.offset[1];
      }
      else if(parameterStr == "offset_ch3") {
        inStr >> m_channelConfig.offset[2];
      }
      else if(parameterStr == "offset_ch4") {
        inStr >> m_channelConfig.offset[3];
      }

      // Voltage per division
      else if(parameterStr == "voltage_divisions_ch1") {
        inStr >> m_channelConfig.voltage_divisions[0];
      }
      else if(parameterStr == "voltage_divisions_ch2") {
        inStr >> m_channelConfig.voltage_divisions[1];
      }
      else if(parameterStr == "voltage_divisions_ch3") {
        inStr >> m_channelConfig.voltage_divisions[2];
      }
      else if(parameterStr == "voltage_divisions_ch4") {
        inStr >> m_channelConfig.voltage_divisions[3];
      }

      // ------------ General Settings ---------------

      else if(parameterStr == "time_base") {
        inStr >> m_time_base;
      }
      else if(parameterStr == "sequence") {
        inStr >> m_sequence;
      }
      else if(parameterStr == "events_per_file") {
        inStr >> m_events_per_file;
      }

      // ------------ Trigger Settings ---------------

      else if(parameterStr == "trigger_source") {
        inStr >> m_trigger_source;
      }
      else if(parameterStr == "trigger_slope") {
        inStr >> m_trigger_slope;
      }
      else if(parameterStr == "trigger_level") {
        inStr >> m_trigger_level;
      }
      else if(parameterStr == "trigger_type") {
        inStr >> m_trigger_type;
      }

      // ---------------------------------------------

      else {
        std::cerr << "Warning: " << parameterStr << " is an unknown parameter." << std::endl; 
      }
    }

    lineNumber++;
  }
  
  file.close();

  // Set channel status (default is false)
  for(i = 0; i<4; i++) {
    if(m_channelConfig.name[i] != "Off" &&  m_channelConfig.name[i] != "off") {
      m_channelConfig.isEnabled[i] = true;
    }
  }

  // Check if the inputs are valid.
  int ret_val;
  if(!(ret_val = checkInputs())) return ret_val;
  

  return 0;
}

//----------------------------------------------------------------------

int ScopeSettings::checkInputs() {
  for(int i = 0; i<4; i++) {
    if(m_channelConfig.coupling[i] != "A1M*" &&
       m_channelConfig.coupling[i] != "D1M*" &&
       m_channelConfig.coupling[i] != "D50" &&
       m_channelConfig.coupling[i] != "GND") {
      std::cerr << "Error: invalid coupling requested " << m_channelConfig.coupling[i] << std::endl;
      return 1;
    }
  }

  if(m_trigger_source != "C1" && 
     m_trigger_source != "C2" && 
     m_trigger_source != "C3" && 
     m_trigger_source != "C4" && 
     m_trigger_source != "EX") {
    std::cerr << "Error: invalid trigger source " << m_trigger_source << std::endl; 
    return 1;
  }

  if(m_trigger_slope != "NEG" && 
     m_trigger_slope != "POS") {
    std::cerr << "Error: invalid trigger slope requested " << m_trigger_slope << std::endl;
    return 1;
  }

  return 0;
}

//----------------------------------------------------------------------

void ScopeSettings::printParameterValues(void) {
  int i;

  std::cout << "==============================================" << std::endl;
  std::cout << "          Scope Parameters and Values         " << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "-- Communication Settings --" << std::endl;
  std::cout << "ip_address = " << m_ip_address << std::endl;
  std::cout << "tcp_timeout = " << m_tcp_timeout << "s" << std::endl;
  std::cout << std::endl;
  std::cout << "-- Channel Settings --" << std::endl;
  for(i=0;i<4;i++) std::cout << "channel_" << (i+1) << " = " << m_channelConfig.name[i] << std::endl;  
  for(i=0;i<4;i++) std::cout << "coupling_ch" << (i+1) << " = " << m_channelConfig.coupling[i] << std::endl;
  for(i=0;i<4;i++) std::cout << std::scientific << "offset_ch" << (i+1) << " = " << m_channelConfig.offset[i] << " V" << std::endl;
  for(i=0;i<4;i++) std::cout << std::scientific << "voltage_divisions_ch" << (i+1) << " = " << m_channelConfig.voltage_divisions[i] << " V" << std::endl;
  std::cout << std::endl;
  std::cout << "-- General Settings --" << std::endl;
  std::cout << std::scientific << "time_base = " << m_time_base << " s" << std::endl;
  std::cout << "sequence = " << m_sequence << std::endl;
  std::cout << "events_per_file = " << m_events_per_file << std::endl;
  std::cout << std::endl;
  std::cout << "-- Trigger Settings --" << std::endl;
  std::cout << "trigger_source = " << m_trigger_source << std::endl;
  std::cout << "trigger_slope = " << m_trigger_slope << std::endl;
  std::cout << std::scientific << "trigger_level = " << m_trigger_level << " V" << std::endl;
  std::cout << "trigger_type = " << m_trigger_type << std::endl;
  std::cout << "==============================================" << std::endl;
}

