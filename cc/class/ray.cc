#include "ray.h"

#include <iostream>        // die Weltdaten aus einer Datei lesen
#include <sys/times.h>     // tms, times

using namespace std;  // cout

cschnittpunkte::cschnittpunkte ()
  {
  anz= 0;
  }

void cschnittpunkte::add (const real &pf)
  {
  abstand[anz]= pf;
  anz++;
  }

//--------------------------------- Schnittpunktberechnungsobjekte -------------------------------------------------------------------------------------------
// ---------------------------- Ebene -----------------------------------------------
csebene::csebene ()
  {
  z= 0;
  }

void csebene::init (const cvektor3 &pov)
  {
  z= -pov.z;
  }

void csebene::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real s1= z/prv.z;
  if (s1 > 0)
    psp.add (s1);
  }

// ---------------------------- Zylinder -----------------------------------------------

cszylinder::cszylinder ()
  {
  ov= cvektor3 (0, 0, 0);
  }

void cszylinder::init (const cvektor3 &pov)
  {
  ov= pov;
  c= ov.x*ov.x + ov.y*ov.y - 1;
  }

void cszylinder::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real a= prv.x*prv.x + prv.y*prv.y;
  real b= -prv.x*ov.x - prv.y*ov.y;
  real d= b*b - a*c;
  if (d < 0)
    return;
  real e= sqrtl (d);
  real s1= (b + e)/a;
  if (s1 > 0)
    psp.add (s1);
  real s2= (b - e)/a;
  if (s2 > 0)
    psp.add (s2);
  }

// ---------------------------- Kugel -----------------------------------------------

cskugel::cskugel ()
  {
  ov= cvektor3 (0, 0, 0);
  }

void cskugel::init (const cvektor3 &pov)
  {
  ov= pov;
  c= ov%ov - 1;
  }

void cskugel::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real a= prv%prv;
  real b= -(prv%ov);
  real d= b*b - a*c;
  if (d < 0)
    return;
  real e= sqrtl (d);
  real s1= (b + e)/a;
  if (s1 > 0)
    psp.add (s1);
  real s2= (b - e)/a;
  if (s2 > 0)
    psp.add (s2);
  }

// ---------------------------- zweischaliges Hyperboloid, Kegel, einschaliges Hyperboloid -----------------------------------------------

cshyper::cshyper (const real &pf)    // dieser Wert wählt zwischen den Körpern aus (-1, 0, 1)
  {
  f= pf;
  ov= cvektor3 (0, 0, 0);
  }

void cshyper::init (const cvektor3 &pov)
  {
  ov= pov;
  c= ov.x*ov.x + ov.y*ov.y - ov.z*ov.z + f;
  }

void cshyper::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real a= prv.x*prv.x + prv.y*prv.y - prv.z*prv.z;
  real b= -prv.x*ov.x - prv.y*ov.y + prv.z*ov.z;
  if (a == 0)
    {
    if (b == 0)
      return;
    real s3= 0.5*c/b;
    if (s3 > 0)
      psp.add (s3);
    return;
    }
  real d= b*b - a*c;
  if (d < 0)
    return;
  real e= sqrtl (d);
  real s1= (b + e)/a;
  if (s1 > 0)
    psp.add (s1);
  real s2= (b - e)/a;
  if (s2 > 0)
    psp.add (s2);
  }

// ---------------------------- Rotationsparaboloid ----------------------------------------

csrpara::csrpara ()
  {
  ov= cvektor3 (0, 0, 0);
  }

void csrpara::init (const cvektor3 &pov)
  {
  ov= pov;
  c= ov.x*ov.x + ov.y*ov.y - ov.z;
  }

void csrpara::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real a= prv.x*prv.x + prv.y*prv.y;
  real b= -prv.x*ov.x - prv.y*ov.y + prv.z/2;
  real d= b*b - a*c;
  if (d < 0)
    return;
  real e= sqrtl (d);
  real s1= (b + e)/a;
  if (s1 > 0)
    psp.add (s1);
  real s2= (b - e)/a;
  if (s2 > 0)
    psp.add (s2);
  }

// ---------------------------- hyperbolisches Paraboloid ----------------------------------

cshpara::cshpara ()
  {
  ov= cvektor3 (0, 0, 0);
  }

void cshpara::init (const cvektor3 &pov)
  {
  ov= pov;
  c= ov.x*ov.x - ov.y*ov.y - ov.z;
  }

void cshpara::berechne (const cvektor3 &prv, cschnittpunkte &psp)
  {
  real a= prv.x*prv.x - prv.y*prv.y;
  real b= -prv.x*ov.x + prv.y*ov.y + prv.z/2;
  if (a == 0)
    {
    if (b == 0)
      return;
    real s3= 0.5*c/b;
    if (s3 > 0)
      psp.add (s3);
    return;
    }
  real d= b*b - a*c;
  if (d < 0)
    return;
  real e= sqrtl (d);
  real s1= (b + e)/a;
  if (s1 > 0)
    psp.add (s1);
  real s2= (b - e)/a;
  if (s2 > 0)
    psp.add (s2);
  }

// ---------------------------------------- Parametrisierungen der Oberflächen --------------------------------------------------------------------------
//----------- Ebene winkeltreu -------------------------

cvektor2 cparawebene::berechne (const cvektor3 &pv)
  {
  return cvektor2 (pv.x, pv.y);
  }

//----------- Ebene polar ------------------------------

cvektor2 cparapolebene::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), hypotl (pv.x, pv.y));
  }

//----------- Ebene polar winkeltreu -------------------

cvektor2 cparapolwebene::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), logl (hypotl (pv.x, pv.y)));
  }

//----------- Zylinder winkeltreu-------------------------------

cvektor2 cparawzylinder::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), pv.z);
  }

//----------- Kugel winkeltreu ------------------------

cvektor2 cparawkugel::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), atanhl (pv.z));
  }

//----------- Kugel normal ----------------------------

cvektor2 cpararkugel::berechne (const cvektor3 &pv)
  {
  return cvektor2 (pv.x/(1 + pv.z), pv.y/(1 + pv.z));
  }

cvektor2 cparakugel::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), asinl (pv.z));
  }

//----------- Hyperboloid normal -----------------------

cvektor2 cparaehyper::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), asinhl (pv.z));
  }

//----------- Hyperboloid nicht normal -----------------------

cvektor2 cparazhyper::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), acoshl (fabsl (pv.z)));
  }

//------------------ Kegel ------------------------------------

cvektor2 cparawkegel::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), logl (fabsl (pv.z)));
  }

//----------- Rotationsparaboloid ----------------------

cvektor2 cpararpara::berechne (const cvektor3 &pv)
  {
  return cvektor2 (atan2l (pv.y, pv.x), sqrtl (pv.z));
  }

// -------------------------------------- Begrenzungsobjekte --------------------------------------------------------------------------
// -------------------- keine Begrenzung ------------------------------------

integer cbegrkeine::sichtbar (const cvektor2 &pv)
  {
  return 1;
  while (pv.x != pv.x);        // Variable benutzen, damit Compiler nicht meckert unused variable
  }

// -------------------- Rechteck ------------------------------------

cbegrrechteck::cbegrrechteck (const real plinks, const real prechts, const real punten, const real poben)
  {
  links= plinks;
  rechts= prechts;
  oben= poben;
  unten= punten;
  }

integer cbegrrechteck::sichtbar (const cvektor2 &pv)
  {
  if (pv.x < links)
    return 0;
  if (pv.x > rechts)
    return 0;
  if (pv.y < unten)
    return 0;
  if (pv.y > oben)
    return 0;
  return 1;
  }

// -------------------- Kreis ------------------------------------

cbegrkreis::cbegrkreis (const real plinks, const real prechts, const real pinnen, const real paussen)
  {
  links= plinks;
  rechts= prechts;
  innen= pinnen;
  aussen= paussen;
  }

integer cbegrkreis::sichtbar (const cvektor2 &pv)
  {
  real r= hypotl (pv.x, pv.y);
  if (r > aussen)
    return 0;
  if (r < innen)
    return 0;
  real w= atan2l (pv.y, pv.x);
  if (w < links)
    return 0;
  if (w > rechts)
    return 0;
  return 1;
  }

// -------------------- Ellipse ------------------------------------

cbegrellipse::cbegrellipse (const real pha, const real phb)
  {
  ha= pha;
  hb= phb;
  }

integer cbegrellipse::sichtbar (const cvektor2 &pv)
  {
  real xh= pv.x/ha;
  real yh= pv.y/hb;
  if (xh*xh + yh*yh > 1)
    return 0;
  return 1;
  }

// --------------------------------- Mannigfaltigkeiten --------------------------------------------------------------------------

clmannig::clmannig ()
  {
  srand (56);
  }

cvektor3 clmannig::getpixel (const cvektor2 &pv)
  {
  cvektor3 sum (0, 0, 0);
  real xm (pv.x - 1);
  real ym (pv.y - 1);
  for (integer yl= 0; yl < 4; ++yl)
    {
    xm= pv.x - 1;
    for (integer xl= 0; xl < 4; ++xl)
      {
      sum= sum + getpunkt (cvektor2 (xm + drand48 ()*0.25, ym + drand48 ()*0.25));
      xm+= 0.25;
      }
    ym+= 0.25;
    }
  return sum/16;
  }

//------------------------------- Färbungen  ----------------------------------------------------------------
//------------------------- Schachfeld ---------------------------------

cmonochrom::cmonochrom (const cvektor3 &pfarbe)
  {
  farbe= pfarbe;
  }

cvektor3 cmonochrom::getpunkt (const cvektor2 &pv)
  {
  return farbe;
  if (pv.x != pv.x)   // pv benutzen, weil sonst der Compiler meckert
    {
    }
  }

cschachfeld::cschachfeld (const cvektor3 &pfb1, const cvektor3 &pfb2, const real &pkx, const real &pky)
  {
  fb1= pfb1;
  fb2= pfb2;
  kx= pkx;
  ky= pky;
  }

cvektor3 cschachfeld::getpunkt (const cvektor2 &pv)
  {
  if (integer (fabsl (floorl (pv.x*kx) + floorl (pv.y*ky))) & 1)
    return fb2;
  return fb1;
  }

cscreenmannig::cscreenmannig (clscreen* pscreen, real pzoom)
  : screen (pscreen), zoom (pzoom), xmax (screen->xanz - 1), ymax (screen->yanz - 1),
    rmax (screen->rmax), gmax (screen->gmax), bmax (screen->bmax)
  {
  }

cvektor3 cscreenmannig::getpunkt (const cvektor2 &pv)
  {
  integer x= integer (fmodl (fabsl (pv.x/zoom*xmax), xmax) + 0.5);
  integer y= integer (fmodl (fabsl (pv.y/zoom*xmax), ymax) + 0.5);
  real r, g, b;
  screen->getpixel (x, y, r, g, b);
  return cvektor3 (r/rmax, g/gmax, b/bmax);
  }

// ---------------------------------------------------Geometrische Gebilde --------------------------------------------------------------------------
// ------------------------ ckörper --------------------------------------------------------------------------

ckoerper::ckoerper (clschnitt* pschnitt, clpara* ppara, clbegr* pbegr, clmannig* pmannig, const cvektor3 &ppos, const cbasis3 &pbasis)
  : koerperpos (ppos), schnitt (pschnitt), para (ppara), begr (pbegr), mannig (pmannig), koerperbasis (pbasis),
    drehbasis (cvektor3 (1, 0, 0), cvektor3 (0, 1, 0), cvektor3 (0, 0, 1)),
    augpos (0, 0, 0),
    augbasis (cvektor3 (1, 0, 0), cvektor3 (0, 1, 0), cvektor3 (0, 0, 1)),
    transbasis (cvektor3 (1, 0, 0), cvektor3 (0, 1, 0), cvektor3 (0, 0, 1)),
    transpos (0, 0, 0), transrich (1, 0, 0)
  {
  startzeit= 0;
  stopzeit= 0;
  startpos= koerperpos;
  stoppos= koerperpos;
  }

void ckoerper::setzeauge (const cvektor3 &paugpos, const cbasis3 &paugbasis)
  {
  augpos= paugpos;
  augbasis= paugbasis;
  const cbasis3 gkoerperbasis (drehbasis*koerperbasis);
  transbasis= augbasis/gkoerperbasis;
  transpos= (augpos - koerperpos)/gkoerperbasis;
  schnitt->init (transpos);
  }

void ckoerper::drehe (const cbasis3 &pdrehbasis)
  {
  drehbasis= normiere (transp (transp (augbasis*pdrehbasis)/transp (augbasis)) * drehbasis);
  const cbasis3 gkoerperbasis (drehbasis*koerperbasis);
  transbasis= augbasis/gkoerperbasis;
  transpos= (augpos - koerperpos)/gkoerperbasis;
  schnitt->init (transpos);
  }

void ckoerper::dreheein ()
  {
  cvektor4 aw (winkelachsefrommatrix (koerperbasis));
  if (aw.r < 0.01745329)
    aw.r= -aw.r;
    else
    aw.r*= -0.2;
  cbasis3 db (matrixfromquaternion (quaternionfromwinkelachse (aw)));
  drehe (db);
  }

void ckoerper::verschiebe (const cvektor3 &pv)
  {
  koerperpos= koerperpos + augbasis*pv;
  }

void ckoerper::setzepos (const cvektor3 &pv)
  {
  koerperpos= pv;

  const cbasis3 gkoerperbasis (drehbasis*koerperbasis);
  transpos= (augpos - koerperpos)/gkoerperbasis;
  schnitt->init (transpos);
  }

void ckoerper::schneide (const cvektor3 &psv, cschnittpunkte &psp)
  {
  transrich= transbasis*psv;
  schnitt->berechne (transrich, psp);
  }

void ckoerper::begrenze (const integer pnr, cschnittpunkte &psp)              // parametrisiert und begrenzt den Körper
  {
  psp.mannigpos[pnr]= para->berechne (transpos + psp.abstand[pnr]*transrich);
  psp.sichtbar[pnr]= begr->sichtbar (psp.mannigpos[pnr]);
  }

cvektor3 ckoerper::faerbe (const integer pnr, cschnittpunkte &psp)
  {
  return mannig->getpunkt (psp.mannigpos[pnr]);
  }

void ckoerper::aktualisiere ()
  {
  tms zeit;
  real bewpos= 0;
  real zeitpos;
  clock_t zeitint= times (&zeit);
  zeitpos= zeitint;
  //printf ("Zeit: %5.2Lf  \n", zeitpos);
  if (zeitpos > startzeit)
    {
    if (zeitpos < stopzeit)
      {
      bewpos= (zeitpos - startzeit)/(stopzeit - startzeit);
      koerperpos= (1 - bewpos)*startpos + bewpos*stoppos;
      }
      else
      {
      koerperpos= stoppos;
      }
    }

  const cbasis3 gkoerperbasis (drehbasis*koerperbasis);
  transpos= (augpos - koerperpos)/gkoerperbasis;
  schnitt->init (transpos);
  }

// ------------------------ ckörperliste --------------------------------------------------------------------------

ckoerperliste::ckoerperliste () : anz (0) {}

void ckoerperliste::add (ckoerper* pkoerper)
  {
  koerper[anz++]= pkoerper;
  }

void ckoerperliste::setzeauge (const cvektor3 &paugpos, const cbasis3 &paugbasis)
  {
  for (integer klauf= 0; klauf < anz; ++klauf)
    koerper[klauf]->setzeauge (paugpos, paugbasis);
  }

//----------------------------- Welt ---------------------------------------------------------------------------------------------------------

cwelt::cwelt (const real &pabstand, const cvektor3 &paugpos, const cbasis3 &paugbasis)
  : abstand (pabstand), augpos (paugpos), augbasis (paugbasis)
  {
  augdrehaw= cvektor4 (0, 0, 0, 0);
  }

cwelt::cwelt (const real &pabstand, const char* pname)
  : abstand (pabstand), augpos (0,0,0),
    augbasis (cvektor3 (0,0,0), cvektor3 (0,0,0), cvektor3 (0,0,0))
  {
  augdrehaw= cvektor4 (0, 0, 0, 0);
  FILE* datei= fopen (pname, "rb");
  integer anz;
  anz= fread (&augpos, sizeof (augpos), 1, datei);
  anz= fread (&augbasis, sizeof (augbasis), 1, datei);
  anz= fread (&anz, sizeof (anz), 1, datei);
  }

cvektor3 cwelt::getpunkt (const cvektor2 &pv)
  {
  cvektor3 sv (pv.x, pv.y, abstand);                      // Ebenenprojektion
  //cvektor3 sv (sin(pv.x/abstand*2*M_PI), pv.y/abstand*2*M_PI, cos (pv.x/abstand*2*M_PI));   // Zylinderprojektion

  // Schnittpunkte erstellen, Schnittpunkte parametrisieren, Objekt begrenzen
  cschnittpunkte schnittpunkte;
  for (integer klauf= 0; klauf < koerperliste.anz; klauf++)
    {
    integer lschnittanz= schnittpunkte.anz;
    koerperliste.koerper[klauf]->schneide (sv, schnittpunkte);
    for (integer knrlauf= lschnittanz; knrlauf < schnittpunkte.anz; knrlauf++)
      {
      schnittpunkte.koerpernr[knrlauf]= klauf;
      koerperliste.koerper[klauf]->begrenze (knrlauf, schnittpunkte);
      }
    }
  if (schnittpunkte.anz == 0)
    return himmelfarbe;             // bei keinem Schnittpunkt Hintergrundfarbe zurückgeben

  // nahsten Schnittpunkt heraussuchen
  integer kmin= -1;
  for (signed long nlauf= 0; nlauf < schnittpunkte.anz; nlauf++)
    if (schnittpunkte.sichtbar[nlauf])
      {
      if (kmin == -1)
        {
        kmin= nlauf;
        }
        else
        {
        if (schnittpunkte.abstand[nlauf] < schnittpunkte.abstand[kmin])
          kmin= nlauf;
        }
      }

  // bei keinem Schnittpunkt Hintergrundfarbe zurückgeben
  if (kmin == -1)
    return himmelfarbe;

  // Farbe des nahsten Schnittpunkts zurückgeben
  return koerperliste.koerper[schnittpunkte.koerpernr[kmin]]->faerbe (kmin, schnittpunkte);
  }

void cwelt::addkoerper (ckoerper* pkoerper)
  {
  pkoerper->setzeauge (augpos, augbasis);
  koerperliste.add (pkoerper);
  }

void cwelt::setzeposition (const cvektor3 &pv)
  {
  augpos= pv;
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::verschiebeauge (const cvektor3 &pv)
  {
  augpos= augpos + augbasis*pv;
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::setzelage (const cbasis3 &pb)
  {
  augbasis= normiere (pb);
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheauge (const cbasis3 &pb)
  {
  augbasis= normiere (augbasis*pb);
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugex (const real pwinkel)
  {
  augbasis= normiere (augbasis*getrotx(pwinkel));
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugey (const real pwinkel)
  {
  augbasis= normiere (augbasis*getroty(pwinkel));
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugez (const real pwinkel)
  {
  augbasis= normiere (augbasis*getrotz(pwinkel));
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugeachse (const real pwinkel)
  {
  augdrehaw.r= pwinkel;
  cbasis3 achsdrehbasis (matrixfromwinkelachse (augdrehaw));
  augbasis= normiere (achsdrehbasis*augbasis);
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugenorm (const real pwinkel)
  {
  if (pwinkel != pwinkel) {};  // wegen unused variable
  cvektor3 flugw (eulerwinkelfrommatrix (augbasis));
  cbasis3 bnorm (getroty (flugw.x));
  cvektor4 aw (winkelachsefrommatrix (augbasis/bnorm));
  if (aw.r < 0.01745329)
    aw.r= -aw.r;
  else
    aw.r*= -0.2;
  cbasis3 db (matrixfromwinkelachse (aw));
  augbasis= normiere (augbasis*db);
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::dreheaugeein (const real pwinkel)
  {
  if (pwinkel != pwinkel) {};  // wegen unused variable
  cvektor4 aw (winkelachsefrommatrix (augbasis));
  if (aw.r < 0.01745329)
    aw.r= -aw.r;
  else
    aw.r*= -0.2;
  cbasis3 db (matrixfromwinkelachse (aw));
  augbasis= normiere (augbasis*db);
  koerperliste.setzeauge (augpos, augbasis);
  }

void cwelt::aktualisiere ()
  {
  for (integer lauf= 0; lauf < koerperliste.anz; lauf++)
    koerperliste.koerper[lauf]->aktualisiere ();
  }

//---------------------------------- Bildklassen --------------------------------------------------------------------------

cpunkt::cpunkt () : pos (0,0), farbe (0,0,0) {}

cpunktscreen::cpunktscreen (integer px, integer py)
  : xanz (px), yanz (py)
  {
  vbild= new cpunkt[xanz*yanz];
  srand48 (56);
  }

cpunktscreen::~cpunktscreen ()
  {
  delete (vbild);
  }

void cpunktscreen::fuelle (clmannig &pmannig)
  {
  const real xoff (-xanz/(real) (2));
  const real yoff (-yanz/(real) (2));
  for (integer ny= 0; ny < yanz; ++ny)
    for (integer nx= 0; nx < xanz; ++nx)
      {
      vbild[ny*xanz + nx].pos.x= nx + xoff + drand48 ();
      vbild[ny*xanz + nx].pos.y= ny + yoff + drand48 ();
      vbild[ny*xanz + nx].farbe= pmannig.getpunkt (vbild[ny*xanz + nx].pos);
      }
  }

void cpunktscreen::plotte (clscreen &plscreen)
  {
  for (integer ny= 0; ny < plscreen.yanz; ++ny)
    for (integer nx= 0; nx < plscreen.xanz; ++nx)
      {
      //integer vx= (integer) (real(nx)/(plscreen.xanz-1)*(xanz-1) + 0.5);
      //integer vy= (integer) (real(ny)/(plscreen.yanz-1)*(yanz-1) + 0.5);
      //plscreen.putpixel (nx, ny, vbild[vy*xanz + vx].farbe);
      }
  plscreen.flush ();
  }
