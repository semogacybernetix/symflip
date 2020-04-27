#ifndef psscr_h
#define psscr_h

#include "vscr.h"

struct cpsdatei : public clscreen
  {
  cpsdatei (const char* pname);
  cpsdatei (const char* pname, integer px, integer py);
  virtual ~cpsdatei ();
  void getpixel (integer px, integer py, real &pr, real &pg, real &pb);
  void putpixel (integer px, integer py, real pr, real pg, real pb);
  void hintergrund (unsigned char iwert);
  void flush ();
  void gethexstring (char* ochar, unsigned char iwert);

  private:
  const char*          name;
  bool                 schreiben;

  public:
  unsigned char*       vbild;
  integer              zeile;

  private:
  integer              bsize;
  };

#endif
