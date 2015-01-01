#include "RootTreeOutput.h"
#include "LeCroyDsoTrace.h"

#include <iostream>
#include <cstring>

RootTreeOutput::RootTreeOutput(): m_outputFile(0),
				  m_tree(0) {
}

//--------------------------------------------------------------------

RootTreeOutput::~RootTreeOutput() {
}

//--------------------------------------------------------------------


int RootTreeOutput::initialize(std::string fileName) {

  // Open the output root file.
  if (!m_outputFile) {
    m_outputFile = TFile::Open(fileName.c_str(),"RECREATE");
    if (!m_outputFile) {
      std::cerr << "Error: Output histogram file " << fileName << " could not be opened!" << std::endl;
      return 1;
    }
  }
  else {
    std::cerr << "Error: Output root file has already been opened!" << std::endl;
    return 2;
  }

  // Create the TTree objects.
  m_tree = new TTree("scopeTraces","LeCroy X-Stream Scope Traces");

  // Add the trace information
  m_tree->Branch("triggerNumber",&m_triggerNumber,"triggerNumber/i");
  m_tree->Branch("descriptorName",m_descriptorName,"descriptorName[100]/C");
  m_tree->Branch("templateName",m_templateName,"templateName[100]/C");
  m_tree->Branch("commType",&m_commType,"commType/b");
  m_tree->Branch("commOrder",&m_commOrder,"commOrder/b");
  m_tree->Branch("waveDescriptorLength",&m_waveDescriptorLength,"waveDescriptorLength/I");
  m_tree->Branch("userTextLength",&m_userTextLength,"userTextLength/I");
  m_tree->Branch("resDesc1",&m_resDesc1,"resDesc1/I");
  m_tree->Branch("triggerTimeArrayLength",&m_triggerTimeArrayLength,"triggerTimeArrayLength/I");  
  m_tree->Branch("riseTimeArrayLength",&m_riseTimeArrayLength,"riseTimeArrayLength/I");
  m_tree->Branch("resArray1Length",&m_resArray1Length,"resArray1Length/I");  
  m_tree->Branch("waveArray1Length",&m_waveArray1Length,"waveArray1Length/I");
  m_tree->Branch("waveArray2Length",&m_waveArray2Length,"waveArray2Length/I");
  m_tree->Branch("resArray2Length",&m_resArray2Length,"resArray2Length/I"); 
  m_tree->Branch("resArray3Length",&m_resArray3Length,"resArray3Length/I");
  m_tree->Branch("instrumentName",m_instrumentName,"instrumentName[100]/C");
  m_tree->Branch("instrumentNumber",&m_instrumentNumber,"instrumentNumber/I");
  m_tree->Branch("traceLabel",m_traceLabel,"traceLabel[100]/C");
  m_tree->Branch("reserved1",&m_reserved1,"reserved1/S");
  m_tree->Branch("reserved2",&m_reserved2,"reserved2/S");
  m_tree->Branch("waveArrayCount",&m_waveArrayCount,"waveArrayCount/I");  
  m_tree->Branch("pointsPerScreen",&m_pointsPerScreen,"pointsPerScreen/I");
  m_tree->Branch("firstValidPoint",&m_firstValidPoint,"firstValidPoint/I");
  m_tree->Branch("lastValidPoint",&m_lastValidPoint,"lastValidPoint/I");
  m_tree->Branch("firstPoint",&m_firstPoint,"firstPoint/I");
  m_tree->Branch("sparsingFactor",&m_sparsingFactor,"sparsingFactor/I");
  m_tree->Branch("segmentIndex",&m_segmentIndex,"segmentIndex/I");
  m_tree->Branch("subArrayCount",&m_subArrayCount,"subArrayCount/I");
  m_tree->Branch("sweepsPerAcq",&m_sweepsPerAcq,"sweepsPerAcq/I");
  m_tree->Branch("pointsPerPair",&m_pointsPerPair,"pointsPerPair/S");
  m_tree->Branch("pairOffset",&m_pairOffset,"pairOffset/S");
  m_tree->Branch("verticalGain",&m_verticalGain,"verticalGain/F");
  m_tree->Branch("verticalOffset",&m_verticalOffset,"verticalOffset/F");
  m_tree->Branch("maxValue",&m_maxValue,"maxValue/F");
  m_tree->Branch("minValue",&m_minValue,"minValue/F");
  m_tree->Branch("nominalBits",&m_nominalBits,"nominalBits/S");
  m_tree->Branch("nominalSubArrayCount",&m_nominalSubArrayCount,"nominalSubArrayCount/S");
  m_tree->Branch("horizontalInterval",&m_horizontalInterval,"horizontalInterval/F");
  m_tree->Branch("horizontalOffset",&m_horizontalOffset,"horizontalOffset/D");
  m_tree->Branch("pixelOffset",&m_pixelOffset,"pixelOffset/D");
  m_tree->Branch("verticalUnit",m_verticalUnit,"verticalUnit[100]/C");
  m_tree->Branch("horizontalUnit",m_horizontalUnit,"horizontalUnit[100]/C");
  m_tree->Branch("horizontalUncertainty",&m_horizontalUncertainty,"horizontalUncertainty/F");  
  m_tree->Branch("triggerSeconds",&m_triggerSeconds,"triggerSeconds/D");  
  m_tree->Branch("triggerMinutes",&m_triggerMinutes,"triggerMinutes/s");
  m_tree->Branch("triggerHours",&m_triggerHours,"triggerHours/s");  
  m_tree->Branch("triggerDays",&m_triggerDays,"triggerDays/s");  
  m_tree->Branch("triggerMonths",&m_triggerMonths,"triggerMonths/s");
  m_tree->Branch("triggerYears",&m_triggerYears,"triggerYears/i");
  m_tree->Branch("acquisitionDuration",&m_acquisitionDuration,"acquisitionDuration/F");
  m_tree->Branch("recordType",&m_recordType,"recordType/b");
  m_tree->Branch("processingDone",&m_processingDone,"processingDone/b");
  m_tree->Branch("reserved5",&m_reserved5,"reserved5/S");
  m_tree->Branch("risSweeps",&m_risSweeps,"risSweeps/S");
  m_tree->Branch("timeBase",&m_timeBase,"timeBase/b");
  m_tree->Branch("verticalCoupling",&m_verticalCoupling,"verticalCoupling/b");
  m_tree->Branch("probeAttenuation",&m_probeAttenuation,"probeAttenuation/F");
  m_tree->Branch("fixedVerticalGain",&m_fixedVerticalGain,"fixedVerticalGain/b");
  m_tree->Branch("bandwidthLimit",&m_bandwidthLimit,"bandwidthLimit/b");
  m_tree->Branch("verticalVernier",&m_verticalVernier,"verticalVernier/F");
  m_tree->Branch("acqVerticalOffset",&m_acqVerticalOffset,"acqVerticalOffset/F");
  m_tree->Branch("waveSource",&m_waveSource,"waveSource/b");
 
  m_tree->Branch("trigTimeArrayCount", &m_trigTimeArrayCount, "trigTimeArrayCount/I");
  m_tree->Branch("trigTimeArray", m_trigTimeArray, "trigTimeArray[trigTimeArrayCount]/D");
  m_tree->Branch("trigOffsetArray", m_trigOffsetArray, "trigOffsetArray[trigTimeArrayCount]/D");
  m_tree->Branch("rawDataArray", m_rawDataArray, "rawDataArray[waveArrayCount]/I");

  return 0;
}

//--------------------------------------------------------------------

int RootTreeOutput::saveTrace(LeCroyDsoTrace* trace) {
  int index, max_index;

  // Check for null pointer
  if(trace == 0) {
    std::cerr << "Error null trace pointer found" << std::endl;
    return 1;
  }
 
  m_triggerNumber = trace->triggerNumber; 
  strcpy(m_descriptorName, trace->descriptorName.c_str());
  strcpy(m_templateName, trace->templateName.c_str());
  m_commType = trace->commType;
  m_commOrder = trace->commOrder;
  m_waveDescriptorLength = trace->waveDescriptorLength;
  m_userTextLength = trace->userTextLength;
  m_resDesc1 = trace->resDesc1;
  m_triggerTimeArrayLength = trace->triggerTimeArrayLength;
  m_riseTimeArrayLength = trace->riseTimeArrayLength;
  m_resArray1Length = trace->resArray1Length;
  m_waveArray1Length = trace->waveArray1Length;
  m_waveArray2Length = trace->waveArray2Length;
  m_resArray2Length = trace->resArray2Length;
  m_resArray3Length = trace->resArray3Length;
  strcpy(m_instrumentName, trace->instrumentName.c_str());
  m_instrumentNumber = trace->instrumentNumber;
  strcpy(m_traceLabel, trace->traceLabel.c_str());
  m_reserved1 = trace->reserved1;
  m_reserved2 = trace->reserved2;
  m_waveArrayCount = trace->waveArrayCount;
  m_pointsPerScreen = trace->pointsPerScreen;
  m_firstValidPoint = trace->firstValidPoint;
  m_lastValidPoint = trace->lastValidPoint;
  m_firstPoint = trace->firstPoint;
  m_sparsingFactor = trace->sparsingFactor;
  m_segmentIndex = trace->segmentIndex;
  m_subArrayCount = trace->subArrayCount;
  m_sweepsPerAcq = trace->sweepsPerAcq;
  m_pointsPerPair = trace->pointsPerPair;
  m_pairOffset = trace->pairOffset;
  m_verticalGain = trace->verticalGain;
  m_verticalOffset = trace->verticalOffset;
  m_maxValue = trace->maxValue;
  m_minValue = trace->minValue;
  m_nominalBits = trace->nominalBits;
  m_nominalSubArrayCount = trace->nominalSubArrayCount;
  m_horizontalInterval = trace->horizontalInterval;
  m_horizontalOffset = trace->horizontalOffset;
  m_pixelOffset = trace->pixelOffset;
  strcpy(m_verticalUnit, trace->verticalUnit.c_str());
  strcpy(m_horizontalUnit, trace->horizontalUnit.c_str());
  m_horizontalUncertainty = trace->horizontalUncertainty;
  
  // TriggerTime time in Root form.
  m_triggerSeconds = trace->triggerTime.seconds;
  m_triggerMinutes = trace->triggerTime.minutes;
  m_triggerHours = trace->triggerTime.hours;
  m_triggerDays = trace->triggerTime.days;
  m_triggerMonths = trace->triggerTime.months;
  m_triggerYears = trace->triggerTime.years;
  
  m_acquisitionDuration = trace->acquisitionDuration;
  m_recordType = trace->recordType;
  m_processingDone = trace->processingDone;
  m_reserved5 = trace->reserved5;
  m_risSweeps = trace->risSweeps;
  m_timeBase = trace->timeBase;
  m_verticalCoupling = trace->verticalCoupling;
  m_probeAttenuation = trace->probeAttenuation;
  m_fixedVerticalGain = trace->fixedVerticalGain;
  m_bandwidthLimit = trace->bandwidthLimit;
  m_verticalVernier = trace->verticalVernier;
  m_acqVerticalOffset = trace->acqVerticalOffset;
  m_waveSource = trace->waveSource;
  
  // Trigger Time array

  // Check the array size.
  max_index = trace->trigTimeArray.size();
  if(m_trigTimeArray_size < max_index) {
    max_index = m_trigTimeArray_size;
    std::cerr << "Warning: truncating the trig time array to fit into the Root array" << std::endl;
  }
  m_trigTimeArrayCount = max_index;
  

  // Copy the values
  for(index = 0; index < max_index; index++) {
    m_trigTimeArray[index] = trace->trigTimeArray[index];
  }

  // Fill the rest of the array with 0.
  for(; index < m_trigTimeArray_size; index++) {
    m_trigTimeArray[index] = 0;
  }


  // Raw data array.

  // Check the array size.
  max_index = trace->rawDataArray.size();
  if(m_rawDataArray_size < max_index) {
    max_index = m_rawDataArray_size;
    std::cerr << "Warning: truncating the data to fit into the Root array" << std::endl;
  }

  // Copy the values
  for(index = 0; index < max_index; index++) {
    m_rawDataArray[index] = trace->rawDataArray[index];
  }

  // Fill the rest of the array with 0.
  for(; index < m_rawDataArray_size; index++) {
    m_rawDataArray[index] = 0;
  }
  
  // Write the data into the TTree
  m_tree->Fill();
  
  return 0;
}

//--------------------------------------------------------------------

int RootTreeOutput::finalize() {
  std::cout << "finalize" << std::endl;

  m_outputFile->Write();
  m_outputFile->Close();

  m_outputFile = 0; // delete does not work with Root TFile

  return 0;
}

//--------------------------------------------------------------------
