#include "xscr.h"

#include <X11/Xutil.h>       // XdestroyImage
#include <iostream>          // cout, printf, scanf
#include <unistd.h>          // usleep

using namespace std;

Display*              mydisplay;
Window                mywindow;
GC                    mygc;
XEvent                report;
XImage*               xbild;

// --------------------------------------------------------- cxscreen ------------------------------------------------------

cxscreen::cxscreen (const char* pname, integer px, integer py)
  {
  ypix1= (py - 1);
  xanz= px;
  yanz= py;
  mydisplay= XOpenDisplay ("");
  integer ftiefe= XDefaultDepth (mydisplay, 0);
  cout << "Pixeltiefe: " << ftiefe << endl;
  Visual* myvisual= XDefaultVisual (mydisplay, 0);
  integer redmask= myvisual->red_mask;
  rshift= 0;
  while (!(redmask & 1) && rshift < 128)
    {
    rshift++;
    redmask >>= 1;
    }
  rdepth= 0;
  while (redmask & 1)
    {
    rdepth++;
    redmask >>= 1;
    }
  integer greenmask= myvisual->green_mask;
  gshift= 0;
  while (!(greenmask & 1) && gshift < 128)
    {
    gshift++;
    greenmask >>= 1;
    }
  gdepth= 0;
  while (greenmask & 1)
    {
    gdepth++;
    greenmask >>= 1;
    }
  integer bluemask= myvisual->blue_mask;
  bshift= 0;
  while (!(bluemask & 1) && bshift < 128)
    {
    bshift++;
    bluemask >>= 1;
    }
  bdepth= 0;
  while (bluemask & 1)
    {
    bdepth++;
    bluemask >>= 1;
    }
//  cout << " r: " << rshift << " g: " << gshift << " b: " << bshift << " ftiefe: " << ftiefe << endl;
  cout << "Bits per rgb: " << myvisual->bits_per_rgb << endl;
  switch (ftiefe)
    {
    case 4:
      {
      rmax= bmax= 1; gmax= 3; rshift= 3; gshift= 1; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[16];
      for (integer cn= 0; cn < 16; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 65535*(cn >> 3);
        defs[cn].green= 21845*((cn >> 1) & 3);
        defs[cn].blue= 65535*(cn & 1);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 16);
      integer mymask= CWColormap | CWBackPixel | CWEventMask;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 8:
      {
      rmax= gmax= 7; bmax= 3; rshift= 5; gshift= 2; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[256];
      for (integer cn= 0; cn < 256; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 9362*(cn >> 5);
        defs[cn].green= 9362*((cn >> 2) & 7);
        defs[cn].blue= 21845*(cn & 3);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 256);
      integer mymask= CWColormap | CWBackPixel | CWEventMask | CWBackingStore;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      myattr.backing_store= Always;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 15: case 16: case 24:
      {
      rmax= (1 << rdepth) - 1;
      gmax= (1 << gdepth) - 1;
      bmax= (1 << bdepth) - 1;
//      cout << " rmax: " << rmax << " gmax: " << gmax << " bmax: " << bmax << endl;
      integer mymask= CWBackPixel | CWEventMask | CWBackingStore | CWBackingPlanes;
      XSetWindowAttributes myattr;
      myattr.background_pixel= 0;
      myattr.backing_store= Always;
      myattr.backing_planes= 24;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    }
  mygc= XCreateGC (mydisplay, mywindow, 0, 0);
  XStoreName (mydisplay, mywindow, pname);
  XMapRaised (mydisplay, mywindow);
  XFlush (mydisplay);
  usleep (300000);
  }

cxscreen::~cxscreen ()
  {
  XCloseDisplay (mydisplay);
  }

void cxscreen::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    return;
  integer pri (pr);
  integer pgi (pg);
  integer pbi (pb);
  XSetForeground (mydisplay, mygc, (pri << rshift) | (pgi << gshift) | (pbi << bshift));
  XDrawPoint (mydisplay, mywindow, mygc, px, ypix1 - py);
  }

void cxscreen::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  pr= pg= pb= 0;
  }

void cxscreen::flush ()
  {
  XFlush (mydisplay);
  }

void cxscreen::putscreen (clscreen& pscreen, integer px, integer py)
  {
  if ((px >= xanz) | (py >= yanz))
      return;
  real rskala= real (rmax)/pscreen.rmax;
  real gskala= real (gmax)/pscreen.gmax;
  real bskala= real (bmax)/pscreen.bmax;
  integer clipx, clipy;
  if (xanz < px + pscreen.xanz)
      clipx= xanz - px - 1;
    else
      clipx= pscreen.xanz;
  if (yanz < pscreen.yanz)
      clipy= yanz - py - 1;
    else
      clipy= pscreen.yanz;
  integer ftiefe= rdepth + gdepth + bdepth;
  if (ftiefe == 16)
    {
    integer* simage= new integer[clipx*clipy];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]=  (pred << rshift) | (pgreen << gshift) | (pblue << bshift);
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, ftiefe, ZPixmap, 0, (char*) simage, clipx, clipy, 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, px, ypix1 - clipy1 - py, clipx, clipy);
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  if (ftiefe == 24)
    {
    signed char* simage= new signed char[clipx*clipy*4];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]= pblue;
        l_addr++;
        simage[l_addr]= pgreen;
        l_addr++;
        simage[l_addr]= pred;
        l_addr++;
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, ftiefe, ZPixmap, 0, (char*) simage, clipx, clipy, 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, px, ypix1 - clipy1 - py, clipx, clipy);
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  }

void cxscreen::getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer& xmin, integer& ymin, integer& xmax, integer& ymax)
  {
  pexp= 0;
  XNextEvent (mydisplay, &report);
  if (report.type == ButtonPress)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= 1;
    }
  if (report.type == ButtonRelease)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= -1;
    }
  if (report.type == Expose)
    {
    xmin= report.xexpose.x;
    xmax= report.xexpose.x + report.xexpose.width;
    ymin= report.xexpose.y;
    ymax= report.xexpose.y + report.xexpose.height;
    pexp= 1;
    }
  }

bool cxscreen::checkexpose ()
  {
  return XCheckMaskEvent (mydisplay, ExposureMask, &report);
  }

// --------------------------------------------------------- cmemoryxscreen ------------------------------------------------------

cmemoryscreen::cmemoryscreen (const char* pname, integer px, integer py)
  {
  ypix1= (py - 1);
  xanz= px;
  yanz= py;
  mydisplay= XOpenDisplay ("");
  integer ftiefe= XDefaultDepth (mydisplay, 0);
  cout << "Pixeltiefe: " << ftiefe << endl;
  Visual* myvisual= XDefaultVisual (mydisplay, 0);
  integer redmask= myvisual->red_mask;
  rshift= 0;
  while (!(redmask & 1) && rshift < 128)
    {
    rshift++;
    redmask >>= 1;
    }
  rdepth= 0;
  while (redmask & 1)
    {
    rdepth++;
    redmask >>= 1;
    }
  integer greenmask= myvisual->green_mask;
  gshift= 0;
  while (!(greenmask & 1) && gshift < 128)
    {
    gshift++;
    greenmask >>= 1;
    }
  gdepth= 0;
  while (greenmask & 1)
    {
    gdepth++;
    greenmask >>= 1;
    }
  integer bluemask= myvisual->blue_mask;
  bshift= 0;
  while (!(bluemask & 1) && bshift < 128)
    {
    bshift++;
    bluemask >>= 1;
    }
  bdepth= 0;
  while (bluemask & 1)
    {
    bdepth++;
    bluemask >>= 1;
    }
//  cout << " r: " << rshift << " g: " << gshift << " b: " << bshift << " ftiefe: " << ftiefe << endl;
  cout << "Bits per rgb: " << myvisual->bits_per_rgb << endl;
  switch (ftiefe)
    {
    case 4:
      {
      rmax= bmax= 1; gmax= 3; rshift= 3; gshift= 1; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[16];
      for (integer cn= 0; cn < 16; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 65535*(cn >> 3);
        defs[cn].green= 21845*((cn >> 1) & 3);
        defs[cn].blue= 65535*(cn & 1);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 16);
      integer mymask= CWColormap | CWBackPixel | CWEventMask;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 8:
      {
      rmax= gmax= 7; bmax= 3; rshift= 5; gshift= 2; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[256];
      for (integer cn= 0; cn < 256; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 9362*(cn >> 5);
        defs[cn].green= 9362*((cn >> 2) & 7);
        defs[cn].blue= 21845*(cn & 3);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 256);
      integer mymask= CWColormap | CWBackPixel | CWEventMask | CWBackingStore;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      myattr.backing_store= Always;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 15: case 16: case 24:
      {
      rmax= (1 << rdepth) - 1;
      gmax= (1 << gdepth) - 1;
      bmax= (1 << bdepth) - 1;
//      cout << " rmax: " << rmax << " gmax: " << gmax << " bmax: " << bmax << endl;
      integer mymask= CWBackPixel | CWEventMask | CWBackingStore | CWBackingPlanes;
      XSetWindowAttributes myattr;
      myattr.background_pixel= 0;
      myattr.backing_store= Always;
      myattr.backing_planes= 24;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    }
  mygc= XCreateGC (mydisplay, mywindow, 0, 0);
  XStoreName (mydisplay, mywindow, pname);
  XMapRaised (mydisplay, mywindow);
  XFlush (mydisplay);
  bildspeicher= new integer[xanz*yanz];
  usleep (300000);
  }

cmemoryscreen::~cmemoryscreen ()
  {
  XCloseDisplay (mydisplay);
  }

void cmemoryscreen::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    return;
  integer pri (pr);
  integer pgi (pg);
  integer pbi (pb);
  bildspeicher[py*xanz + px]= (pri << rshift) | (pgi << gshift) | (pbi << bshift);
  }

void cmemoryscreen::putpixelx (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    return;
  integer pri (pr);
  integer pgi (pg);
  integer pbi (pb);
  XSetForeground (mydisplay, mygc, (pri << rshift) | (pgi << gshift) | (pbi << bshift));
  XDrawPoint (mydisplay, mywindow, mygc, px, ypix1 - py);
  }

void cmemoryscreen::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  pr= pg= pb= 0;
  }

void cmemoryscreen::flush ()
  {
  for (integer ylauf= 0; ylauf < yanz; ylauf++)
    for (integer xlauf= 0; xlauf < xanz; xlauf++)
      { 
      XSetForeground (mydisplay, mygc, bildspeicher[ylauf*xanz + xlauf]);
      XDrawPoint (mydisplay, mywindow, mygc, xlauf, ypix1 - ylauf);
      }
  XFlush (mydisplay);
  }

void cmemoryscreen::putscreen (clscreen& pscreen, integer px, integer py)
  {
  if ((px >= xanz) | (py >= yanz))
      return;
  real rskala= real (rmax)/pscreen.rmax;
  real gskala= real (gmax)/pscreen.gmax;
  real bskala= real (bmax)/pscreen.bmax;
  integer clipx, clipy;
  if (xanz < px + pscreen.xanz)
      clipx= xanz - px - 1;
    else
      clipx= pscreen.xanz;
  if (yanz < pscreen.yanz)
      clipy= yanz - py - 1;
    else
      clipy= pscreen.yanz;
  integer ftiefe= rdepth + gdepth + bdepth;
  if (ftiefe == 16)
    {
    integer* simage= new integer[clipx*clipy];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]=  (pred << rshift) | (pgreen << gshift) | (pblue << bshift);
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, ftiefe, ZPixmap, 0, (char*) simage, clipx, clipy, 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, px, ypix1 - clipy1 - py, clipx, clipy);
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  if (ftiefe == 24)
    {
    signed char* simage= new signed char[clipx*clipy*4];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]= pblue;
        l_addr++;
        simage[l_addr]= pgreen;
        l_addr++;
        simage[l_addr]= pred;
        l_addr++;
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, ftiefe, ZPixmap, 0, (char*) simage, clipx, clipy, 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, px, ypix1 - clipy1 - py, clipx, clipy);
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  }

void cmemoryscreen::getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer& xmin, integer& ymin, integer& xmax, integer& ymax)
  {
  pexp= 0;
  XNextEvent (mydisplay, &report);
  if (report.type == ButtonPress)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= 1;
    }
  if (report.type == ButtonRelease)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= -1;
    }
  if (report.type == Expose)
    {
    xmin= report.xexpose.x;
    xmax= report.xexpose.x + report.xexpose.width;
    ymin= report.xexpose.y;
    ymax= report.xexpose.y + report.xexpose.height;
    pexp= 1;
    }
  }

bool cmemoryscreen::checkexpose ()
  {
  return XCheckMaskEvent (mydisplay, ExposureMask, &report);
  }

//----------------------------------------------- ximagescreen ----------------------------------------------------------------------------------------------------------------

cximagescreen::cximagescreen (const char* pname, integer px, integer py)
  {
  ypix1= (py - 1);
  xanz= px;
  yanz= py;
  mydisplay= XOpenDisplay ("");
  integer ftiefe= XDefaultDepth (mydisplay, 0);
  cout << "Pixeltiefe: " << ftiefe << endl;
  Visual* myvisual= XDefaultVisual (mydisplay, 0);
  integer redmask= myvisual->red_mask;
  rshift= 0;
  while (!(redmask & 1) && rshift < 128)
    {
    rshift++;
    redmask >>= 1;
    }
  rdepth= 0;
  while (redmask & 1)
    {
    rdepth++;
    redmask >>= 1;
    }
  integer greenmask= myvisual->green_mask;
  gshift= 0;
  while (!(greenmask & 1) && gshift < 128)
    {
    gshift++;
    greenmask >>= 1;
    }
  gdepth= 0;
  while (greenmask & 1)
    {
    gdepth++;
    greenmask >>= 1;
    }
  integer bluemask= myvisual->blue_mask;
  bshift= 0;
  while (!(bluemask & 1) && bshift < 128)
    {
    bshift++;
    bluemask >>= 1;
    }
  bdepth= 0;
  while (bluemask & 1)
    {
    bdepth++;
    bluemask >>= 1;
    }
//  cout << " r: " << rshift << " g: " << gshift << " b: " << bshift << " ftiefe: " << ftiefe << endl;
  cout << "Bits per rgb: " << myvisual->bits_per_rgb << endl;
  switch (ftiefe)
    {
    case 4:
      {
      rmax= bmax= 1; gmax= 3; rshift= 3; gshift= 1; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[16];
      for (integer cn= 0; cn < 16; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 65535*(cn >> 3);
        defs[cn].green= 21845*((cn >> 1) & 3);
        defs[cn].blue= 65535*(cn & 1);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 16);
      integer mymask= CWColormap | CWBackPixel | CWEventMask;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 8:
      {
      rmax= gmax= 7; bmax= 3; rshift= 5; gshift= 2; bshift= 0;
      Colormap cmap= XCreateColormap (mydisplay, XDefaultRootWindow (mydisplay), myvisual, AllocAll);
      XColor defs[256];
      for (integer cn= 0; cn < 256; ++cn)
        {
        defs[cn].pixel= cn;
        defs[cn].red= 9362*(cn >> 5);
        defs[cn].green= 9362*((cn >> 2) & 7);
        defs[cn].blue= 21845*(cn & 3);
        defs[cn].flags= DoRed | DoGreen | DoBlue;
        }
      XStoreColors (mydisplay, cmap, defs, 256);
      integer mymask= CWColormap | CWBackPixel | CWEventMask | CWBackingStore;
      XSetWindowAttributes myattr;
      myattr.colormap= cmap;
      myattr.background_pixel= 0;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      myattr.backing_store= Always;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    case 15: case 16: case 24:
      {
      rmax= (1 << rdepth) - 1;
      gmax= (1 << gdepth) - 1;
      bmax= (1 << bdepth) - 1;
//      cout << " rmax: " << rmax << " gmax: " << gmax << " bmax: " << bmax << endl;
      integer mymask= CWBackPixel | CWEventMask | CWBackingStore | CWBackingPlanes;
      XSetWindowAttributes myattr;
      myattr.background_pixel= 0;
      myattr.backing_store= Always;
      myattr.backing_planes= 24;
      myattr.event_mask= ExposureMask | ButtonPressMask | ButtonReleaseMask;
      mywindow= XCreateWindow (mydisplay, XDefaultRootWindow (mydisplay), 0, 0, xanz, yanz, 0, ftiefe, InputOutput, myvisual, mymask, &myattr);
      break;
      }
    }
  mygc= XCreateGC (mydisplay, mywindow, 0, 0);
  XStoreName (mydisplay, mywindow, pname);
  XMapRaised (mydisplay, mywindow);
  XFlush (mydisplay);
  xbild= XCreateImage (mydisplay, myvisual, ftiefe, ZPixmap, 0, bild, xanz, yanz, 32, xanz*4);
  integer image_byte_size= xanz*yanz*xbild->bits_per_pixel/8;
  bild= (char*) malloc (image_byte_size);
  xbild->data= bild;   // hier wird der Grafikspeicher an das XImage gebunden
  }

cximagescreen::~cximagescreen ()
  {
  XDestroyImage (xbild);
  XCloseDisplay (mydisplay);
  }

void cximagescreen::putpixel (integer px, integer py, real pr, real pg, real pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    return;
  integer pri= integer (pr);
  integer pgi= integer (pg);
  integer pbi= integer (pb);
  integer pos;

  pos= 4*(px + (ypix1 - py)*xanz);
  bild[pos]= char (pbi);
  bild[pos+1]= (signed char) (pgi);
  bild[pos+2]= (signed char) (pri);
  }

void cximagescreen::getpixel (integer px, integer py, real &pr, real &pg, real &pb)
  {
  if ((px < 0) || (px >= xanz) || (py < 0) || (py >= yanz))
    {
    pr= pg= pb= 0;
    return;
    }
  pr= pg= pb= 0;
  }

void cximagescreen::flush ()
  {
  XPutImage (mydisplay, mywindow, mygc, xbild, 0, 0, 0, 0, (unsigned short) (xanz), (unsigned short) (yanz));
  }

void cximagescreen::putscreen (clscreen& pscreen, integer px, integer py)
  {
  if ((px >= xanz) | (py >= yanz))
      return;
  real rskala= real (rmax)/pscreen.rmax;
  real gskala= real (gmax)/pscreen.gmax;
  real bskala= real (bmax)/pscreen.bmax;
  integer clipx, clipy;
  if (xanz < px + pscreen.xanz)
      clipx= xanz - px - 1;
    else
      clipx= pscreen.xanz;
  if (yanz < pscreen.yanz)
      clipy= yanz - py - 1;
    else
      clipy= pscreen.yanz;
  integer ftiefe= rdepth + gdepth + bdepth;
  if (ftiefe == 16)
    {
    integer* simage= new integer[clipx*clipy];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]=  (pred << rshift) | (pgreen << gshift) | (pblue << bshift);
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, (unsigned short) (ftiefe), ZPixmap, 0, (char*) simage, (unsigned short) (clipx), (unsigned short) (clipy), 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, (signed short) (px), (signed short) (ypix1 - clipy1 - py), (unsigned short) (clipx), (unsigned short) (clipy));
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  if (ftiefe == 24)
    {
    signed char* simage= new signed char[clipx*clipy*4];
    integer l_addr= 0;
    integer clipy1= clipy - 1;
    for (integer l_y= clipy1; l_y >= 0; l_y--)
      for (integer l_x= 0; l_x < clipx; l_x++)
        {
        real gred, ggreen, gblue;
        pscreen.getpixel (l_x, l_y, gred, ggreen, gblue);
        integer pred= (integer) (gred * rskala + drand48 ());
        integer pgreen= (integer) (ggreen * gskala + drand48 ());
        integer pblue= (integer) (gblue * bskala + drand48 ());
        simage[l_addr]= (signed char) (pblue);
        l_addr++;
        simage[l_addr]= (signed char) (pgreen);
        l_addr++;
        simage[l_addr]= (signed char) (pred);
        l_addr++;
        l_addr++;
        }
    Visual* myvisual= XDefaultVisual (mydisplay, 0);
    XImage* ximage= XCreateImage (mydisplay, myvisual, (unsigned short) (ftiefe), ZPixmap, 0, (char*) simage, (unsigned short) (clipx), (unsigned short) (clipy), 8, 0);
    XPutImage (mydisplay, mywindow, mygc, ximage, 0, 0, (signed short) (px), (signed short) (ypix1 - clipy1 - py), (unsigned short) (clipx), (unsigned short) (clipy));
    XFlush (mydisplay);
    XDestroyImage (ximage);
    }
  }

void cximagescreen::getmaus (integer &paction, integer &px, integer &py, integer &pexp, integer& xmin, integer& ymin, integer& xmax, integer& ymax)
  {
  pexp= 0;
  XNextEvent (mydisplay, &report);
  if (report.type == ButtonPress)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= 1;
    }
  if (report.type == ButtonRelease)
    {
    px= report.xbutton.x;
    py= ypix1 - report.xbutton.y;
    paction= report.xbutton.button;
    paction*= -1;
    }
  if (report.type == Expose)
    {
    xmin= report.xexpose.x;
    xmax= report.xexpose.x + report.xexpose.width;
    ymin= report.xexpose.y;
    ymax= report.xexpose.y + report.xexpose.height;
    pexp= 1;
    }
  }

bool cximagescreen::checkexpose ()
  {
  return XCheckMaskEvent (mydisplay, ExposureMask, &report);
  }
