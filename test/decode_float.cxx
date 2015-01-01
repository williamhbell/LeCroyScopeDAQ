#include <iostream>
#include <bitset>
#include <cmath>
#include "LeCroyDsoBinaryDecoder.h"

int main() {
  char bytes[4];
  bytes[0] = (char)0x34;
  bytes[1] = (char)0x83;
  bytes[2] = (char)0x12;
  bytes[3] = (char)0x6F;

  std::bitset<8> bits(bytes[0]);
  std::cout << bits << std::endl;

  LeCroyDsoBinaryDecoder decoder;
  decoder.highestFirst(true);

  std::cout << decoder.bytesToFloat(bytes, 0) << std::endl;

  return 0;
}
