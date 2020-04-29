#ifndef ray_h
#define ray_h

#include "../screen/vscr.h"  // für die screen-Textur aus einem Screen (z.B. jpeg-Datei) texturieren zu können

// ------- Schnittpunktklasse ------------------------------------------------------------------------------------------------

struct cschnittpunkte
  {
  integer        anz;
  integer        koerpernr[1000];
  integer        sichtbar[1000];
  real           abstand[1000];
  cvektor2       mannigpos[1000];

  cschnittpunkte ();
  void add (const real &pf);
  };

//-------------- Schnittobjekte ------------------------------------------------------

struct clschnitt  // abstrakte Schnittklasse von der die konkreten Schnittklassen abgeleitet sind
  {
  virtual void init (const cvektor3 &pov)= 0;
  virtual void berechne (const cvektor3 &prv, cschnittpunkte &psp)= 0;
  };

struct csebene : public clschnitt
  {
  csebene ();
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  real z;
  };

struct cszylinder : public clschnitt
  {
  cszylinder ();
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  cvektor3 ov;
  real c;
  };

struct cskugel : public clschnitt
  {
  cskugel ();
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  cvektor3 ov;
  real c;
  };

struct cshyper : public clschnitt
  {
  cshyper (const real &pf);
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  real f;
  cvektor3 ov;
  real c;
  };

struct csrpara : public clschnitt
  {
  csrpara ();
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  cvektor3 ov;
  real c;
  };

struct cshpara : public clschnitt
  {
  cshpara ();
  void init (const cvektor3 &pov);
  void berechne (const cvektor3 &prv, cschnittpunkte &psp);

  private:
  cvektor3 ov;
  real c;
  };

//------------------------- Parametrisierungsobjekte -------------------------------------------------------------------------------------------------


struct clpara                                                // abstraktes Parametrisierungsobjekt, von dem die konkreten Parametrisierungsobjekte abgeleitet sind
  {
  virtual cvektor2 berechne (const cvektor3 &pv)= 0;
  };

struct cparawebene : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparapolebene : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparapolwebene : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparawzylinder : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparawkugel : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cpararkugel : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparakugel : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparaehyper : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparazhyper : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cparawkegel : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

struct cpararpara : clpara
  {
  cvektor2 berechne (const cvektor3 &pv);
  };

// ---------------------------------------------------------------------------------------------
// Begrenzungsobjekte

struct clbegr     // abstraktes Begrenzungsobjekt, von dem die konkreten Begrenzungsobjekte abgeleitet sind
  {
  virtual integer sichtbar (const cvektor2 &pv)= 0;
  };

struct cbegrkeine : clbegr
  {
  integer sichtbar (const cvektor2 &pv);
  };

struct cbegrrechteck : clbegr
  {
  cbegrrechteck (const real plinks, const real prechts, const real punten, const real poben);
  integer sichtbar (const cvektor2 &pv);

  private:
  real links, rechts, oben, unten;
  };

struct cbegrkreis : clbegr
  {
  cbegrkreis (const real plinks, const real prechts, const real pinnen, const real paussen);
  integer sichtbar (const cvektor2 &pv);
  
  real links, rechts, innen, aussen;
  };

struct cbegrellipse : clbegr
  {
  cbegrellipse (const real pha, const real phb);
  integer sichtbar (const cvektor2 &pv);

  private:
  real ha, hb;
  };

//------------------------------- Mannigfaltigkeiten ----------------------------------------------------------------------------------------------
// Texturen

struct clmannig     // abstrakte Textur, von der die konkreten Texturen abgeleitet sind
  {
  clmannig ();
  virtual cvektor3 getpunkt (const cvektor2 &pv)= 0;         // Färbung eines Punktes berechnen
  cvektor3 getpixel (const cvektor2 &pv);                    // Färbung einer Pixelfläche berechnen, benutzt getpunkt, allgemein für clmannig, unabhängig vom abgeleitetem Objekt
  };

struct cmonochrom : public clmannig
  {
  cmonochrom (const cvektor3 &pfarbe);
  cvektor3 getpunkt (const cvektor2 &pv);

  private:
  cvektor3 farbe;
  };

struct cschachfeld : public clmannig
  {
  cschachfeld (const cvektor3 &pfb1, const cvektor3 &pfb2, const real &pkx, const real &pky);
  cvektor3 getpunkt (const cvektor2 &pv);

  private:
  cvektor3 fb1, fb2;        // Färbung des Schachfelds
  real kx, ky;              // Skalierung des Schachfelds (Größe)
  };

struct cscreenmannig : public clmannig     // Zum Texturieren mit Bildern
  {
  cscreenmannig (clscreen* pscreen, real pzoom);
  cvektor3 getpunkt (const cvektor2 &pv);

  private:
  clscreen* screen;
  real zoom, xmax, ymax, rmax, gmax, bmax;
  };

// ------------------------------------------------------------------------------------------------
// Geometrische Gebilde

struct ckoerper
  {
  ckoerper (clschnitt* pschnitt, clpara* ppara, clbegr* pbegr, clmannig* pmannig, const cvektor3 &ppos, const cbasis3 &pbasis);
  void setzeauge (const cvektor3 &paugpos, const cbasis3 &paugbasis);
  void drehe (const cbasis3 &pdrehbasis);
  void dreheein ();                                  // auf Einheitsposition setzen
  void verschiebe (const cvektor3 &pv);
  void setzepos (const cvektor3 &pv);
  void schneide (const cvektor3 &psv, cschnittpunkte &psp);
  void begrenze (const integer pnr, cschnittpunkte &psp);
  cvektor3 faerbe (const integer pnr, cschnittpunkte &psp);
  void aktualisiere ();

  // Bewegung
  real        startzeit, stopzeit;
  cvektor3    startpos, stoppos;
  cvektor3    koerperpos;

  private:
  clschnitt*  schnitt;
  clpara*     para;
  clbegr*     begr;
  clmannig*   mannig;

  cbasis3     koerperbasis;
  cbasis3     drehbasis;
  cvektor3    augpos;
  cbasis3     augbasis;
  cbasis3     transbasis;
  cvektor3    transpos;
  cvektor3    transrich;

  };

struct ckoerperliste
  {
  integer          anz;
  ckoerper*        koerper[1000];

  ckoerperliste ();
  void add (ckoerper* pkoerper);
  void setzeauge (const cvektor3 &paugpos, const cbasis3 &paugbasis);
  };

struct cwelt : public clmannig
  {
  cwelt (const real &pabstand, const cvektor3 &paugpos, const cbasis3 &paugbasis);
  cwelt (const real &pabstand, const char* pname);
  cvektor3 getpunkt (const cvektor2 &pv);
  void addkoerper (ckoerper* pkoerper);

  void setzeposition (const cvektor3 &pv);
  void verschiebeauge (const cvektor3 &pv);

  void setzelage (const cbasis3 &pb);
  void dreheauge (const cbasis3 &pb);
  void dreheaugex (const real pwinkel);
  void dreheaugey (const real pwinkel);
  void dreheaugez (const real pwinkel);
  void dreheaugeachse (const real pwinkel);
  void dreheaugenorm (const real pwinkel);
  void dreheaugeein (const real pwinkel);   // auf Einheitsposition setzen
  void aktualisiere ();
  

  ckoerperliste  koerperliste;
  real           abstand;               // Abstand der Bildebene in Pixeln vom Ursprung in z-Richtung = Abstand des Betrachters vom Monitor in Pixeln
  cvektor3       augpos;
  cbasis3        augbasis;
  cvektor4       augdrehaw;
  cvektor3       himmelfarbe;
  };

// --------------------------------------------------------------------------------------------
// Bildklassen

struct cpunkt
  {
  cvektor2  pos;
  cvektor3  farbe;
  cpunkt ();
  };

struct cpunktscreen
  {
  integer xanz, yanz;

  cpunktscreen (integer px, integer py);
  ~cpunktscreen ();
  void fuelle (clmannig &pmannig);
  void plotte (clscreen &plscreen);

  private:
  cpunkt  *vbild;
  };

#endif
