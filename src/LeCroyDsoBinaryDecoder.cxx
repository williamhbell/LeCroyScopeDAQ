#include "LeCroyDsoBinaryDecoder.h"
#include "LeCroyDsoTimeStamp.h"
#include <cmath>
#include <iostream>

//--------------------------------------------------------------------

std::string LeCroyDsoBinaryDecoder::bytesToString(char *charBuffer, unsigned long offset, unsigned short maxLength) {
  char tmpStr[250];
  bool endOfString = false;
  int i = 0;
  while(!endOfString && i<maxLength && i<250) {
    if((int)charBuffer == 0) {
      endOfString = true;
      tmpStr[i] = '\0';
    }
    else {
      tmpStr[i] = charBuffer[i+offset];
    }
    i++;
  }
  if(i==250) { 
    std::cerr << "Warning string too long for buffer" << std::endl;
  }

  return std::string(tmpStr);
}


//--------------------------------------------------------------------

short LeCroyDsoBinaryDecoder::byteToShort(char *charBuffer, unsigned long offset) {
  unsigned char *byte = (unsigned char *)&charBuffer[offset];
  return (short)(*byte);
}

//--------------------------------------------------------------------

unsigned short LeCroyDsoBinaryDecoder::bytesToEnum(char *charBuffer, unsigned long offset) {
  unsigned short enum_val = 0;

  unsigned char *bytes[2];
  int in_byte, byte, max_byte;

  max_byte = offset + 2; // 2 bytes are used to store enum
  
  // Map the bytes to sort the order in case the lowest byte is
  // first.
  if(m_highestFirst) {
    in_byte = 0;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte++;
    }
  }
  else {
    in_byte = 1;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte--; 
    }
  }
  
  enum_val += (int)(*bytes)[1];
  
  return enum_val;
}

//--------------------------------------------------------------------

int LeCroyDsoBinaryDecoder::bytesToInt(char *charBuffer, unsigned long offset) {
  int val;
  
  unsigned char *bytes[2];
  int in_byte, byte, max_byte;
  max_byte = offset + 2; // 2 bytes are used to store 'word'
  
  // Map the bytes to sort the order in case the lowest byte is
  // first.
  if(m_highestFirst) {
    in_byte = 0;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte++;
    }
  }
  else {
    in_byte = 1;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte--; 
    }
  }
  
  val = 0;
  unsigned short bitOffset = 8;
  for(in_byte = 0; in_byte<2; in_byte++) {
    val += (long)*bytes[in_byte] << bitOffset;
    bitOffset -= 8;
  }

  return val;
}

//--------------------------------------------------------------------

long LeCroyDsoBinaryDecoder::bytesToLong(char *charBuffer, unsigned long offset) {
  long val;

  unsigned char *bytes[4];
  int in_byte, byte, max_byte;
  max_byte = offset + 4; // 4 bytes are used to store long
  
  // Map the bytes to sort the order in case the lowest byte is
  // first.
  if(m_highestFirst) {
    in_byte = 0;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte++;
    }
  }
  else {
    in_byte = 3;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte--; 
    }
  }

  val = 0;
  unsigned short bitOffset = 24;
  for(in_byte = 0; in_byte<4; in_byte++) {
    val += (long)*bytes[in_byte] << bitOffset;
    bitOffset -= 8;
  }

  return val;
}

//--------------------------------------------------------------------

float LeCroyDsoBinaryDecoder::bytesToFloat(char *charBuffer, unsigned long offset) {
  int s, e;
  uint32_t f_bits, mask;
  float f, frac;
  unsigned char *bytes[4];
  int in_byte, byte, max_byte;

  max_byte = offset + 4; // 4 bytes are used to store float

  // Map the bytes to sort the order in case the lowest byte is
  // first.
  if(m_highestFirst) {
    in_byte = 0;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte++;
    }
  }
  else {
    in_byte = 3;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte--; 
    }
  }

  // Sign bit
  s = (*bytes[0] & 128) >> 7;

  // Exponent bits
  // e is made from 8 bits formed from the first 7 bits of the first
  // byte and the highest bit of the second byte.
  e = ((*bytes[0] & 127) << 1) + ((*bytes[1] & 128) >> 7);

  // Fractional bits (the remaining bits)
  f_bits = (uint32_t)(((*bytes[1] & 127) << 16) + (*bytes[2] << 8) + (*bytes[3]));

  // Calculate the float from the bits.
  // The bits are worth 0.5,0.25,0.125...
  mask = 4194304; // 2^22;
  f = 0.0;
  frac = 0.5;
  for(int i = 0; i<23 ; i++) {
    if(f_bits & mask) {
      f += frac;
    }
    frac /= 2.0;
    mask >>= 1;
  }

  return (float)(std::pow(-1.0,s)*std::pow(2.0,e-127)*(1+f));
}

//--------------------------------------------------------------------

double LeCroyDsoBinaryDecoder::bytesToDouble(char *charBuffer, unsigned long offset) {
  int s, e;
  uint32_t d_bytes[2], mask;
  double d, frac;
  unsigned char *bytes[8];
  int in_byte, byte, max_byte, iword, left_shift;

  max_byte = offset + 8; // 8 bytes are used to store float

  // Map the bytes to sort the order in case the lowest byte is
  // first.
  if(m_highestFirst) {
    in_byte = 0;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte++;
    }
  }
  else {
    in_byte = 7;
    for(byte = offset; byte < max_byte; byte++) {
      bytes[in_byte] = (unsigned char *)&charBuffer[byte];
      in_byte--; 
    }
  }

  // Sign bit
  s = (*bytes[0] & 128) >> 7;
  
  // Exponent bits
  // e is made from 8 bits formed from the first 7 bits of the first
  // byte and the highest 4 bits of the second byte.
  // (128, 64, 32, 16 = 240)
  e = ((*bytes[0] & 127) << 4) + ((*bytes[1] & 240) >> 4);
  
  // Fractional bits (the remaining 52 bits)
  // (Four bits from first word: 8, 4, 2, 1 = 15)
  iword = 0; // Most significant word first.
  d_bytes[0] = 0; d_bytes[1] = 0;
  for(in_byte=1;in_byte<=7;in_byte++) {
    if(in_byte == 4) {
      iword = 1;
    }

    left_shift = (7-in_byte)*8 - (1-iword)*32;
    
    if(in_byte == 1) {
      d_bytes[iword] += (*bytes[in_byte] & 15) << left_shift;
    }
    else {
      d_bytes[iword] += (*bytes[in_byte]) << left_shift;
    }
  }

  // Calculate the float from the bits.
  // The bits are worth 0.5,0.25,0.125...
  mask = 524288; // 2^19 (first 20 of 52 bits)
  d = 0.0;
  frac = 0.5;
  iword = 0; // Most significant word first.
  for(int i = 0; i<52 ; i++) {
    if(i == 20) {
      iword = 1;
      mask = (uint32_t)2147483648; // 2^31 (lower 32 of 52 bits)
    }

    if(d_bytes[iword] & mask) {
      d += frac;
    }
    frac /= 2.0;
    mask >>= 1;
  }
  
  return (double)(std::pow(-1.0,s)*std::pow(2.0,e-1023)*(1+d));
}

//--------------------------------------------------------------------

LeCroyDsoTimeStamp LeCroyDsoBinaryDecoder::bytesToTimeStamp(char *charBuffer, unsigned long offset) {
  LeCroyDsoTimeStamp timeStamp;
  timeStamp.seconds = bytesToDouble(charBuffer,offset);
  timeStamp.minutes = byteToShort(charBuffer,offset+8);
  timeStamp.hours = byteToShort(charBuffer,offset+9);
  timeStamp.days = byteToShort(charBuffer,offset+10);
  timeStamp.months = byteToShort(charBuffer,offset+11);
  timeStamp.years = bytesToInt(charBuffer,offset+12);

  return timeStamp;
}

//--------------------------------------------------------------------

std::vector<double> LeCroyDsoBinaryDecoder::readTrigTimeArray(char *charBuffer, unsigned long offset, 
							      long numBytes, 
							      std::vector<double> *triggerOffsetVector) {
  std::vector<double> triggerTimeVector;
  triggerOffsetVector->clear();
  
  long max_byte = offset+numBytes;
  while(offset<max_byte) {
    triggerTimeVector.push_back(bytesToDouble(charBuffer, offset));
    offset+=8;

    if(offset>=max_byte) {
      std::cerr << "Error: TriggerTime is not paired with TriggerOffset!" << std::endl;
      return triggerTimeVector;
    }
   
    triggerOffsetVector->push_back(bytesToDouble(charBuffer, offset));
    offset+=8;
  }

  return triggerTimeVector;
}

//--------------------------------------------------------------------

std::vector<int> LeCroyDsoBinaryDecoder::readDataArray(char *charBuffer, unsigned long offset, 
						       long numPoints) {
  std::vector<int> dataVector;
  long i;

  for(i=0; i<numPoints; i++) {
    if(m_comType == 0) { // byte
      dataVector.push_back(byteToShort(charBuffer, offset));
      offset++;
    }
    else if(m_comType == 1) { // word
      dataVector.push_back(bytesToInt(charBuffer, offset));
      offset+=2;
    }
    else {
      std::cerr << "Communication type " << m_comType << " is out of range" << std::endl;
    }
  }
  
  return dataVector;
}

//--------------------------------------------------------------------
