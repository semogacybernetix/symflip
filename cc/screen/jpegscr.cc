#include "jpegscr.h"

#include <cstdio>
#include <cstring>
#include <csetjmp>
#include <jpeglib.h>  // apt install libjpeg-dev

struct my_error_mgr
  {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
  };

typedef struct my_error_mgr* my_error_ptr;

METHODDEF (void) my_error_exit (j_common_ptr cinfo)
  {
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
  }

cjpegdatei::cjpegdatei (const char* pname)
  : clscreen (0, 0, 255, 255, 255), schreiben (false)
  {
  FILE* infile= fopen (pname, "rb");
  if (infile == NULL)
    {
    printf ("kann Datei nicht oeffnen: \n");
    return;
    }
  jpeg_decompress_struct cinfo;
  my_error_mgr jerr;
  cinfo.err= jpeg_std_error (&jerr.pub);
  jerr.pub.error_exit= my_error_exit;
  if (setjmp (jerr.setjmp_buffer))
    {
    jpeg_destroy_decompress (&cinfo);
    fclose (infile);
    return;
    }
  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, infile);
  jpeg_read_header (&cinfo, 1);
  jpeg_start_decompress (&cinfo);
  xanz= cinfo.output_width;
  yanz= cinfo.output_height;
  signed long rowstride = cinfo.output_width * cinfo.output_components;
  vbild= new unsigned char[rowstride*yanz];
  unsigned char* pufferpos= vbild;
  JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, rowstride, 1);               // blöder Conversion Fehler JDIMENSION aka unsigned int from long int -Wconversion
  while (cinfo.output_scanline < cinfo.output_height)
    {
    jpeg_read_scanlines (&cinfo, buffer, 1);
    memcpy (pufferpos, buffer[0], rowstride);
    pufferpos+= rowstride;
    }
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);
  fclose (infile);
  return;
  }

cjpegdatei::cjpegdatei (const char* pname, integer px, integer py)
  : clscreen (px, py, real (255), real (255), real (255)), schreiben (true)
  {
  name= pname;
  }

cjpegdatei::~cjpegdatei ()
  {
  if (schreiben) flush ();
  delete (vbild);
  }

void cjpegdatei::hintergrund (unsigned char iwert)
  {
  memset (vbild, iwert, xanz*yanz*3);
  }

void cjpegdatei::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px >= xanz) || (py >= yanz))
    return;
  integer pos= py*xanz + px*3;
  vbild[pos]=   (unsigned char) (pb);
  vbild[pos + 1]= (unsigned char) (pg);
  vbild[pos + 2]= (unsigned char) (pr);
  }

void cjpegdatei::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px >= xanz) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  integer pos= ((yanz - py - 1)*xanz + px)*3;
  pr= vbild[pos];
  pg= vbild[pos + 1];
  pb= vbild[pos + 2];
  }

void cjpegdatei::flush ()
  {
  if (!schreiben)
     return;
//  FILE* datei= fopen (name, "wb");
//  jetzt das jpeg-bild erzeugen
//  fclose (datei);
  }
