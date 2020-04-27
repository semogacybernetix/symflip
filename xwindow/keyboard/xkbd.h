#ifndef xkbd_h
#define xkbd_h

#include "../../cc/keyboard/vkbd.h"

struct cxkeyboard : public clkeyboard
  {
  cxkeyboard ();
  virtual ~cxkeyboard ();
  void flush ();
  };

#endif
