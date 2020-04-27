#include "bmpscr.h"

#include <cstdio>
#include <cstring>

cbmpdatei::cbmpdatei (const char* pname)
  {
  rmax= gmax= bmax= 255;
  schreiben= false;
  name= pname;
  FILE* datei= fopen (name, "rb");
  if (datei == NULL)
    {
    printf ("File open error: ");
    printf ("\n");
    }
  signed long ret= fread (head, 0x36, 1, datei);
  xanz= ((signed long) (head[0x13]) << 8) | (signed long) (head[0x12]);
  yanz= ((signed long) (head[0x17]) << 8) | (signed long) (head[0x16]);
  zeile= 3*xanz + (xanz & 3);
  bsize= zeile*yanz;
  vbild= new unsigned char[bsize];
  ret= fread (vbild, bsize, 1, datei);
  fclose (datei);
  ret*= 1;  // damit der doofe Compiler nicht meckert, dass ich die Variable nicht benutzen würde
  // erst meckert er mich an, dass ich einen Rückgabewert ignoriere, den ich nicht brauche (Vorwurf: Sie ignorieren, dass in Neuseeland jemand einen Pups
  // gelassen hat)
  // dann meckert er mich an, dass ich den nicht ignorierten Rückgabewert nicht verwende, obwohl ich keine Verwendung für ihn habe
  // dann kann ich dem Compiler ja auch vorwerfen, dass er ignoriert, dass ich heute Geburtstag habe
  // Fazit: es ist Schwachsinn, jemandem vorzuwerfen etwas zu ignorieren, wenn er dafür keine Verwendung hat
  }

cbmpdatei::cbmpdatei (const char* pname, integer px, integer py)
  {
  xanz= px;
  yanz= py;
  rmax= gmax= bmax= 255;
  schreiben= true;
  zeile= 3*px + (px & 3);
  bsize= zeile*py;
  name= pname;
  vbild= new unsigned char[bsize];
  memset (head, 0, 0x36);
  signed long fsize (0x36 + bsize);
  head[0]= 'B';
  head[1]= 'M';
  head[2]= (unsigned char) (fsize);
  head[3]= (unsigned char) (fsize >> 8);
  head[4]= (unsigned char) (fsize >> 16);
  head[5]= (unsigned char) (fsize >> 24);
  head[0xa]= 0x36;
  head[0xe]= 0x28;
  head[0x12]= (unsigned char) (xanz);
  head[0x13]= (unsigned char) (xanz/256);
  head[0x16]= (unsigned char) (yanz);
  head[0x17]= (unsigned char) (yanz/256);
  head[0x1a]= 1;
  head[0x1c]= 24;
  head[0x1e]= 0;
  head[0x22]= (unsigned char) (bsize);
  head[0x23]= (unsigned char) (bsize >> 8);
  head[0x24]= (unsigned char) (bsize >> 16);
  head[0x25]= (unsigned char) (bsize >> 24);
  head[0x26]= 0x12;
  head[0x27]= 0xb;
  head[0x2a]= 0x12;
  head[0x2b]= 0xb;
  memset (vbild, 0, bsize);
  }

cbmpdatei::~cbmpdatei ()
  {
  flush ();
  delete (vbild);
  }

void cbmpdatei::flush ()
  {
  if (!schreiben)
    return;
  FILE* datei= fopen (name, "wb");
  fwrite (head, 0x36, 1, datei);
  fwrite (vbild, bsize, 1, datei);
  fclose (datei);
  }

void cbmpdatei::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    return;
  integer pos= py*zeile + px*3;
  vbild[pos]=     (unsigned char) (pb);
  vbild[pos + 1]= (unsigned char) (pg);
  vbild[pos + 2]= (unsigned char) (pr);
  }

void cbmpdatei::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  integer pos= py*zeile + px*3;
  pb= vbild[pos];
  pg= vbild[pos + 1];
  pr= vbild[pos + 2];
  }
