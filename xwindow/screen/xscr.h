#ifndef xscr_h
#define xscr_h

#include "../../cc/screen/vscr.h"

struct cxscreen : public clscreen
  {
  cxscreen (const char* pname, integer px, integer py);
  virtual ~cxscreen ();
  void putpixel (integer px, integer py, real pr, real pg, real pb);
  void getpixel (integer px, integer py, real &pr, real &pg, real &pb);
  void flush ();

  void putscreen (clscreen &pscreen, integer px, integer py);
  void getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer &xmin, integer &ymin, integer &xmax, integer &ymax);
  bool checkexpose ();

  public:
  integer        rshift, gshift, bshift;
  integer        rdepth, gdepth, bdepth;
  integer        ypix1;
  };

struct cmemoryscreen : public clscreen
  {
  cmemoryscreen (const char* pname, integer px, integer py);
  virtual ~cmemoryscreen ();
  void putpixel (integer px, integer py, real pr, real pg, real pb);
  void putpixelx (integer px, integer py, real pr, real pg, real pb);
  void getpixel (integer px, integer py, real &pr, real &pg, real &pb);
  void flush ();

  void putscreen (clscreen &pscreen, integer px, integer py);
  void getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer &xmin, integer &ymin, integer &xmax, integer &ymax);
  bool checkexpose ();

  public:
  integer        rshift, gshift, bshift;
  integer        rdepth, gdepth, bdepth;
  integer        ypix1;
  
  integer*       bildspeicher;
  };

struct cximagescreen : public clscreen
  {
  cximagescreen (const char* pname, integer px, integer py);
  virtual ~cximagescreen ();
  void putpixel (integer px, integer py, real pr, real pg, real pb);
  void getpixel (integer px, integer py, real &pr, real &pg, real &pb);
  void flush ();

  void putscreen (clscreen& pscreen, integer px, integer py);
  void getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer &xmin, integer &ymin, integer &xmax, integer &ymax);
  bool checkexpose ();

  public:
  integer        rshift, gshift, bshift;
  integer        rdepth, gdepth, bdepth;
  integer        ypix1;

  private:
  char*          bild;
  };

#endif
