#ifndef game_h
#define game_h

#include "ray.h"
#include "../screen/vscr.h"
#include "../keyboard/vkbd.h"

struct cflugsimu
  {
  cflugsimu (cwelt* pwelt, clkeyboard* plkeyboard, clscreen* plscreen);
  void welttoscreenl ();
  void welttoscreenz ();
  void welttoscreentakt ();
  void welttoscreenthread (integer pthreadnr);
  void fliege ();
  void fliegespiel (cbasis3& spiegelebenen, ckoerper* bewkugel);
  void fliegespieltakt (cbasis3& spiegelebenen, ckoerper* bewkugel);
  void fliegethread ();
  void fliegetakt ();
  void fliege2 ();

  integer  threadanz;           // Anzahl der Threads (von außen setzbar)
  real     framerate;           // Framerate fürs Taktfliegen (von außen setzbar)
  
  private:
  integer      flugsimutps;     // Ticks pro Sekunde
  integer      flugsimudatum;   // gegenwärtiges Zeitsegment in dem sich das Programm befindet
  integer      pixelpos;
 
  cwelt*       welt;
  clkeyboard*  keyboard;
  clscreen*    screen;
  real         xoff, yoff;
  real         bewstep, drehstep;

  struct cpixel
    {
    integer x, y;
    };

  cpixel*     pixels;
  };

#endif
