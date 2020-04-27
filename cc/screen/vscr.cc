#include "vscr.h"

clscreen::clscreen ()
  : xanz (0), yanz (0), rmax (0), gmax (0), bmax (0)
  {
  }


clscreen::clscreen (integer px, integer py, real pr, real pg, real pb)
  : xanz (px), yanz (py), rmax (pr), gmax (pg), bmax (pb)
  {
  }

void clscreen::putscreen (clscreen& pscreen, integer px, integer py)
  {
  real r, g, b;
  for (integer laufy= 0; laufy < pscreen.yanz; laufy++)
    for (integer laufx= 0; laufx < pscreen.xanz; laufx++)
      {
      getpixel (laufx, laufy, r, g, b);
      putpixel (px + laufx, py + laufy, r, g, b);
      }
  }
