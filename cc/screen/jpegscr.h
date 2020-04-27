#ifndef jpegscr_h
#define jpegscr_h

#include "vscr.h"

struct cjpegdatei : public clscreen
  {
  cjpegdatei (const char* pname);
  cjpegdatei (const char* pname, integer px, integer py);
  virtual ~cjpegdatei ();
  void putpixel (integer px, integer py, real pr, real pg, real pb);
  void getpixel (integer px, integer py, real &pr, real &pg, real &pb);
  void hintergrund (unsigned char iwert);
  void putscreen (clscreen& pscreen, integer px, integer py);
  void flush ();

  private:
  const char*          name;
  bool                 schreiben;
  unsigned char*       vbild;
  integer              bsize;
  };

#endif
