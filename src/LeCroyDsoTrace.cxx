#include "LeCroyDsoTrace.h"
#include "LeCroyDsoTimeStamp.h"

LeCroyDsoTrace::LeCroyDsoTrace(): triggerNumber(0),
                                  commType(0),
				  commOrder(0),
				  waveDescriptorLength(0),
				  userTextLength(0),
				  resDesc1(0),
				  triggerTimeArrayLength(0),
				  riseTimeArrayLength(0),
				  resArray1Length(0),
				  waveArray1Length(0),
				  waveArray2Length(0),
				  resArray2Length(0),
				  resArray3Length(0),
				  instrumentNumber(0),
				  reserved1(0),
				  reserved2(0),
				  waveArrayCount(0),
				  pointsPerScreen(0),
				  firstValidPoint(0),
				  lastValidPoint(0),
				  firstPoint(0),
				  sparsingFactor(0),
				  segmentIndex(0),
				  subArrayCount(0),
				  sweepsPerAcq(0),
				  pointsPerPair(0),
				  pairOffset(0),
				  verticalGain(0),
				  verticalOffset(0),
				  maxValue(0),
				  minValue(0),
				  nominalBits(0),
				  nominalSubArrayCount(0),
				  horizontalInterval(0),
				  horizontalOffset(0),
				  pixelOffset(0),
				  horizontalUncertainty(0),
				  triggerTime(),
				  acquisitionDuration(0),
				  recordType(0),
				  processingDone(0),
				  reserved5(0),
				  risSweeps(0),
				  timeBase(0),
				  verticalCoupling(0),
				  probeAttenuation(0),
				  fixedVerticalGain(0),
				  bandwidthLimit(0),
				  verticalVernier(0),
				  acqVerticalOffset(0),
				  waveSource(0),
				  trigTimeArray(),
				  trigOffsetArray(),
				  rawDataArray() { 
}

void LeCroyDsoTrace::printValues(void) {
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "LeCroyDso Trace" << std::endl;
    std::cout << "\t Trigger Number = " << triggerNumber << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "DESCRIPTOR_NAME = " << descriptorName <<std::endl;
    std::cout << "TEMPLATE_NAME = " << templateName << std::endl;
    std::cout << "COMM_TYPE = " << commType << std::endl;
    std::cout << "COMM_ORDER = " << commOrder << std::endl;
    std::cout << "WAVE_DESCRIPTOR = " << waveDescriptorLength << std::endl;
    std::cout << "USER_TEXT = " << userTextLength << std::endl;
    std::cout << "RES_DESC1 = " << resDesc1 << std::endl;
    std::cout << "TRIGTIME_ARRAY = " << triggerTimeArrayLength << std::endl;
    std::cout << "RIS_TIME_ARRAY = " << riseTimeArrayLength << std::endl;
    std::cout << "RES_ARRAY1 = " << resArray1Length << std::endl;
    std::cout << "WAVE_ARRAY_1 = " << waveArray1Length << std::endl;
    std::cout << "WAVE_ARRAY_2 = " << waveArray2Length << std::endl;
    std::cout << "RES_ARRAY2 = " << resArray2Length << std::endl;
    std::cout << "RES_ARRAY3 = " << resArray3Length << std::endl;
    std::cout << "INSTRUMENT_NAME = " << instrumentName << std::endl;
    std::cout << "INSTRUMENT_NUMBER = " << instrumentNumber << std::endl;
    std::cout << "TRACE_LABEL = " << traceLabel << std::endl;
    std::cout << "RESERVED1 = " << reserved1 << std::endl;
    std::cout << "RESERVED2 = " << reserved2 << std::endl;
    std::cout << "WAVE_ARRAY_COUNT = " << waveArrayCount << std::endl;
    std::cout << "PNTS_PER_SCREEN = " << pointsPerScreen << std::endl;
    std::cout << "FIRST_VALID_PNT = " << firstValidPoint << std::endl;
    std::cout << "LAST_VALID_PNT = " << lastValidPoint << std::endl;
    std::cout << "FIRST_POINT = " << firstPoint << std::endl;
    std::cout << "SPARSING_FACTOR = " << sparsingFactor << std::endl;
    std::cout << "SEGMENT_INDEX = " << segmentIndex << std::endl;
    std::cout << "SUBARRAY_COUNT = " << subArrayCount << std::endl;
    std::cout << "SWEEPS_PER_ACQ = " << sweepsPerAcq << std::endl;
    std::cout << "POINTS_PER_PAIR = " << pointsPerPair << std::endl;
    std::cout << "PAIR_OFFSET = " << pairOffset << std::endl;
    std::cout << "VERTICAL_GAIN = " << verticalGain << std::endl;
    std::cout << "VERTICAL_OFFSET = " << verticalOffset << std::endl;
    std::cout << "MAX_VALUE = " << maxValue << std::endl;
    std::cout << "MIN_VALUE = " << minValue << std::endl;
    std::cout << "NOMINAL_BITS = " << nominalBits << std::endl;
    std::cout << "NOM_SUBARRAY_COUNT = " << nominalSubArrayCount << std::endl;
    std::cout << "HORIZ_INTERVAL = " << horizontalInterval << std::endl;
    std::cout << "HORIZ_OFFSET = " << horizontalOffset << std::endl;
    std::cout << "PIXEL_OFFSET = " << pixelOffset << std::endl;
    std::cout << "VERTUNIT = " << verticalUnit << std::endl;
    std::cout << "HORUNIT = " << horizontalUnit << std::endl;
    std::cout << "HORIZ_UNCERTAINTY = " << horizontalUncertainty << std::endl;
    std::cout << "TRIGGER_TIME = " << triggerTime << std::endl;
    std::cout << "ACQ_DURATION = " << acquisitionDuration << std::endl;
    std::cout << "RECORD_TYPE = " << recordType << std::endl;
    std::cout << "PROCESSING_DONE = " << processingDone << std::endl;
    std::cout << "RESERVED5 = " << reserved5 << std::endl;
    std::cout << "RIS_SWEEPS = " << risSweeps << std::endl;
    std::cout << "TIMEBASE = " << timeBase << std::endl;
    std::cout << "VERT_COUPLING = " << verticalCoupling << std::endl;
    std::cout << "PROBE_ATT = " << probeAttenuation << std::endl;
    std::cout << "FIXED_VERT_GAIN = " << fixedVerticalGain << std::endl;
    std::cout << "BANDWIDTH_LIMIT = " << bandwidthLimit << std::endl;
    std::cout << "VERTICAL_VERNIER = " << verticalVernier << std::endl;
    std::cout << "ACQ_VERT_OFFSET = " << acqVerticalOffset << std::endl;
    std::cout << "WAVE_SOURCE = " << waveSource << std::endl;
    std::cout << std::endl;
    
    long ipoint, imax_point;

    // Only present if sequence is ON.
    if(trigTimeArray.size() > 0) {
      std::cout << "--------------------------------------" << std::endl;
      std::cout << "|       TRIGTIME ARRAY" << std::endl;
      std::cout << "--------------------------------------" << std::endl;
      std::cout << "|    TIME    |    OFFSET  "  << std::endl;
      std::cout << "--------------------------------------" << std::endl;
      imax_point = trigTimeArray.size();
      if(imax_point == trigOffsetArray.size()) {
	for(ipoint=0; ipoint<imax_point; ipoint++) {
	  std::cout << "| ";
	  std::cout.fill(' ');
	  std::cout.width(10);
	  std::cout << trigTimeArray[ipoint] << " | " << trigOffsetArray[ipoint]  << std::endl;
	}
      }
      else {
	for(ipoint=0; ipoint<imax_point; ipoint++) {
	  std::cout << "| ";
	  std::cout.fill(' ');
	  std::cout.width(10);
	  std::cout << trigTimeArray[ipoint] << " | " << "ERROR"  << std::endl;
	}
      }
      std::cout << "--------------------------------------" << std::endl;
      std::cout << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "|       DATA_ARRAY_1" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "|    ADC     |    " <<  verticalUnit << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    imax_point = rawDataArray.size();
    if(rawDataArray.size() == waveArray.size()) {
      for(ipoint=0; ipoint<imax_point; ipoint++) {
	std::cout << "| ";
	std::cout.fill(' ');
	std::cout.width(10);
	std::cout << rawDataArray[ipoint] << " | " << waveArray[ipoint]  << std::endl;
      }
    }
    else {
      for(ipoint=0; ipoint<imax_point; ipoint++) {
	std::cout << "| ";
	std::cout.fill(' ');
	std::cout.width(10);
	std::cout << rawDataArray[ipoint] << " | " << "ERROR"  << std::endl;
      }
    }
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
}
