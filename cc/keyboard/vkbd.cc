#include "vkbd.h"

clkeyboard::~clkeyboard () {}

char clkeyboard::getkey (unsigned short px, unsigned short py)
  {
  return key[px][py];
  }

void clkeyboard::putkey (unsigned short px, unsigned short py, char pkey)
  {
  key[px][py]= pkey;
  }
