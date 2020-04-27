#ifndef vkbd_h
#define vkbd_h

struct clkeyboard
  {
  virtual ~clkeyboard ()= 0;
  char getkey (unsigned short px, unsigned short py);
  void putkey (unsigned short px, unsigned short py, char pkey);
  virtual void flush ()= 0;

  char       key[20][6];
  };

#endif
