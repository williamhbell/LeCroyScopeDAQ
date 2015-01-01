#include "LeCroyTraceDecoder.h"

#include "ApplicationBase.h"

#include "LeCroyDsoBinaryDecoder.h"
#include "LeCroyDsoTrace.h"
#include "IOmethods.h"

#include <iostream>
#include <sstream>

LeCroyTraceDecoder::LeCroyTraceDecoder(ApplicationBase *appBase): m_appBase(appBase),
								  m_debug(false) {
  m_debug = m_appBase->isDebug();
  m_binaryDecoder = new LeCroyDsoBinaryDecoder();
}

//--------------------------------------------------------------------

LeCroyTraceDecoder::~LeCroyTraceDecoder() {
  delete m_binaryDecoder;
}

//--------------------------------------------------------------------


long LeCroyTraceDecoder::decodeHeader(char *charBuffer, 
				      int *firstBytePos, 
				      long charBufferSize) {
  int i,j;
  bool found;
  char tmpstr[20];

  // Find the beginning of the header
  i=0;
  found = false;
  while(!found && i<charBufferSize) {
    if(charBuffer[i] == '#') {
      found = true;
    }
    i++;
  }
  if(i >= charBufferSize) {
    std::cerr << "Error: beginning of trace header could not be found." << std::endl;
    return 0;
  }
  
  // Find the beginning of the trace "WAVEDESC..." and read the
  // number of bytes that will follow.   
  i++; // Skip the number of ascii numbers (normally 9)
  found = false;
  j=0;
  while(!found && i<charBufferSize && j<20) {
    if(charBuffer[i] == 'W') {
      found = true;
    }
    else {
      tmpstr[j] = charBuffer[i];
      j++;
      i++;
    }
  }
  if(i >= charBufferSize) {
    std::cerr << "Error: beginning of trace could not be found." << std::endl;
    return 0;
  }
  if(j >= 20) {
    std::cerr << "Error: buffer used for header is too short." << std::endl;
    return 0;
  }

  *firstBytePos = i; // Index of 'W'
  
  // Append string terminator to allow cstring to be parsed.
  tmpstr[j] = '\0';
  return IOmethods::atol(std::string(tmpstr));
}

//--------------------------------------------------------------------

// This function follows the waveform template.

LeCroyDsoTrace* LeCroyTraceDecoder::decodeTrace(char *charBuffer, 
						long charBufferSize,
						int first_byte,
						long number_of_bytes) {
  long offset;

  // Create an empty trace
  LeCroyDsoTrace *trace = new LeCroyDsoTrace();

  if(m_debug) {
    std::cout << "Length of data stream =" << number_of_bytes << std::endl;
  }

  offset = 0;
  trace->descriptorName = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 16);

  offset = 16;
  trace->templateName = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 16);

  offset = 32;
  trace->commType = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);

  offset = 34;
  trace->commOrder = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);
  if(trace->commOrder == 0) { 
    m_binaryDecoder->highestFirst(true);
  }
  else {
    m_binaryDecoder->highestFirst(false);
  } 
  
  offset = 36;
  trace->waveDescriptorLength = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 40;
  trace->userTextLength = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 44;
  trace->resDesc1 = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 48;
  trace->triggerTimeArrayLength = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);
  
  offset = 52;
  trace->riseTimeArrayLength = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);
  
  offset = 56;
  trace->resArray1Length = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 60;
  trace->waveArray1Length = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 64;
  trace->waveArray2Length = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 68;
  trace->resArray2Length = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 72;
  trace->resArray3Length = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 76;
  trace->instrumentName = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 16);
  
  offset = 92;
  trace->instrumentNumber = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 96;
  trace->traceLabel = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 16);

  offset = 112;
  trace->reserved1 = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 114;
  trace->reserved2 = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 116;
  trace->waveArrayCount = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);
  
  offset = 120;
  trace->pointsPerScreen = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 124;
  trace->firstValidPoint = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 128;
  trace->lastValidPoint = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 132;
  trace->firstPoint = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 136;
  trace->sparsingFactor = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 140;
  trace->segmentIndex = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 144;
  trace->subArrayCount = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 148;
  trace->sweepsPerAcq = m_binaryDecoder->bytesToLong(charBuffer, first_byte+offset);

  offset = 152;
  trace->pointsPerPair = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 154;
  trace->pairOffset = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 156;
  trace->verticalGain = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 160;
  trace->verticalOffset = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 164;
  trace->maxValue = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);
  
  offset = 168;
  trace->minValue = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);
  
  offset = 172;
  trace->nominalBits = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 174;
  trace->nominalSubArrayCount = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 176;
  trace->horizontalInterval = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 180;
  trace->horizontalOffset = m_binaryDecoder->bytesToDouble(charBuffer, first_byte+offset);
  
  offset = 188;
  trace->pixelOffset = m_binaryDecoder->bytesToDouble(charBuffer, first_byte+offset);

  offset = 196;
  trace->verticalUnit = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 48);

  offset = 244;
  trace->horizontalUnit = m_binaryDecoder->bytesToString(charBuffer, first_byte+offset, 48);
  
  offset = 292;
  trace->horizontalUncertainty = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 296;
  trace->triggerTime = m_binaryDecoder->bytesToTimeStamp(charBuffer, first_byte+offset);

  offset = 312;
  trace->acquisitionDuration = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 316;
  trace->recordType = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);

  offset = 318;
  trace->processingDone = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);

  offset = 320;
  trace->reserved5 = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 322;
  trace->risSweeps = m_binaryDecoder->bytesToInt(charBuffer, first_byte+offset);

  offset = 324;
  trace->timeBase = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);

  offset = 326;
  trace->verticalCoupling = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset);

  offset = 328;
  trace->probeAttenuation = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 332;
  trace->fixedVerticalGain = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset); 

  offset = 334;
  trace->bandwidthLimit = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset); 

  offset = 336;
  trace->verticalVernier = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 340;
  trace->acqVerticalOffset = m_binaryDecoder->bytesToFloat(charBuffer, first_byte+offset);

  offset = 344;
  trace->waveSource = m_binaryDecoder->bytesToEnum(charBuffer, first_byte+offset); 

  // Only present in sequence mode
  if(trace->triggerTimeArrayLength>0) {
    offset = first_byte + trace->waveDescriptorLength +  trace->userTextLength;
    trace->trigTimeArray = m_binaryDecoder->readTrigTimeArray(charBuffer, offset, trace->triggerTimeArrayLength, &(trace->trigOffsetArray));
  }

  // Offset to DATA_ARRAY_1
  offset = first_byte + trace->waveDescriptorLength +  trace->userTextLength + trace->triggerTimeArrayLength + trace->riseTimeArrayLength;
  
  trace->rawDataArray = m_binaryDecoder->readDataArray(charBuffer, offset, trace->waveArrayCount);
  
  // Convert ADC values to vertical unit (normally Volts)
  adcToVoltage(trace);

  if(m_debug) {
    trace->printValues();
  }
  
  return trace;
}

//-----------------------------------------------------------------

int LeCroyTraceDecoder::adcToVoltage(LeCroyDsoTrace *trace) {
  if(!trace) {
    std::cerr << "Error: LeCroyDsoTrace pointer is null" << std::endl;
    return 1;
  }

  long ipoint, imax_point;

  // Initialise the array to be the same size as the input data array.
  trace->waveArray.clear();
  imax_point = trace->rawDataArray.size();
  trace->waveArray.resize(imax_point);

  // Loop over all element and calculate the value in volts.
  for(ipoint=0; ipoint<imax_point; ipoint++) {
    trace->waveArray[ipoint] = trace->verticalGain * trace->rawDataArray[ipoint] - trace->verticalOffset;
  }
  
  return 0;
}

//-----------------------------------------------------------------
