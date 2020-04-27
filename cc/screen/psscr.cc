#include "psscr.h"

#include <cstdio>
#include <cstring>

cpsdatei::cpsdatei (const char* pname)
  : clscreen (1, 1, 255, 255, 255), schreiben (false)
  {
  name= pname;
  FILE* datei= fopen (name, "rb");
  if (datei == NULL)
    {
    printf ("Kann Datei nicht öffnen: ");
    printf ("%s", name);
    printf ("\n");
    }
  vbild= new unsigned char[bsize];
  fclose (datei);
  }

cpsdatei::cpsdatei (const char* pname, integer px, integer py)
  : clscreen (px, py, 255, 255, 255), schreiben (true),
    zeile (3*px), bsize (zeile*py)
  {
  name= pname;
  vbild= new unsigned char[bsize];
  }

cpsdatei::~cpsdatei ()
  {
  if (schreiben) flush ();
  delete (vbild);
  }

void cpsdatei::hintergrund (unsigned char iwert)
  {
  memset (vbild, iwert, bsize);
  }

void cpsdatei::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px >= xanz) || (py >= yanz))
    return;
  integer pos (py*zeile + px*3);
  vbild[pos]=   (unsigned char) (pr);
  vbild[pos + 1]= (unsigned char) (pg);
  vbild[pos + 2]= (unsigned char) (pb);
  }

void cpsdatei::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px >= xanz) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  integer pos (py*zeile + px*3);
  pr= (unsigned char) (vbild[pos]);
  pg= (unsigned char) (vbild[pos + 1]);
  pb= (unsigned char) (vbild[pos + 2]);
  }

void cpsdatei::flush ()
  {
  if (!schreiben)
    return;
  FILE* datei= fopen (name, "wb");
  fprintf (datei, "%%! generated from psscr.cc\n");
  fprintf (datei, "\n");
  fprintf (datei, "%%%%SOFHA-TitleEx: <");
  char zeichen[3];
  zeichen[2]= 0;
  integer nlauf= 0;
  while (name[nlauf] > 0)
    {
    if (nlauf > 0)
      fprintf (datei, " ");
    gethexstring (zeichen, name[nlauf]);
    fprintf (datei, "%s", zeichen);
    nlauf++;
    }
  fprintf (datei, ">\n\n");
  real mm= real (360)/127;
  real psx= powl (2, -2.25)*1000*mm - 10*mm;
  real psy= powl (2, -1.75)*1000*mm - 10*mm;
  real ratio= real (yanz)/real (xanz);
  real transx, transy, scalex, scaley;
  if (ratio < psy/psx)
    {
 scalex= psx;
 scaley= psx*ratio;
 transx= 5*mm;
 transy= 5*mm + (psy - scaley)/2;
 }
 else
 {
 scalex= psy/ratio;
 scaley= psy;
 transx= 5*mm + (psx - scalex)/2;
 transy= 5*mm;
 }

  fprintf (datei, "%Lf %Lf translate\n", transx, transy);
  fprintf (datei, "%Lf %Lf scale\n", scalex, scaley);
  fprintf (datei, "/DeviceCMYK setcolorspace\n");
  fprintf (datei, "\n");
  fprintf (datei, "<<\n");
  fprintf (datei, "  /ImageType 1\n");
  fprintf (datei, "  /Width %lld\n", xanz);
  fprintf (datei, "  /Height %lld\n", yanz);
  fprintf (datei, "  /BitsPerComponent 8\n");
  fprintf (datei, "  /Decode [0 1 0 1 0 1 0 1]\n");
  fprintf (datei, "  /ImageMatrix [%lld 0 0 %lld 0 0]\n", xanz, yanz);
  fprintf (datei, "  /DataSource currentfile /ASCIIHexDecode filter\n");
  fprintf (datei, ">>\n");
  fprintf (datei, "image\n");
  fprintf (datei, "\n");
  for (integer laufb= 0; laufb < bsize; laufb+= 3)
    {
    unsigned char min= vbild[laufb];
    if (vbild[laufb + 1] > min)
      min= vbild[laufb + 1];
    if (vbild[laufb + 2] > min)
      min= vbild[laufb + 2];
    gethexstring (zeichen, (unsigned char) (min - vbild[laufb]));
    fwrite (zeichen, 2, 1, datei);
    gethexstring (zeichen, (unsigned char) (min - vbild[laufb + 1]));
    fwrite (zeichen, 2, 1, datei);
    gethexstring (zeichen, (unsigned char) (min - vbild[laufb + 2]));
    fwrite (zeichen, 2, 1, datei);
    gethexstring (zeichen, (unsigned char) (255 - min));
    fwrite (zeichen, 2, 1, datei);
    if ((laufb + 3)%24)
      fprintf (datei, " ");
      else
      fprintf (datei, "\n");
    }
  fprintf (datei, "\n");
  fprintf (datei, ">\n");
  fprintf (datei, "\n");
  fprintf (datei, "showpage\n");
  }

void cpsdatei::gethexstring (char* ochar, unsigned char iwert)
  {
  unsigned char st1= iwert/16;
  unsigned char st2= iwert%16;
  if (st1 < 10)
    ochar[0]= char (0x30 + st1);
    else
    ochar[0]= char (0x37 + st1);
  if (st2 < 10)
    ochar[1]= char (0x30 + st2);
    else
    ochar[1]= char (0x37 + st2);
  }
