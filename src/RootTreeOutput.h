#ifndef ROOT_TREE_OUTPUT_H
#define ROOT_TREE_OUTPUT_H

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TArrayF.h>

#include <string>

class LeCroyDsoTrace;

class RootTreeOutput {
 public:
  RootTreeOutput();

  ~RootTreeOutput();

  // Open ROOT output file, create tree and add branches.
  int initialize(std::string fileName);

  // Write all traces in this event to the root file.
  int saveTrace(LeCroyDsoTrace *trace);
  
  // Flush buffers to the output file.
  int finalize();

 private:
  // File pointer to write to.
  TFile *m_outputFile;

  // Output TTree
  TTree *m_tree;

  // Branch variables

  // Additional counter to keep traces together
  UInt_t m_triggerNumber;

  // Trace header in Root form.
  char m_descriptorName[100];
  char m_templateName[100];
  UChar_t m_commType;
  UChar_t m_commOrder;
  Int_t m_waveDescriptorLength;
  Int_t m_userTextLength;
  Int_t m_resDesc1;
  Int_t m_triggerTimeArrayLength;
  Int_t m_riseTimeArrayLength;
  Int_t m_resArray1Length;
  Int_t m_waveArray1Length;
  Int_t m_waveArray2Length;
  Int_t m_resArray2Length;
  Int_t m_resArray3Length;
  char m_instrumentName[100];
  Int_t m_instrumentNumber;
  char m_traceLabel[100];
  Short_t m_reserved1;
  Short_t m_reserved2;
  Int_t m_waveArrayCount;
  Int_t m_pointsPerScreen;
  Int_t m_firstValidPoint;
  Int_t m_lastValidPoint;
  Int_t m_firstPoint;
  Int_t m_sparsingFactor;
  Int_t m_segmentIndex;
  Int_t m_subArrayCount;
  Int_t m_sweepsPerAcq;
  Short_t m_pointsPerPair;
  Short_t m_pairOffset;
  Float_t m_verticalGain;
  Float_t m_verticalOffset;
  Float_t m_maxValue;
  Float_t m_minValue;
  Short_t m_nominalBits;
  Short_t m_nominalSubArrayCount;
  Float_t m_horizontalInterval;
  Double_t m_horizontalOffset;
  Double_t m_pixelOffset;
  char m_verticalUnit[100];
  char m_horizontalUnit[100];
  Float_t m_horizontalUncertainty;

  // TriggerTime time in Root form.
  Double_t m_triggerSeconds;
  UShort_t m_triggerMinutes;
  UShort_t m_triggerHours;
  UShort_t m_triggerDays;
  UShort_t m_triggerMonths;

  UInt_t m_triggerYears;
  Float_t m_acquisitionDuration;
  UChar_t m_recordType;
  UChar_t m_processingDone;
  Short_t m_reserved5;
  Short_t m_risSweeps;
  UChar_t m_timeBase;
  UChar_t m_verticalCoupling;
  Float_t m_probeAttenuation;
  UChar_t m_fixedVerticalGain;
  UChar_t m_bandwidthLimit;
  Float_t m_verticalVernier;
  Float_t m_acqVerticalOffset;
  UChar_t m_waveSource;

  // TRIGTIME: ARRAY
  Int_t m_trigTimeArrayCount;
  Double_t m_trigTimeArray[2000];
  Double_t m_trigOffsetArray[2000];
  static const long m_trigTimeArray_size = 2000;


  // SIMPLE: ARRAY
  Int_t m_rawDataArray[1000000];
  static const long m_rawDataArray_size = 1000000;
};

#endif
