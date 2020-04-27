#ifndef vscr_h
#define vscr_h

#include "../class/vektor.h"

struct clscreen
  {
  clscreen ();
  clscreen (integer px, integer py, real pr, real pg, real pb);

  virtual void getpixel (integer px, integer py, real &pr, real &pg, real &pb)= 0;
  virtual void putpixel (integer px, integer py, real pr, real pg, real pb)= 0;
  void putscreen (clscreen& pscreen, integer px, integer py);
  virtual void flush ()= 0;

  integer xanz, yanz;
  real rmax, gmax, bmax;
  };

#endif
