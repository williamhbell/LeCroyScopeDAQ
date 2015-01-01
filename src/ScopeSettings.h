#ifndef SCOPE_SETTINGS_H
#define SCOPE_SETTINGS_H

#include <string>

class ChannelConfig {
 public:
  ChannelConfig(void);

  bool isEnabled[4];
  std::string name[4];
  std::string coupling[4];
  float offset[4];
  float voltage_divisions[4];
};

/**
 * A class to read scope settings and their associated values from a
 * text file.
 */
class ScopeSettings {
 public:

  ScopeSettings(std::string parameterFileName);
  int initialize(void);  // Read the parameters and their values from file.
  int checkInputs(void);
  void printParameterValues(void); // Print all of the parameters and their settings.

  
  // ========= Accessors for the different parameters ======

  // ------------ Communication Settings ---------

  std::string ip_address() {
    return m_ip_address;
  }
  unsigned short tcp_timeout() {
    return m_tcp_timeout;
  }

  // ------------ Channel Settings ---------------

  std::string channel_1() {
    return m_channelConfig.name[0];
  }

  std::string channel_2() {
    return m_channelConfig.name[1];
  }

  std::string channel_3() {
    return m_channelConfig.name[2];
  }

  std::string channel_4() {
    return m_channelConfig.name[3];
  }
  
  std::string coupling_ch1() {
    return m_channelConfig.coupling[0];
  }

  std::string coupling_ch2() {
    return m_channelConfig.coupling[1];
  }

  std::string coupling_ch3() {
    return m_channelConfig.coupling[2];
  }

  std::string coupling_ch4() {
    return m_channelConfig.coupling[3];
  }

  float offset_ch1() {
    return m_channelConfig.offset[0];
  }

  float offset_ch2() {
    return m_channelConfig.offset[1];
  }

  float offset_ch3() {
    return m_channelConfig.offset[2];
  }

  float offset_ch4() {
    return m_channelConfig.offset[3];
  }

  float voltage_divisions_ch1() {
    return m_channelConfig.voltage_divisions[0];
  }

  float voltage_divisions_ch2() {
    return m_channelConfig.voltage_divisions[1];
  }

  float voltage_divisions_ch3() {
    return m_channelConfig.voltage_divisions[2];
  }

  float voltage_divisions_ch4() {
    return m_channelConfig.voltage_divisions[3];
  }
  
  // ------------ General Settings ---------------

  float time_base() {
    return m_time_base;
  }

  unsigned short sequence() {
    return m_sequence;
  }

  unsigned long events_per_file() {
    return m_events_per_file;
  }

  // ------------ Trigger Settings ---------------

  std::string trigger_source () {
    return m_trigger_source;
  }
  
  std::string trigger_slope () {
    return m_trigger_slope;
  }
  
  float trigger_level() {
    return m_trigger_level;
  }

  std::string trigger_type () {
    return m_trigger_type;
  }
  
  // ----------------------------------------------
  
  ChannelConfig channelConfig() {
    return m_channelConfig;
  }

 private:
  std::string m_parameterFileName; // The file the setting are read from

  // A variable to store the value of each parameter
  std::string m_ip_address;
  unsigned short m_tcp_timeout;
  ChannelConfig m_channelConfig;
  float m_time_base;
  unsigned short m_sequence;
  unsigned long m_events_per_file;
  std::string m_output_file_prefix;
  std::string m_trigger_source;
  std::string m_trigger_slope;
  float m_trigger_level;
  std::string m_trigger_type;

  // The sizes of the character buffers used while reading the text
  // file.
  static const int MAX_LINE_LENGTH = 500;
  static const int MAX_WORD_LENGTH = 500;
  static const int MAX_FILE_LENGTH = 500;
};

#endif
