#include <iostream>
#include <bitset>
#include "LeCroyDsoBinaryDecoder.h"

int main() {
  char bytes[8];
  bytes[0] = (char)0xFE;
  bytes[1] = (char)0xDC;
  bytes[2] = (char)0xBA;
  bytes[3] = (char)0x98;
  bytes[4] = (char)0x76;
  bytes[5] = (char)0x54;
  bytes[6] = (char)0x32;
  bytes[7] = (char)0x10;

  std::bitset<8> bits(bytes[0]);
  std::cout << bits << std::endl;

  LeCroyDsoBinaryDecoder decoder;
  decoder.highestFirst(true);

  std::cout << decoder.bytesToDouble(bytes, 0) << std::endl;

  return 0;
}
