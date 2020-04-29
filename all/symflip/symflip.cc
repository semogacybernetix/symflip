// Dieses Programm visualisiert Symmetriegruppen 3-dimensionaler Figuren
// Es ist das Program quaternionentest welches den Raytracer für die Grafikausgabe inkludiert

// 1.618033988749894848204586834365638117720309179                g
// 0.618033988749894848204586834365638117720309179                g^-1
// 0.8090169943749474241022934171828                          0.5 g
// 0.3090169943749474241022934171828                          0.5 g^-1

// Berechnungen
#include "../../cc/class/gruppe.h"
#include "../../cc/class/game.h"

// Ausgabemedien
#include "../../conio/gruppecon.h"
#include "../../cc/screen/allscr.h"
#include "../../xwindow/screen/xscr.h"
#include "../../xwindow/keyboard/xkbd.h"

// Standardlibs
#include <iostream>               // getchar, scanf
#include <sys/times.h>            // tms, times

const integer lmax= 1000;         //  maximale Elementeanzahl der Gruppen

//***************************************** Grafikroutinen ***************************************************************

void addebenen90 (cwelt& pwelt)
  {
  // Körperstandpunkte
  cvektor3 kst1 (0,0,0);

  // Körperlagen
  cbasis3 kl2 (cvektor3 (10,0,0), cvektor3 (0,10,0), cvektor3 (0,0,10));
  cbasis3 kl1 (cvektor3 (0,10,0), cvektor3 (0,0,10), cvektor3 (10,0,0));
  cbasis3 kl3 (cvektor3 (0,0,10), cvektor3 (10,0,0), cvektor3 (0,10,0));

  // Begrenzungen
  cbegrellipse* ell1= new cbegrellipse (10, 10);

  // Texturen
  cschachfeld* textur1= new cschachfeld (cvektor3 (1,0,0), cvektor3 (1,.5,.5), 1, 1);
  cschachfeld* textur2= new cschachfeld (cvektor3 (0,1,0), cvektor3 (.5,1,.5), 1, 1);
  cschachfeld* textur3= new cschachfeld (cvektor3 (0,0,1), cvektor3 (.5,.5,1), 1, 1);

  // Körper addieren
  pwelt.addkoerper (new ckoerper (new csebene, new cparawebene, ell1, textur1, kst1, kl1));
  pwelt.addkoerper (new ckoerper (new csebene, new cparawebene, ell1, textur2, kst1, kl2));
  pwelt.addkoerper (new ckoerper (new csebene, new cparawebene, ell1, textur3, kst1, kl3));
  }

void addkoordinatensystem (cwelt& pwelt)
  {
  // Körperstandpunkte
  cvektor3 kst1 (0,0,0);

  // Körperlagen
  cbasis3 kl2 (cvektor3 (0, 0, 1), cvektor3 (1, 0, 0), cvektor3 (0, 1, 0));
  cbasis3 kl1 (cvektor3 (0, 1, 0), cvektor3 (0, 0, 1), cvektor3 (1, 0, 0));
  cbasis3 kl3 (cvektor3 (1, 0, 0), cvektor3 (0, 1 ,0), cvektor3 (0, 0, 1));

  // Begrenzungen
  cbegrrechteck* begrkoord= new cbegrrechteck (-4, 4, -1000, 1000);

  // Texturen
  cschachfeld* textur1= new cschachfeld (cvektor3 (1,0,0), cvektor3 (1,.5,.5), 2/M_PIl, 1);
  cschachfeld* textur2= new cschachfeld (cvektor3 (0,1,0), cvektor3 (.5,1,.5), 2/M_PIl, 1);
  cschachfeld* textur3= new cschachfeld (cvektor3 (0,0,1), cvektor3 (.5,.5,1), 2/M_PIl, 1);

  // Körper addieren
  pwelt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrkoord, textur1, kst1, kl1));
  pwelt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrkoord, textur2, kst1, kl2));
  pwelt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrkoord, textur3, kst1, kl3));
  }

// Vektor mit geringstem Winkel wird zurückgegeben
cvektor3 ermittlenahstenvektor (cvektor3 pkante, cvektor3liste& pvektorliste)
  {
  cvektor3 vecke= pvektorliste.v[0];
  real wimin= winkelb (vecke, pkante);
  real wi;
  for (integer lauf= 1; lauf < pvektorliste.anz; lauf++)
    {
    wi= winkelb (pvektorliste.v[lauf], pkante);
    if (wi < wimin)
      {
      vecke= pvektorliste.v[lauf];
      wimin= wi;
      }
    }
  return vecke;
  }

// ************************************************************** Visualisierungen *****************************************************************
// ------------------------ Körper aus Symmetriegruppe visualisieren -------------------------------------------------------------------------------

void visualisiereebenen (const cbasis3 pspiegelbasis)
  {
  // Spiegelebenen berechnen
  cbasis3 sp1, sp2, sp3;
  sp1.z= pspiegelbasis.x;
  sp1.x= senk (sp1.z);
  sp1.y= sp1.z^sp1.x;
  sp1= normiere (sp1);
  sp2.z= pspiegelbasis.y;
  sp2.x= senk (sp2.z);
  sp2.y= sp2.z^sp2.x;
  sp2= normiere (sp2);
  sp3.z= pspiegelbasis.z;
  sp3.x= senk (sp3.z);
  sp3.y= sp3.z^sp3.x;
  sp3= normiere (sp3);

  real kzoom= 100;

  //cbegrrechteck* begrkreis= new cbegrrechteck (-4, 4, -10, 0);       // Polarebene abstandstreu
  cbegrkreis* begrkreis= new cbegrkreis (-4, 4, 0, 1);               // Schachbrettebene

  // Ebenentexturen
  cschachfeld* textursp1= new cschachfeld (cvektor3 (0,1,1), cvektor3 (.2,.8,.8), 16/M_PIl, 16/M_PIl);
  cschachfeld* textursp2= new cschachfeld (cvektor3 (1,0,1), cvektor3 (1,.5,1), 16/M_PIl, 16/M_PIl);
  cschachfeld* textursp3= new cschachfeld (cvektor3 (1,1,0), cvektor3 (.8,.8,.2), 16/M_PIl, 16/M_PIl);

  //cvektor3 zentrivektor= -kzoom*normiere (pspiegelbasis.x + pspiegelbasis.y + pspiegelbasis.z);
  cvektor3 zentrivektor= -kzoom*cvektor3 (1, 1, 1);
  
  // Welt erschaffen
  cwelt welt (1920, 5*zentrivektor, einsbasis3);  // Bildschirmentfernung, Standpunkt, Lage
  welt.himmelfarbe= cvektor3 (.2, .2, .2);

  //addebenen90 (welt);
  addkoordinatensystem (welt);

  // Spiegelebenen hinzufügen
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp1, nullvektor3, kzoom*sp1));
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp2, nullvektor3, kzoom*sp2));
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp3, nullvektor3, kzoom*sp3));
  
  // Sichtpunkt einrichten
  cbasis3 augbasis;
  augbasis.z= -zentrivektor;
  augbasis.x= augbasis.z^zvektor3;
  augbasis.y= augbasis.z^augbasis.x;
  augbasis= normiere (augbasis);
  welt.setzelage (augbasis);
    
  // Flugsimulator starten
  cflugsimu flugsimu (&welt, new cxkeyboard, new cxscreen ("Drehgruppe", 600, 600));
  flugsimu.threadanz= 16;
  flugsimu.framerate= 10;
  flugsimu.fliege ();
  }

void visualisiereexakt (cbasis3liste& pbliste, const cbasis3 pspiegelbasis)
  {
  ccluster cluster (1000);
  cluster.gruppe->kopiere (pbliste);
  cluster.vervollstaendige ();
  printf ("Cluster.gruppe\n");
  printb3liste (*cluster.gruppe);
  printf ("Cluster.partliste\n");
  printv3liste (*cluster.partliste);
  printf ("Cluster.spieg\n");
  printb3liste (*cluster.spieg);
  
  printf ("Kantenliste\n");
  printv3liste (*cluster.kliste);
  printf ("Eckenliste\n");
  printv3liste (*cluster.eliste);
  printf ("Flächenliste\n");
  printv3liste (*cluster.fliste);
  
  // Ecken und Flächen vertauschen
  cvektor3liste* swapliste;
  swapliste= cluster.eliste;
  cluster.eliste= cluster.fliste;
  cluster.fliste=swapliste;
  
  // Lagenarray der Kanten erzeugen
  cbasis3* klagenarray= new cbasis3[lmax];
  cbasis3 klage;
  cvektor3 kante;
  cvektor3 kurzkante;
  cvektor3 kvecke, kvecke2;
  real kantenlaenge;
  real kantendicke= 3;
  for (integer klauf= 0; klauf < cluster.kliste->anz; klauf++)
    {
    kante= cluster.kliste->v[klauf];
    kvecke= ermittlenahstenvektor (kante, *cluster.eliste);
    kvecke2= cvektor4 (0, kante.x, kante.y, kante.z)*kvecke;    // zweiten Eckenvektor ermitteln durch Quaternionenmultiplikation (180° Drehung = Spiegelung)
    klage.z= kvecke - kvecke2;                                  // z-Achse der Lagenbasis nach Kantenvektor (Differenz der Eckenvektoren) ausrichten
    //klage.z= kvecke^kvecke2;                                  // senkrecht, dualer Körper
    klage.x= cluster.kliste->v[klauf];
    klage.y= klage.z^klage.x;
    klage= normiere (klage);
    klagenarray[klauf]= kantendicke*klage;                      // Dicke der Kanten einstellen
    }
  kantenlaenge= 0.5*abs (kvecke - kvecke2);
  kurzkante= 0.5*(kvecke + kvecke2);                            // Kantenvektor zeigt auf Kantenmittelpunkt statt dahinter

  // Körperstandpunkte
  cvektor3 kstursprung (0,0,0);

  // Körperlagen
  cbasis3 klebene (cvektor3 (10,0,0), cvektor3 (0,10,0), cvektor3 (0,0,10));
  cbasis3 klzylinder (cvektor3 (1,0,0), cvektor3 (0,1,0), cvektor3 (0,0,1));
  klzylinder= 5*klzylinder;
  cbasis3 klmitte (cvektor3 (100,0,0), cvektor3 (0,100,0), cvektor3 (0,0,100));

  // Begrenzungen anlegen
  real zbegrenzung= 100*kantenlaenge/kantendicke;
  cbegrrechteck* begrrecht= new cbegrrechteck (-4, 4, -zbegrenzung, zbegrenzung);        // Länge der Kanten einstellen

  // Texturen anlegen
  cschachfeld* textur2= new cschachfeld (cvektor3 (1,0,1), cvektor3 (1,1,0), 10/M_PIl, 10/M_PIl);

  // Körper addieren
  real kzoom= 100;

  // Welt erschaffen
  cwelt welt (1920, cvektor3 (100, 100, -1920), einsbasis3);  // Bildschirmentfernung, Standpunkt, Lage
  welt.himmelfarbe= cvektor3 (.2, .2, .2);

  //addebenen90 (welt);
  addkoordinatensystem (welt);

  // Kantenzylinder hinzufügen
  for (integer lauf= 0; lauf < cluster.kliste->anz; lauf++)
    welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrrecht, textur2, kzoom*abs (kurzkante)*cluster.kliste->v[lauf], klagenarray[lauf]));

  //cbegrrechteck* begrkreis= new cbegrrechteck (-4, 4, -10, 0);       // Polarebene abstandstreu
  cbegrkreis* begrkreis= new cbegrkreis (-4, 4, 0, 1);               // Schachbrettebene

  // Flugsimulator starten
  cflugsimu flugsimu (&welt, new cxkeyboard, new cxscreen ("Drehgruppe", 600, 600));
  flugsimu.threadanz= 16;
  flugsimu.framerate= 10;
  flugsimu.fliege ();
  }

void visualisierekantenmodell (cbasis3liste& pbliste, cbasis3 pspiegelbasis)
  {
  ccluster cluster (1000);
  cluster.gruppe->kopiere (pbliste);
  cluster.vervollstaendige ();
  printf ("Cluster.gruppe\n");
  printb3liste (*cluster.gruppe);
  printf ("Cluster.partliste\n");
  printv3liste (*cluster.partliste);
  printf ("Cluster.spieg\n");
  printb3liste (*cluster.spieg);
  
  // Ecken und Flächen vertauschen
  cvektor3liste* swapliste;
  swapliste= cluster.eliste;
  cluster.eliste= cluster.fliste;
  cluster.fliste=swapliste;
  
  printf ("Kantenliste\n");
  printv3liste (*cluster.kliste);
  printf ("Eckenliste\n");
  printv3liste (*cluster.eliste);
  printf ("Flächenliste\n");
  printv3liste (*cluster.fliste);
  
  // Spiegelebenen berechnen
  cbasis3 sp1, sp2, sp3;
  sp1.z= pspiegelbasis.x;
  sp1.x= senk (sp1.z);
  sp1.y= sp1.z^sp1.x;
  sp1= normiere (sp1);
  sp2.z= pspiegelbasis.y;
  sp2.x= senk (sp2.z);
  sp2.y= sp2.z^sp2.x;
  sp2= normiere (sp2);
  sp3.z= pspiegelbasis.z;
  sp3.x= senk (sp3.z);
  sp3.y= sp3.z^sp3.x;
  sp3= normiere (sp3);

  // Lagenarray der Kanten erzeugen
  cbasis3* klagenarray= new cbasis3[lmax];
  cbasis3 klage;
  cvektor3 kante;
  cvektor3 kurzkante;
  cvektor3 kvecke, kvecke2;
  real kantenlaenge;
  real kantendicke= 3;
  for (integer klauf= 0; klauf < cluster.kliste->anz; klauf++)
    {
    kante= cluster.kliste->v[klauf];
    kvecke= ermittlenahstenvektor (kante, *cluster.eliste);
    kvecke2= cvektor4 (0, kante.x, kante.y, kante.z)*kvecke;    // zweiten Eckenvektor ermitteln durch Quaternionenmultiplikation (180° Drehung = Spiegelung)
    klage.z= kvecke - kvecke2;                                  // z-Achse der Lagenbasis nach Kantenvektor (Differenz der Eckenvektoren) ausrichten
    //klage.z= kvecke^kvecke2;                                  // senkrecht, dualer Körper
    klage.x= cluster.kliste->v[klauf];
    klage.y= klage.z^klage.x;
    klage= normiere (klage);
    klagenarray[klauf]= kantendicke*klage;                      // Dicke der Kanten einstellen
    }
  kantenlaenge= 0.5*abs (kvecke - kvecke2);
  kurzkante= 0.5*(kvecke + kvecke2);                            // Kantenvektor zeigt auf Kantenmittelpunkt statt dahinter

  // Lagenarray der Flächen erzeugen
  cbasis3* flagenarray= new cbasis3[lmax];
  cbasis3 flage;
  cvektor3 vflaeche;
  for (integer klauf= 0; klauf < cluster.fliste->anz; klauf++)
    {
    flage.z= cluster.fliste->v[klauf];
    flage.x= senk (flage.z);
    flage.y= flage.z^flage.x;
    flage= normiere (flage);
    flagenarray[klauf]= 11*flage;                               // Größe der Flächen einstellen
    }

  // Welt erschaffen
  cwelt welt (1920, cvektor3 (0, 0, -1920), einsbasis3);        // Bildschirmentfernung, Standpunkt, Lage
  welt.himmelfarbe= cvektor3 (0, .3, .5);

  // Körperstandpunkte
  cvektor3 kstursprung (0,0,0);

  // Körperlagen
  cbasis3 klebene (cvektor3 (10,0,0), cvektor3 (0,10,0), cvektor3 (0,0,10));
  cbasis3 klzylinder (cvektor3 (1,0,0), cvektor3 (0,1,0), cvektor3 (0,0,1));
  klzylinder= 5*klzylinder;
  cbasis3 klmitte (cvektor3 (100,0,0), cvektor3 (0,100,0), cvektor3 (0,0,100));

  // Begrenzungen anlegen
  real zbegrenzung= 100*kantenlaenge/kantendicke;
  cbegrrechteck* begrrecht= new cbegrrechteck (-4, 4, -zbegrenzung, zbegrenzung);        // Länge der Kanten einstellen

  // Texturen anlegen
  cschachfeld* textur1= new cschachfeld (cvektor3 (1,1,1), cvektor3 (.3,.3,.3), 10/M_PIl, 10/M_PIl);
  cschachfeld* textur2= new cschachfeld (cvektor3 (1,0,1), cvektor3 (1,1,0), 10/M_PIl, 10/M_PIl);
  cschachfeld* textur3= new cschachfeld (cvektor3 (1,0,0), cvektor3 (0,1,0), 10/M_PIl, 10/M_PIl);

  // Körper addieren
  real kzoom= 100;

  // Bewegungskugel hinzufügen
  ckoerper* bewkugel= new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, nullvektor3, 0.1*kzoom*einsbasis3);
  bewkugel->koerperpos= kzoom*cluster.eliste->v[0];            // Bewegungskugel liegt auf einer Ecke
  //bewkugel->koerperpos= kzoom*abs (kurzkante)*cluster.kliste->v[0];      // Bewegungskugel liegt auf einer Kante
  //bewkugel->koerperpos= kzoom/sqrtl (3)*cluster.fliste->v[0];            // Bewegungskugel liegt auf einer Fläche
  bewkugel->startpos= bewkugel->koerperpos;
  bewkugel->stoppos= bewkugel->startpos;
  bewkugel->startzeit= 0;
  bewkugel->stopzeit= 0;
  welt.addkoerper (bewkugel);
  
  // Kantenzylinder hinzufügen
  for (integer lauf= 0; lauf < cluster.kliste->anz; lauf++)
    welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrrecht, textur2, kzoom*abs (kurzkante)*cluster.kliste->v[lauf], klagenarray[lauf]));
   
  // Spiegelungen paarweise multiplizieren
  cbasis3 drehachsen= drehachsenvonspiegelungen (pspiegelbasis);
  cbasis3 dr1, dr2, dr3;

  dr1.z= drehachsen.x;
  dr1.x= senk (dr1.z);
  dr1.y= dr1.z*dr1.x;
  dr1= normiere (dr1);
  dr2.z= drehachsen.y;
  dr2.x= senk (dr2.z);
  dr2.y= dr2.z*dr2.x;
  dr2= normiere (dr2);
  dr3.z= drehachsen.z;
  dr3.x= senk (dr3.z);
  dr3.y= dr3.z*dr3.x;
  dr3= normiere (dr3);
  
  // Drehachsen hinzufügen
  //welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, new cbegrkeine, textur3, nullvektor3, dr1));
  //welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, new cbegrkeine, textur3, nullvektor3, dr2));
  //welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, new cbegrkeine, textur3, nullvektor3, dr3));

  //cbegrrechteck* begrkreis= new cbegrrechteck (-4, 4, -10, 0);       // Polarebene abstandstreu
  cbegrkreis* begrkreis= new cbegrkreis (-4, 4, 0, 1);               // Schachbrettebene

  // Ebenentexturen
  cschachfeld* textursp1= new cschachfeld (cvektor3 (1,0,0.5), cvektor3 (1,.5,0), 16/M_PIl, 16/M_PIl);
  cschachfeld* textursp2= new cschachfeld (cvektor3 (0.6,.7,0), cvektor3 (0,.7,.6), 16/M_PIl, 16/M_PIl);
  cschachfeld* textursp3= new cschachfeld (cvektor3 (0,0.5,1), cvektor3 (.5,0,1), 16/M_PIl, 16/M_PIl);

  // Spiegelebenen hinzufügen
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp1, nullvektor3, kzoom*sp1));
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp2, nullvektor3, kzoom*sp2));
  welt.addkoerper (new ckoerper (new csebene, new cparawebene, begrkreis, textursp3, nullvektor3, kzoom*sp3));
  
  // Sichtpunkt auf kleinstes Spiegeldreieck
  cbasis3 senkbasis (~pspiegelbasis);
  senkbasis.x = normiere (senkbasis.x);
  senkbasis.y = normiere (senkbasis.y);
  senkbasis.z = normiere (senkbasis.z);
  cvektor3 zentrivektor (normiere (senkbasis.x + senkbasis.y + senkbasis.z));
  zentrivektor= pspiegelbasis.x + pspiegelbasis.y + pspiegelbasis.z;

  // Sichtpunkt einrichten
  cbasis3 augbasis;
  augbasis.y= zentrivektor;
  augbasis.x= senk (augbasis.y);
  augbasis.z= augbasis.x^augbasis.y;
  augbasis= normiere (augbasis);

  welt.setzeposition (-1040*augbasis.z);
  //welt.setzeposition (400*zentrivektor);
  welt.setzelage (augbasis);
    
  // Flugsimulator starten
  cflugsimu flugsimu (&welt, new cxkeyboard, new cxscreen ("Drehgruppe", 600, 600));
  //cflugsimu flugsimu (&welt, new cxkeyboard, new cbmpdatei ("Drehgruppe.bmp", 3840, 2160));
  flugsimu.threadanz= 16;
  flugsimu.framerate= 10;
  flugsimu.fliegespieltakt (pspiegelbasis, bewkugel);
  }

// ----------------------------------------------------- Fundamentalbereiche visualisieren ---------------------------------
void visualisierefundamentalbereiche (cbasis3liste& pbliste, const cbasis3 pspiegelbasis)
  {
  ccluster cluster (1000);
  cluster.gruppe->kopiere (pbliste);
  cluster.vervollstaendige ();
  printf ("Cluster.gruppe\n");
  printb3liste (*cluster.gruppe);
  printf ("Cluster.partliste\n");
  printv3liste (*cluster.partliste);
  printf ("Cluster.spieg\n");
  printb3liste (*cluster.spieg);
  
  // Kanten, Ecken, Flächen liste Ortsvektoren erzeugen
  cvektor3 kantenvektor;
  kantenvektor= cluster.partliste->v[2];
  cvektor3liste kantenliste (lmax);
  kantenliste.elementhinzu (kantenvektor, 0);
  kantenliste.schliesseab (*cluster.kern);

  cvektor3 eckenvektor;
  eckenvektor= cluster.partliste->v[1];
  cvektor3liste eckenliste (lmax);
  eckenliste.elementhinzu (eckenvektor, 0);
  eckenliste.schliesseab (*cluster.kern);

  cvektor3 flaechenvektor;
  flaechenvektor= cluster.partliste->v[0];
  cvektor3liste flaechenliste (lmax);
  flaechenliste.elementhinzu (flaechenvektor, 0);
  flaechenliste.schliesseab (*cluster.kern);

  printf ("Kantenliste\n");
  printv3liste (kantenliste);
  printf ("Eckenliste\n");
  printv3liste (eckenliste);
  printf ("Flächenliste\n");
  printv3liste (flaechenliste);
    
  // Lagenarray der Kanten erzeugen
  cbasis3* klagenarray= new cbasis3[lmax];
  cbasis3 klage;
  cvektor3 kante;
  cvektor3 kurzkante;
  cvektor3 kvecke, kvecke2;
  real kantenlaenge;
  real kantendicke= 3;
  for (integer klauf= 0; klauf < kantenliste.anz; klauf++)
    {
    kante= kantenliste.v[klauf];
    kvecke= ermittlenahstenvektor (kante, eckenliste);
    kvecke2= cvektor4 (0, kante.x, kante.y, kante.z)*kvecke;    // zweiten Eckenvektor ermitteln durch Quaternionenmultiplikation (180° Drehung = Spiegelung)
    klage.z= kvecke - kvecke2;                                  // z-Achse der Lagenbasis nach Kantenvektor (Differenz der Eckenvektoren) ausrichten
    //klage.z= kvecke^kvecke2;                                  // senkrecht, dualer Körper
    klage.x= kantenliste.v[klauf];
    klage.y= klage.z^klage.x;
    klage= normiere (klage);
    klagenarray[klauf]= kantendicke*klage;                      // Dicke der Kanten einstellen
    }
  kantenlaenge= 0.5*abs (kvecke - kvecke2);
  kurzkante= 0.5*(kvecke + kvecke2);                            // Kantenvektor zeigt auf Kantenmittelpunkt statt dahinter

  // Lagenarray der Flächen erzeugen
  cbasis3* flagenarray= new cbasis3[lmax];
  cbasis3 flage;
  cvektor3 vflaeche;
  for (integer klauf= 0; klauf < flaechenliste.anz; klauf++)
    {
    flage.z= flaechenliste.v[klauf];
    flage.x= senk (flage.z);
    flage.y= flage.z^flage.x;
    flage= normiere (flage);
    flagenarray[klauf]= 11*flage;                               // Größe der Flächen einstellen
    }

  // Welt erschaffen
  cwelt welt (1920, cvektor3 (0, 0, -1000), cbasis3 (1));       // Bildschirmentfernung, Standpunkt, Lage

  // Körperstandpunkte
  cvektor3 kstursprung (0,0,0);

  // Körperlagen
  cbasis3 klebene (cvektor3 (10,0,0), cvektor3 (0,10,0), cvektor3 (0,0,10));
  cbasis3 klzylinder (cvektor3 (1,0,0), cvektor3 (0,1,0), cvektor3 (0,0,1));
  klzylinder= 5*klzylinder;
  cbasis3 klmitte (cvektor3 (100,0,0), cvektor3 (0,100,0), cvektor3 (0,0,100));

  // Begrenzungen anlegen
  real zbegrenzung= 100*kantenlaenge/kantendicke;
  cbegrrechteck* begrrecht= new cbegrrechteck (-4, 4, -zbegrenzung, zbegrenzung);        // Länge der Kanten einstellen

  // Texturen anlegen
  cmonochrom* textur1= new cmonochrom (cvektor3 (.5, .5, .5));
  cschachfeld* textur2= new cschachfeld (cvektor3 (1,0,1), cvektor3 (1,1,0), 100/M_PIl, 10/M_PIl);
  cschachfeld* textur4= new cschachfeld (cvektor3 (0,1,0), cvektor3 (0,1,1), 100/M_PIl, 10/M_PIl);
  cmonochrom* textur3= new cmonochrom (cvektor3 (.5, 1, 0));

  // Körper addieren
  real kzoom= 100;

  // Fundametalbereiche zeigen
  cbegrrechteck* begreb= new cbegrrechteck (-4, 4, 1-.05, 1.05);
  cbegrrechteck* begrzyl= new cbegrrechteck (-4, 4, -.05, .05);
  for (integer lauf= 0; lauf < cluster.spiegkoord->anz; lauf+= 1)
    {
    welt.addkoerper (new ckoerper (new cszylinder, new cparawzylinder, begrzyl, textur2, nullvektor3, 1.201*kzoom*cluster.spiegkoord->b[lauf]));
    welt.addkoerper (new ckoerper (new csebene,    new cparapolebene,  begreb,  textur4, nullvektor3, 1.201*kzoom*cluster.spiegkoord->b[lauf]));
    }
  printb3liste (*cluster.spiegkoord);

  // Mittenkugel hinzufügen
  welt.addkoerper (new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, nullvektor3, 1.2*kzoom*einsbasis3));

  // Flugsimulator starten
  cflugsimu flugsimu (&welt, new cxkeyboard, new cxscreen ("Drehgruppe", 600, 600));
  flugsimu.threadanz= 16;
  flugsimu.framerate= 10;
  flugsimu.fliege ();
  }

//********************************************************** grafikfreie Funktionen ***********************************************************************

//------------------------------------------------------------- Hilfsfunktionen ----------------------------------------------------------

void zeigewinkel (const cbasis3& pdreibein)
  {
  cvektor3 pwi;
  pwi.x= winkelb (pdreibein.x, pdreibein.y);
  pwi.y= winkelb (pdreibein.x, pdreibein.z);
  pwi.z= winkelb (pdreibein.y, pdreibein.z);
  pwi.x*= 180/M_PIl;
  pwi.y*= 180/M_PIl;
  pwi.z*= 180/M_PIl;
  real det3b= det (pdreibein);
  printf ("%16.10LF %16.10LF %16.10LF Determinante: %16.10LF\n", pwi.x, pwi.y, pwi.z, det3b);
  }

void zeigedreibein (const cbasis3& pdreibein)
  {
  printf ("Spiegelachsen:\n");
  printvektor3ord (pdreibein.x, 0);
  printvektor3ord (pdreibein.y, 0);
  printvektor3ord (pdreibein.z, 0);
  printf ("\n");

  printf ("Winkel zwischen den Spiegelachsen:\n");
  cbasis3 dreibein= pdreibein;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  dreibein.y= -dreibein.y;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  dreibein.y= -dreibein.y;
  dreibein.z= -dreibein.z;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  dreibein.y= -dreibein.y;
  zeigewinkel (dreibein);
  dreibein.x= -dreibein.x;
  zeigewinkel (dreibein);
  printf ("\n");

  cvektor3 iwi;
  iwi.x= winkelb (pdreibein.x^pdreibein.y, -pdreibein.y^pdreibein.z);
  iwi.y= winkelb (pdreibein.y^pdreibein.z, -pdreibein.z^pdreibein.x);
  iwi.z= winkelb (pdreibein.z^pdreibein.x, -pdreibein.x^pdreibein.y);
  iwi.x*= 180/M_PIl;
  iwi.y*= 180/M_PIl;
  iwi.z*= 180/M_PIl;
  real iws= iwi.x + iwi.y + iwi.z;
  printf ("Innenwinkel des Dreibeins:\n %16.10LF %16.10LF %16.10LF    Innenwinkelsumme: %16.10LF\n", iwi.x, iwi.y, iwi.z, iws);
  printf ("\n");

  cvektor3 zv= normiere (pdreibein.x + pdreibein.y + pdreibein.z);
  printf ("Zentrivektor:\n");
  printvektor3ord (zv, 0);
  printf ("\n");

  cvektor3 zwi;
  zwi.x= winkelb (zv, pdreibein.x);
  zwi.y= winkelb (zv, pdreibein.y);
  zwi.z= winkelb (zv, pdreibein.z);
  zwi.x*= 180/M_PIl;
  zwi.y*= 180/M_PIl;
  zwi.z*= 180/M_PIl;
  printf ("Zentriwinkel zwischen Zentrivektor und den Spiegelachsen:\n %16.10LF %16.10LF %16.10LF\n", zwi.x, zwi.y, zwi.z);
  printf ("\n");

  cbasis3liste spiegeldrehungen (3);
  drehungenvonspiegelungen (spiegeldrehungen, pdreibein);
  printf ("von den Spiegelungen abgeleitetes Drehsystem:\n");
  printbasis3dreh (spiegeldrehungen.b[0], 0, 0, 0);
  printbasis3dreh (spiegeldrehungen.b[1], 0, 0, 0);
  printbasis3dreh (spiegeldrehungen.b[2], 0, 0, 0);
  printf ("\n");

  cvektor4 wiachs1= (winkelachsefrommatrix (spiegeldrehungen.b[0]));
  cvektor4 wiachs2= (winkelachsefrommatrix (spiegeldrehungen.b[1]));
  cvektor4 wiachs3 (winkelachsefrommatrix (spiegeldrehungen.b[2]));
  cvektor3 achse1= orientiere (normiere (cvektor3 (wiachs1.i, wiachs1.j, wiachs1.ij)));
  cvektor3 achse2= orientiere (normiere (cvektor3 (wiachs2.i, wiachs2.j, wiachs2.ij)));
  cvektor3 achse3 (orientiere (normiere (cvektor3 (wiachs3.i, wiachs3.j, wiachs3.ij))));
  real wid1 (180/M_PIl*winkelg (achse1, achse2));
  real wid2 (180/M_PIl*winkelg (achse1, achse3));
  real wid3 (180/M_PIl*winkelg (achse2, achse3));
  printf ("Winkel zwischen den Drehachsen (xy,xz,yz): %9.3Lf°%9.3Lf°%9.3Lf°\n", wid1, wid2, wid3);
  }

//--------------------------------------  1 Drehspiegelung -------------------------------------------------

void dreh1 ()
  {
  cvektor4 el;
  cvektor4liste qliste (lmax);
  cbasis3liste bliste (lmax);
  cvektor3liste vliste (lmax);

  el= winkelachseeingabe ();
  qliste.winkelachsehinzu (el);
  bliste.winkelachsehinzu (el);
  qliste.schliesseab ();
  bliste.schliesseab ();

  printv4liste (qliste);
  printb3liste (bliste);

  vliste.elementhinzu (cvektor3 (1, 0, 0), 1);
  vliste.schliesseab (bliste);
  printv3liste (vliste);
  }

//-------------------------------------- 2 Drehspiegelungen ------------------------------------------------

void dreh2 ()
  {
  cvektor4 el1, el2;
  cbasis3liste bliste (lmax);

  el1= winkelachseeingabe ();
  el2= winkelachseeingabe ();
  bliste.winkelachsehinzu (el1);
  bliste.winkelachsehinzu (el2);
  getchar ();

  visualisiereexakt (bliste, einsbasis3);
  //visualisierekantenmodell (bliste, einsbasis3);
  }

//-------------------------------------- 3 Drehspiegelungen ------------------------------------------------

void dreh3 ()
  {
  cvektor4 el1, el2, el3;
  cvektor4liste qliste (lmax);
  cbasis3liste bliste (lmax);

  el1= winkelachseeingabe ();
  el2= winkelachseeingabe ();
  el3= winkelachseeingabe ();
  qliste.winkelachsehinzu (el1);
  qliste.winkelachsehinzu (el2);
  qliste.winkelachsehinzu (el3);
  bliste.winkelachsehinzu (el1);
  bliste.winkelachsehinzu (el2);
  bliste.winkelachsehinzu (el3);
  qliste.schliesseab ();
  bliste.schliesseab ();
  if (!qliste.uebergelaufen)
    {
    qliste.ermittleordnung ();
    qliste.sortierenachordnung ();
    }
  if (!bliste.uebergelaufen)
    {
    bliste.ermittleordnung ();
    bliste.sortierenachordnung ();
    }

  printv4liste (qliste);
  getchar ();
  getchar ();
  printb3liste (bliste);
  getchar ();

  visualisiereexakt (bliste, einsbasis3);
  //visualisierekantenmodell (bliste, einsbasis3);
  }

//-------------------------------------- 3 Ebenenwinkel Ebenenrausdrehung ------------------------------------------------
// Ebenenwinkel in 180° Drehungen umrechnen
void ebenenwinkel3e ()
  {
  cvektor3 wi;
  cbasis3 sachsen;
  cvektor4 el1, el2, el3;
  cbasis3liste bliste (lmax);
  cbasis3liste kern (lmax);
  cvektor3liste partliste (lmax);

  // Winkel zwischen den Spiegelebenen eingeben
  vektor3eingabe (wi);
  wi.x= wi.x*M_PIl/180;
  wi.y= wi.y*M_PIl/180;
  wi.z= wi.z*M_PIl/180;
  
  // Spiegelachsen aus Ebenenwinkel ermitteln
  sachsen= spiegelachsenvonebenenwinkelk (wi);

  zeigedreibein (sachsen);
  getchar ();
  getchar ();

  // Spiegelmatrizen aus Spiegelachsen bilden
  el1= cvektor4 (M_PIl, sachsen.x);
  el2= cvektor4 (M_PIl, sachsen.y);
  el3= cvektor4 (M_PIl, sachsen.z);
  
  // Spiegelmatrizen zur Gruppe hinzufügen
  bliste.winkelachseihinzu (el1);
  bliste.winkelachseihinzu (el2);
  bliste.winkelachseihinzu (el3);

  visualisierekantenmodell (bliste, sachsen);
  //visualisierefundamentalbereiche (bliste, sachsen);
  }

//-------------------------------------- 3 Dreieckswinkel ------------------------------------------------
// Dreieckswinkel in 180° Drehungen umrechnen
void ebenenwinkel3d ()
  {
  cvektor3 wi;
  cbasis3 sachsen;
  cvektor4 el1, el2, el3;
  cbasis3liste bliste (lmax);
  cbasis3liste kern (lmax);
  cvektor3liste partliste (lmax);

  // Winkel zwischen den Spiegelebenen eingeben
  vektor3eingabe (wi);
  wi.x= wi.x*M_PIl/180;
  wi.y= wi.y*M_PIl/180;
  wi.z= wi.z*M_PIl/180;
  
  // Spiegelachsen aus Ebenenwinkel ermitteln
  sachsen= spiegelachsenvonebenenwinkeld (wi);

  zeigedreibein (sachsen);
  getchar ();
  getchar ();

  // Spiegelmatrizen aus Spiegelachsen bilden
  el1= cvektor4 (M_PIl, sachsen.x);
  el2= cvektor4 (M_PIl, sachsen.y);
  el3= cvektor4 (M_PIl, sachsen.z);
  
  // Spiegelmatrizen zur Gruppe hinzufügen
  bliste.winkelachseihinzu (el1);
  bliste.winkelachseihinzu (el2);
  bliste.winkelachseihinzu (el3);

  visualisierekantenmodell (bliste, sachsen);
  //visualisierefundamentalbereiche (bliste, sachsen);
  }

//-------------------------------------- 3 Ebenen anzeigen ------------------------------------------------
// Ebenenwinkel in 180° Drehungen umrechnen
void zeigeebenen ()
  {
  cvektor3 wi;
  cbasis3 sachsen;

  // Winkel zwischen den Spiegelebenen eingeben
  vektor3eingabe (wi);
  wi.x= wi.x*M_PIl/180;
  wi.y= wi.y*M_PIl/180;
  wi.z= wi.z*M_PIl/180;
  
  // Spiegelachsen aus Ebenenwinkel ermitteln
  sachsen= spiegelachsenvonebenenwinkelk (wi);

  zeigedreibein (sachsen);
  getchar ();
  getchar ();

  visualisiereebenen (sachsen);
  }

//-------------------------------------- Entscheidungseingabe ------------------------------------------------
integer entscheidungseingabe ()
  {
  integer scanret;
  integer ent;
  printf ("\n");
  printf ("[1] 2 Drehspiegelungen [2] 3 Drehspiegelungen [3] 3 Ebenenwinkel  [4] 3 Dreieckswinkel [5] 3 Ebenen anzeigen\n");
  scanret= scanf ("%lld", &ent);
  scanret++;                              //scanret benutzen damit Compiler nicht meckert
  return ent;
  }

//******************************************************************* Hauptprogramm ********************************************************************
int main ()
  {
  integer ent (entscheidungseingabe ());
  switch (ent)
    {
    case 1:
      dreh2 ();
      break;
    case 2:
      dreh3 ();
      break;
    case 3:
      ebenenwinkel3e ();
      break;
    case 4:
      ebenenwinkel3d ();
      break;
    case 5:
      zeigeebenen ();
      break;
    }
  return 0;
  }
