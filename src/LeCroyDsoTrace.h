#ifndef LECROY_DSO_TRACE
#define LECROY_DSO_TRACE

#include <string>
#include <iostream>
#include <vector>

#include "LeCroyDsoTimeStamp.h"

class LeCroyDsoTrace {
 public:
  LeCroyDsoTrace();
  void printValues();  

  // A value to store a trigger number chosen by the readout program
  long triggerNumber;

  // Variables from the wave descriptor header.
  std::string descriptorName;
  std::string templateName;
  unsigned short commType;
  unsigned short commOrder;
  long waveDescriptorLength;
  long userTextLength;
  long resDesc1;
  long triggerTimeArrayLength;
  long riseTimeArrayLength;
  long resArray1Length;
  long waveArray1Length;
  long waveArray2Length;
  long resArray2Length;
  long resArray3Length;
  std::string instrumentName;
  long instrumentNumber;
  std::string traceLabel;
  int reserved1;
  int reserved2;
  long waveArrayCount;
  long pointsPerScreen;
  long firstValidPoint;
  long lastValidPoint;
  long firstPoint;
  long sparsingFactor;
  long segmentIndex;
  long subArrayCount;
  long sweepsPerAcq;
  int pointsPerPair;
  int pairOffset;
  float verticalGain;
  float verticalOffset;
  float maxValue;
  float minValue;
  int nominalBits;
  int nominalSubArrayCount;
  float horizontalInterval;
  double horizontalOffset;
  double pixelOffset;
  std::string verticalUnit;
  std::string horizontalUnit;
  float horizontalUncertainty;
  LeCroyDsoTimeStamp triggerTime;
  float acquisitionDuration;
  unsigned short recordType;
  unsigned short processingDone;
  int reserved5;
  int risSweeps;
  unsigned short timeBase;
  unsigned short verticalCoupling;
  float probeAttenuation;
  unsigned short fixedVerticalGain;
  unsigned short bandwidthLimit;
  float verticalVernier;
  float acqVerticalOffset;
  unsigned short waveSource;

  // Only filled when in sequence mode.
  std::vector<double> trigTimeArray;
  std::vector<double> trigOffsetArray;

  // The contents of wave array 1 read back from the scope.  Units are
  // in ADC counts.  Corresponds to SIMPLE_ARRAY query with INSPECT
  std::vector<int> rawDataArray;

  // The values from rawDataArray converted into an amplitude measured
  // in volts using the information found in the wave descriptor header.
  std::vector<float> waveArray;
};

#endif
