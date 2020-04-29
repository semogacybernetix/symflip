#include "gruppe.h"

#include <iostream>  // printf zum debuggen

real gquant= 1e-8;

//------------------------ Gruppenfunktionen ---------------------------------------------------------------------------------

cbasis3 drehachsenvonspiegelungen (const cbasis3& pb)
  {
  // Spiegelungsmatrizen berechnen
  cbasis3 sp1 (getspiegbasis (pb.x));
  cbasis3 sp2 (getspiegbasis (pb.y));
  cbasis3 sp3 (getspiegbasis (pb.z));

  // Spiegelungen paarweise multiplizieren
  cbasis3 dr1 (sp2*sp3);
  cbasis3 dr2 (sp3*sp1);
  cbasis3 dr3 (sp1*sp2);

  // Winkel und Achsen der Produkte berechnen
  cvektor4 wa1 (winkelachsefrommatrix (dr1));
  cvektor4 wa2 (winkelachsefrommatrix (dr2));
  cvektor4 wa3 (winkelachsefrommatrix (dr3));

  // Achsen isolieren
  cvektor3 dreh1 (cvektor3 (wa1.i, wa1.j, wa1.ij));
  cvektor3 dreh2 (cvektor3 (wa2.i, wa2.j, wa2.ij));
  cvektor3 dreh3 (cvektor3 (wa3.i, wa3.j, wa3.ij));

  return cbasis3 (dreh1, dreh2, dreh3);
  }

// 3 Spiegelachsen (pb.x, pb.y, pb.z) in 2 Drehmatrizen umrechnen. Es werden 3 Drehmatrizen zurückgegeben, damit man sich eines der 3 (Paare von 2 Drehmatrizen) aussuchen kann
void drehungenvonspiegelungen (cbasis3liste& pliste, const cbasis3& pb)
  {
  // Spiegelungsmatrizen berechnen
  cbasis3 sp1 (getspiegbasis (pb.x));
  cbasis3 sp2 (getspiegbasis (pb.y));
  cbasis3 sp3 (getspiegbasis (pb.z));

  // Spiegelungen paarweise multiplizieren
  cbasis3 dr1 (sp1*sp2);
  cbasis3 dr2 (sp1*sp3);
  cbasis3 dr3 (sp2*sp3);

  // Winkel und Achsen der Produkte berechnen
  cvektor4 wa1 (winkelachsefrommatrix (dr1));
  cvektor4 wa2 (winkelachsefrommatrix (dr2));
  cvektor4 wa3 (winkelachsefrommatrix (dr3));

  // Achsen isolieren
  cvektor3 dreh1 (cvektor3 (wa1.i, wa1.j, wa1.ij));
  cvektor3 dreh2 (cvektor3 (wa2.i, wa2.j, wa2.ij));
  cvektor3 dreh3 (cvektor3 (wa3.i, wa3.j, wa3.ij));

  // Achsen orientieren (erste von 0 verschiedene Koordinate muss positiv sein, wenn nicht, dann den Vektor invertieren)
  dreh1= orientiere (dreh1);
  dreh2= orientiere (dreh2);
  dreh3= orientiere (dreh3);

  // Drehmatrizen aus orientierten Achsen wieder zusammensetzen
  pliste.elementhinzu (matrixfromwinkelachse (cvektor4 (wa1.r, dreh1.x, dreh1.y, dreh1.z)));
  pliste.elementhinzu (matrixfromwinkelachse (cvektor4 (wa2.r, dreh2.x, dreh2.y, dreh2.z)));
  pliste.elementhinzu (matrixfromwinkelachse (cvektor4 (wa3.r, dreh3.x, dreh3.y, dreh3.z)));
  }

void spiegelungenvondrehungen (cbasis3liste& pliste, const cvektor4 pwa1, const cvektor4 pwa2)           // 2 Quaternionendrehungen in 3 Spiegelachsen umrechnen
  {
  // Drehmatrizen aus den übergebenen Drehungen erstellen, aber mit halben Drehwinkel
  cbasis3 sdreh1 (matrixfromwinkelachse (cvektor4 (pwa1.r*0.5, pwa1.i, pwa1.j, pwa1.ij)));
  cbasis3 sdreh2 (matrixfromwinkelachse (cvektor4 (pwa2.r*0.5, pwa2.i, pwa2.j, pwa2.ij)));

  // Schnittachse der beiden Drehebenen der übergebenen Drehungen bestimmen
  cvektor3 sachse1 (cvektor3 (pwa1.i, pwa1.j, pwa1.ij)^cvektor3 (pwa2.i, pwa2.j, pwa2.ij));

  // Schnittachse mit halbem Winkel jeweils drehen
  cvektor3 sachse2 (sdreh1*sachse1);
  cvektor3 sachse3 (sdreh2*sachse1);

  // Schnittachse und gedrehte Achsen zur Spiegelmatrizenliste hinzufügen
  pliste.elementhinzu (getspiegbasis (sachse1));
  pliste.elementhinzu (getspiegbasis (sachse2));
  pliste.elementhinzu (getspiegbasis (sachse3));
  }

cbasis3 spiegelachsenvonebenenwinkele (const cvektor3 wi)  // über gleichschenkliges Dreieck, Außenwinkel
  {
  // Grenzfälle: a, a, 0 a[0..180°], 180, a, 180-a a[0..180°] werden korrekt behandelt
  // wi.x [0..360] w.y [0..180] wi.z [0..180]

  real ebx= cosl (0.5*wi.x);        //  wi.x < 180°
  real eby= sinl (0.5*wi.x);        //  wi.x > 0°

  // Vektoren haben die Länge 1
  cvektor3 spieg1 (ebx, -eby, 0);          // gleichschenkliges Dreieck in der xy-Ebene entlang der x-Achse mit Winkel wi.x bilden, wi.x [0°-180°]
  cvektor3 spieg2 (ebx, eby, 0);

  cvektor3 spieg3;
  real viy= cosl (wi.y);
  real viz= cosl (wi.z);

  if (ebx >= gquant)
    spieg3.x= 0.5*(viy + viz)/ebx;        // dritte Spiegelachse über Skalarprodukte ausrechnen
    else
    spieg3.x= 0;                          // wi.x = 180°, wi.y + wi.z = 180° wi.y [0..180°]

   if (eby >= gquant)
    spieg3.y= 0.5*(viy - viz)/eby;
    else
    spieg3.y= 0;                          // wi.x = 0°, wi.y = wi.z, wi.y [0..180°]

  spieg3.z= 1 - spieg3.x*spieg3.x - spieg3.y*spieg3.y;  // 2 Schnittpunkte der Kreise, negativer Wert = positive Determinante

  if (spieg3.z > -gquant)
    spieg3.z= sqrtl (fabsl (spieg3.z));
    else
    spieg3.z= 0;                          // Dreiecksungleichung ist nicht erfüllt
  return cbasis3 (spieg1, spieg2, spieg3);
  }

cbasis3 spiegelachsenvonebenenwinkeld (const cvektor3 wi)  // über gleichschenkliges Dreieck, Innenwinkel
  {
  // Grenzfälle: a, a, 0 a[0..180°], 180, a, 180-a a[0..180°] werden korrekt behandelt
  // wi.x [0..360] w.y [0..180] wi.z [0..180]
  real wix= M_PIl - wi.x;
  real wiy= M_PIl - wi.y;
  real wiz= M_PIl - wi.z;
  
  real ebx= cosl (0.5*wix);        //  wi.x < 180°
  real eby= sinl (0.5*wix);        //  wi.x > 0°

  // Vektoren haben die Länge 1
  cvektor3 spieg1 (ebx, -eby, 0);          // gleichschenkliges Dreieck in der xy-Ebene entlang der x-Achse mit Winkel wi.x bilden, wi.x [0°-180°]
  cvektor3 spieg2 (ebx, eby, 0);

  cvektor3 spieg3;
  real viy= cosl (wiy);
  real viz= cosl (wiz);

  if (ebx >= gquant)
    spieg3.x= 0.5*(viy + viz)/ebx;        // dritte Spiegelachse über Skalarprodukte ausrechnen
    else
    spieg3.x= 0;                          // wi.x = 180°, wi.y + wi.z = 180° wi.y [0..180°]

   if (eby >= gquant)
    spieg3.y= 0.5*(viy - viz)/eby;
    else
    spieg3.y= 0;                          // wi.x = 0°, wi.y = wi.z, wi.y [0..180°]

  spieg3.z= 1 - spieg3.x*spieg3.x - spieg3.y*spieg3.y;  // 2 Schnittpunkte der Kreise, negativer Wert = positive Determinante

  if (spieg3.z > -gquant)
    spieg3.z= sqrtl (fabsl (spieg3.z));
    else
    spieg3.z= 0;                          // Dreiecksungleichung ist nicht erfüllt
  return cbasis3 (spieg1, spieg2, spieg3);
  }

cbasis3 spiegelachsenvonebenenwinkel2 (const cvektor3 wi)  // über Skalarprodukt, Außenwinkel
  {
  // Grenzfälle: a, a, 0 a[0..180°], 180, a, 180-a a[0..180°] werden nicht korrekt behandelt
  real ebx= cosl (wi.x);
  real eby= sinl (wi.x);

  real s3x= cosl (wi.y);
  real s3y= (cosl (wi.z) - ebx*s3x)/eby;               // Fehler wenn wi.y 0 oder 180 Grad ist
  real s3z= sqrtl (1 - s3x*s3x - s3y*s3y);             // z-Koordinate so berechnen dass spieg3 die Länge 1 hat

  cvektor3 spieg1 (1, 0, 0);                          // x-Vektor
  cvektor3 spieg2 (ebx, eby, 0);                      // Vektor mit Winkel wi.x in xy-Ebene
  cvektor3 spieg3 (s3x, s3y, s3z);
  return cbasis3 (spieg1, spieg2, spieg3);
  }

cbasis3 spiegelachsenvonebenenwinkel3 (const cvektor3 wi)  // Außenwinkel, über trigonometrische Abstandsformel
  {
  // Grenzfälle: a, a, 0 a[0..180°], 180, a, 180-a a[0..180°] werden nicht korrekt behandelt
  real ebx= cosl (wi.x);
  real eby= sinl (wi.x);
  real lan= (cosl (wi.z) - cosl (wi.x)*cosl (wi.y))/(sinl (wi.x)*sinl (wi.y));     // Längengrad berechnen, Fehler wenn wi.x oder wi.y 0 oder 180 Grad sind

  cvektor3 spieg1 (0, 0, 1);                          // z-Vektor
  cvektor3 spieg2 (eby, 0, ebx);                      // Vektor mit Winkel wi.x in zx-Ebene
  cvektor3 spieg3 (sinl (wi.y)*lan, sinl (wi.y)*sqrtl (1 - lan*lan), cosl (wi.y));
  return cbasis3 (spieg1, spieg2, spieg3);
  }

cbasis3 spiegelachsenvonebenenwinkelk (const cvektor3 wi)  // Außenwinkel, über trigonometrische Abstandsformel
  {
  // Grenzfälle: (a, a, 0) a[0..180°]; (180, a, 180-a) a[0..180°] werden korrekt behandelt
  // Bereichsüberschreitungen (Dreiecksungleichung) werden korrigiert (der 3. Winkel wird auf die Grenze gesetzt welche er überschritten hat)
  
  cvektor2 sp0, sp1, sp2;                            // Spiegelachsen in Kugelkoordinaten
  cvektor3 spieg0, spieg1, spieg2;                   // Spiegelachsen
  real zenit= 0.5*M_PIl;                             // 90° Nordpol
  
  // Kugelkoordinaten der Spiegelachsen bestimmen bis auf sp2.x
  sp0.x= 0;
  sp0.y= zenit;
  sp1.x= 0;
  sp1.y= zenit - wi.x;
  sp2.y= zenit - wi.y;

  // Minimal- und Maximalwerte für den 3. Winkel ermitteln, dann sp2.x errechnen
  sp2.x= 0;
  real wizmin= acosl (sinl (sp1.y)*sinl (sp2.y) + cosl (sp1.y)*cosl (sp2.y)*cosl (sp1.x - sp2.x));
  sp2.x= M_PIl;
  real wizmax= acosl (sinl (sp1.y)*sinl (sp2.y) + cosl (sp1.y)*cosl (sp2.y)*cosl (sp1.x - sp2.x));
  sp2.x= acosl ((cosl (wi.z) - sinl (sp1.y)*sinl (sp2.y))/(cosl (sp1.y)*cosl (sp2.y)));
  real wiz= acosl (sinl (sp1.y)*sinl (sp2.y) + cosl (sp1.y)*cosl (sp2.y)*cosl (sp1.x - sp2.x));

  // sp2.x Spiegelachsen auf zulässige Grenzen setzen, falls wi.z außerhalb des gültigen Bereichs ist
  if (wi.z <= wizmin)
    sp2.x= 0;
  if (wi.z >= wizmax)
    sp2.x= M_PIl;
  
  // Debug-Ausgabe
  printf ("zmin:  %16.4Lf\n", wizmin/M_PIl*180);
  printf ("zmax:  %16.4Lf\n", wizmax/M_PIl*180);
  printf ("zmid:  %16.4Lf\n", wiz/M_PIl*180);
  
  // die in Kugelkoordinaten errechneten Spiegelachsen in kartesische Koordinaten umrechnen
  spieg0.x= cosl (sp0.x)*cosl (sp0.y);
  spieg0.y= sinl (sp0.x)*cosl (sp0.y);
  spieg0.z= sinl (sp0.y);
  spieg1.x= cosl (sp1.x)*cosl (sp1.y);
  spieg1.y= sinl (sp1.x)*cosl (sp1.y);
  spieg1.z= sinl (sp1.y);
  spieg2.x= cosl (sp2.x)*cosl (sp2.y);
  spieg2.y= sinl (sp2.x)*cosl (sp2.y);
  spieg2.z= sinl (sp2.y);
  return cbasis3 (spieg0, spieg1, spieg2);
  }

cbasis3 getsternbasis (const real pwi)    // 3 Achsen spannen ein Dreibein im ppp-Quadranten auf. pwi: Winkel zwischen den Achsen [0° - 120°] über Symmetrie im PPP-Quadranten
  {
  real w= cosl (pwi);
  real x= sqrtl (2 +   w - 2*sqrtl (1 + w - 2*w*w))/3;
  real z= sqrtl (5 - 2*w + 4*sqrtl (1 + w - 2*w*w))/3;
  if (w < 0)
    x= -x;
  return cbasis3 (cvektor3 (z, x, x), cvektor3 (x, z, x), cvektor3 (x, x, z));
  }

cbasis3 getsternbasis2 (const real pwi)      // über gleichschenkliges Dreieck
  {
  real s= sqrtl ((1 - cosl (pwi))/1.5);
  real w= sqrtl (1 - s*s);
  real y= 0.5*sqrtl (3);
  return cbasis3 (cvektor3 (s, 0, w), cvektor3 (-s*0.5, s*y, w), cvektor3 (-s*0.5, -s*y , w));
  }

void subvektorhinzu (cbasis3liste& pgruppe, cvektor3liste& ppartliste, cvektor3& psubvektor)        // bekommt die volle Gruppe, nicht nur den Kern
  {
  if (aehnlich (psubvektor, nullvektor3))    // 0°-Drehungen übergehen
    return;
  cvektor3liste subliste (1000);

  // Ordnungsliste durchgehen und für jeden Vektor die Gruppe aufspannen und nachsehen, ob der Subvektor dort drin ist. Falls ja, ist der Subvektor schon in einer Vektormenge enthalten
  for (integer vlauf= 0; vlauf < ppartliste.anz; vlauf++)
    {
    subliste.anz= 0;
    subliste.elementhinzu (ppartliste.v[vlauf], 0);
    subliste.schliesseab (pgruppe);                     // Vektormenge ermitteln, Subvektor bezüglich der Gruppe abschließen, nicht bezüglich des Kerns (Fehler sonst bei Th)
    if (subliste.elposition (psubvektor) != -1)         // subvektor nicht in ordliste
      return;
    }
  
  // Ordnung der Subklasse ermitteln
  cbasis3liste kern (pgruppe.maxanz);
  bildekern (pgruppe, kern);
  kern.ermittleordnung ();
  cvektor4 wia;
  cvektor3 laufachse;
  integer maxdreh= 0;
  for (integer blauf= 0; blauf < kern.anz; blauf++)
    {
    wia= winkelachsefrommatrix (kern.b[blauf]);
    laufachse= cvektor3 (wia.i, wia.j, wia.ij);
    if (aehnlich (winkelb (psubvektor, laufachse), 0))
      savemax (maxdreh, kern.ord[blauf]);
    }
  ppartliste.elementhinzu (psubvektor, maxdreh);            // maximale Drehordnung (=Zähligkeit) hinzufügen
  }

// Ermittlung der Drehachsen einer Gruppe und für jede Drehachsengruppe einen Vertreter auswählen
void ermittlepartvektoren (cbasis3liste& pgruppe, cvektor3liste& ppartliste)        // ppartliste
  {
  cvektor3liste drehachsenliste (1000);
  cvektor4 wa;
  cvektor3 achse;

  // Drehachsenliste erstellen
  for (integer elauf= 0; elauf < pgruppe.anz; elauf++)
    {
    if (det (pgruppe.b[elauf]) > 0)                      // Spiegelungen übergehen, da keine Drehachse
      {
      wa= winkelachsefrommatrix (pgruppe.b[elauf]);      // Drehmatrix aus Gruppe holen
      achse= cvektor3 (wa.i, wa.j, wa.ij);               // Drehachse von wa ermitteln
      drehachsenliste.elementhinzu (achse, 0);
      }
    }

  // Liste von Vektoren einer bestimmten Ordnung einen Vertreter hinzufügen
  ppartliste.anz= 0;
  for (integer dlauf= 0; dlauf < drehachsenliste.anz; dlauf++)
    subvektorhinzu (pgruppe, ppartliste, drehachsenliste.v[dlauf]);
  }

void bildekern (const cbasis3liste& pgruppe, cbasis3liste& pkern)
  {
  pkern.anz= 0;
  for (integer lauf= 0; lauf < pgruppe.anz; lauf++)
    if (det (pgruppe.b[lauf]) > 0)
      pkern.elementhinzu (pgruppe.b[lauf]);
  }

void bildespieg (const cbasis3liste& pgruppe, cbasis3liste& pspieg)
  {
  pspieg.anz= 0;
  cvektor4 wa;
  for (integer lauf= 0; lauf < pgruppe.anz; lauf++)
    {
    wa= winkelachsefrommatrix (pgruppe.b[lauf]);
    if (aehnlich (wa.r, -M_PIl))
      pspieg.elementhinzu (pgruppe.b[lauf]);
    }
  }

void getspiegkoord (const cbasis3liste& pbliste, cbasis3liste& pkoordliste)
  {
  pkoordliste.anz= 0;
  cvektor4 wa;
  cbasis3 koord;
  for (integer lauf= 0; lauf < pbliste.anz; lauf++)
    {
    wa= winkelachsefrommatrix (pbliste.b[lauf]);
    koord.z= cvektor3 (wa.i, wa.j, wa.ij);
    koord.x= senk (koord.z);
    koord.y= koord.z*koord.x;
    pkoordliste.b[lauf]= normiere (koord);
    }
  pkoordliste.anz= pbliste.anz;
  }

// interne Funktionen
//---------------------------------- Listenobjekte ----------------------------------------------------------------------------------------------------------------------------------
//---------------------------------- Vektor3liste ----------------------------------------------------
cvektor3liste::cvektor3liste (integer pmaxanz)
  {
  uebergelaufen= 0;
  maxanz= pmaxanz;
  anz= 0;
  v= new cvektor3[maxanz];
  ord= new integer[maxanz];
  }

integer cvektor3liste::elposition (cvektor3 pv)  // Listenposition eines Elements ermitteln -1= nirgendwo
  {
  real dif;
  for (integer lauf= 0;lauf < anz; lauf++)
    {
    dif= abs (pv - v[lauf]);
    if (dif <= gquant)
      return lauf;
    }
  return -1;
  }

void cvektor3liste::elementhinzu (cvektor3 pv, integer pord)
  {
  if (uebergelaufen)            // Liste übergelaufen
    return;
  if (elposition (pv) >= 0)     // Element schon vorhanden
    return;
  if (anz >= maxanz)            // Liste ist voll und läuft durch diese Elementhinzufügung über
    {
    uebergelaufen= 1;
    return;
    }
  v[anz]= pv;
  ord[anz]= pord;
  anz++;
  }

void cvektor3liste::laufedurch (cbasis3liste& pbliste)
  {
  cvektor3 el;
  integer kvanz= anz;
  integer kbanz= pbliste.anz;
  for (integer laufa= 0; laufa < kvanz; laufa++)
    for (integer laufb= 0; laufb < kbanz; laufb++)
      {
      el= pbliste.b[laufb]*v[laufa];
      if (elposition (el) < 0)
        {
        elementhinzu (el, pbliste.ord[laufb]);  // Element und Ordnung der Basis hinzufügen
        if (uebergelaufen)
          return;
        }
      }
  }

void cvektor3liste::schliesseab (cbasis3liste& pbliste)
  {
  integer altanz;
    do
    {
    altanz= anz;
    laufedurch (pbliste);
    }
  while (anz != altanz);
  }

//---------------------------------- Basis3liste -------------------------------------------------------------------------------
cbasis3liste::cbasis3liste (integer pmaxanz)
  {
  uebergelaufen= 0;
  maxanz= pmaxanz;
  anz= 0;
  b= new cbasis3[maxanz];
  ord= new integer[maxanz];
  ordanz= new integer[maxanz + 1];
  zykel= new integer[maxanz];
  gepaart= new integer[maxanz];
  for (integer lauf= 0; lauf < maxanz; lauf++)
    zykel[lauf]= 0;
  }

void cbasis3liste::kopiere (cbasis3liste& pbasis3liste)
  {
  maxanz= pbasis3liste.maxanz;
  anz= pbasis3liste.anz;
  for (integer lauf= 0; lauf < anz; lauf++)
    b[lauf]= pbasis3liste.b[lauf];
  }

integer cbasis3liste::elposition (cbasis3 pb)
  {
  for (integer lauf= 0;lauf < anz; lauf++)
    if (aehnlich (pb, b[lauf]))
      return lauf;
  return -1;
  }

void cbasis3liste::elementhinzu (cbasis3 pb)
  {
  if (uebergelaufen)
    return;
  if (elposition (pb) > -1)
    return;
  if (anz >= maxanz)
    {
    uebergelaufen= 1;
    return;
    }
  b[anz]= pb;
  anz++;
  }

void cbasis3liste::winkelachsehinzu (cvektor4 pq)   // Drehung hinzufügen
  {
  elementhinzu (matrixfromwinkelachse (pq));
  }

void cbasis3liste::winkelachseihinzu (cvektor4 pq)  // Drehspiegelung (Drehung + Inversion) hinzufügen
  {
  elementhinzu (-matrixfromwinkelachse (pq));
  }

void cbasis3liste::laufedurch ()
  {
  cbasis3 el;
  integer kanz= anz;
  for (integer laufa= 0; laufa < kanz; laufa++)
    for (integer laufb= 0; laufb < kanz; laufb++)
      {
      el= b[laufa]*b[laufb];
      if (elposition (el) < 0)
        {
        elementhinzu (el);
        if (uebergelaufen)
          return;
        }
      }
  }

void cbasis3liste::schliesseab ()
  {
  integer altanz;
    do
    {
    altanz= anz;
    laufedurch ();
    }
  while (anz != altanz);
  }

integer cbasis3liste::ermittleelementordnung (integer pel)
  {
  cbasis3liste zykelgruppe (maxanz);
  zykelgruppe.elementhinzu (b[pel]);
  zykelgruppe.schliesseab ();
  return zykelgruppe.anz;
  }
  
void cbasis3liste::ermittleordnung ()            // Ordnungen der Elemente ermitteln und Anzahlen der jeweiligen Ordnungen ermitteln
  {
  // ordanz array mit Nullen initialisieren
  for (integer lauf= 0; lauf < anz + 1; lauf++)  // warum denn einer mehr??????????? Verschiebung: Es gibt kein Element der Ordnung 0, es kann Elemente der Ordnung n geben
    ordanz[lauf]= 0;

  maxord= 0;
  integer elord;

  for (integer lauf= 0; lauf < anz; lauf++)     // von jedem Element die Ordnung ermitteln
    {
    elord= ermittleelementordnung (lauf);
    ord[lauf]= elord;
    ordanz[elord]++;                             // Anzahlarray mit gefundener Ordnung um 1 erhöhen
    savemax (maxord, elord);
    }
  }

void cbasis3liste::sortierenachordnung ()
  {
  integer startel;
  integer maxord;
  integer maxel;
  cbasis3 lb;
  integer lord;

  // Elemente nach Ordnung sortieren

  for (integer slauf= 0; slauf < anz; slauf++)
    {
    startel= slauf;
    maxel= slauf;
    maxord= ord[slauf];
    for (integer lauf= startel; lauf < anz; lauf++)
      if (ord[lauf] > maxord)
        {
        maxel= lauf;
        maxord= ord[lauf];
        }
    lb= b[startel];
    lord= ord[startel];
    b[startel]= b[maxel];
    ord[startel]= ord[maxel];
    b[maxel]= lb;
    ord[maxel]= lord;
    }

  // Zykel erstellen

  integer lpos;
  integer lzykel= 1;
  for (integer lauf= 0; lauf < anz; lauf++)
    {
    if (zykel[lauf] != 0)
      continue;
    lb= b[lauf];
    while (!aehnlich (lb, einsbasis3))
      {
      lpos= elposition (lb);
      if (zykel[lpos] == 0)
        zykel[lpos]= lzykel;
        else
        if (zykel[lpos] < 92000000000000000) // Größe von integer - 2 Stellen wegen Hundertergröße 9.2e16
          zykel[lpos]= zykel[lpos]*100 + lzykel;
      lb= lb*b[lauf];
      }
    lzykel++;
    }

  // Elemente nach Zykel innerhalb der Ordnung sortieren

  }

void cbasis3liste::ermittlepaarung ()
  {
  for (integer elauf= 0; elauf < anz; elauf++)
    {
    gepaart[elauf]= -1;
    for (integer plauf= 0; plauf < anz; plauf++)
      if (aehnlich (-b[elauf], b[plauf]))
        {
        gepaart[elauf]= plauf;
        break;
        }
    }
  }

//---------------------------------- Vektor4liste ----------------------------------------------------  Quaternionenliste
cvektor4liste::cvektor4liste (integer pmaxanz)
  {
  uebergelaufen= 0;
  maxanz= pmaxanz;
  anz= 0;
  v= new cvektor4[maxanz];
  ord= new integer[maxanz];
  ordanz= new integer[maxanz + 1];  // warum denn einer mehr? Weil eine Gruppe mit n Elementen Elemente der Ordnung n statt n-1 haben kann
  zykel= new integer[maxanz];
  gepaart= new integer[maxanz];
  for (integer lauf= 0; lauf < maxanz; lauf++)
    zykel[lauf]= 0;
  }

integer cvektor4liste::elposition (cvektor4 pq)
  {
  for (integer lauf= 0; lauf < anz; lauf++)
    if (aehnlich (pq, v[lauf]))
      return lauf;
  return -1;
  }

void cvektor4liste::elementhinzu (cvektor4 pq)
  {
  if (uebergelaufen)
    return;
  if (elposition (pq) >= 0)
    return;
  if (anz >= maxanz)
    {
    uebergelaufen= 1;
    return;
    }
  v[anz]= pq;
  anz++;
  }

void cvektor4liste::winkelachsehinzu (cvektor4 pq)
  {
  elementhinzu (quaternionfromwinkelachse (pq));
  }

void cvektor4liste::laufedurch ()
  {
  cvektor4 el;
  integer kanz= anz;
  for (integer laufa= 0; laufa < kanz; laufa++)
    for (integer laufb= 0; laufb < kanz; laufb++)
      {
      el= v[laufa]*v[laufb];
      if (elposition (el) < 0)
        {
        elementhinzu (el);
        if (uebergelaufen)
          return;
        }
      }
  }

void cvektor4liste::schliesseab ()
  {
  integer altanz;
    do
    {
    altanz= anz;
    laufedurch ();
    }
  while (anz != altanz);
  }

void cvektor4liste::ermittleordnung ()
  {
  cvektor4 lv;
  integer lord;
  for (integer lauf= 0; lauf < anz + 1; lauf++)  // warum denn einer mehr Weil eine Gruppe mit n Elementen Elemente der Ordnung n statt n-1 haben kann
    ordanz[lauf]= 0;
  for (integer lauf= 0; lauf < anz; lauf++)
    {
    lv= v[lauf];
    lord= 1;
    while (!aehnlich (lv, einsq))
      {
      lv= lv*v[lauf];
      lord+= 1;
      }
    ord[lauf]= lord;
    ordanz[lord]++;
    }
  }

void cvektor4liste::sortierenachordnung ()
  {
  integer startel;
  integer maxord;
  integer maxel;
  cvektor4 lv;
  integer lord;

  // Elemente nach Ordnung sortieren

  for (integer slauf= 0; slauf < anz; slauf++)
    {
    startel= slauf;
    maxel= slauf;
    maxord= ord[slauf];
    for (integer lauf= startel; lauf < anz; lauf++)
      if (ord[lauf] > maxord)
        {
        maxel= lauf;
        maxord= ord[lauf];
        }
    lv= v[startel];
    lord= ord[startel];
    v[startel]= v[maxel];
    ord[startel]= ord[maxel];
    v[maxel]= lv;
    ord[maxel]= lord;
    }

  // Zykel erstellen

  integer lpos;
  integer lzykel= 1;
  for (integer lauf= 0; lauf < anz; lauf++)
    {
    if (zykel[lauf] != 0)
      continue;
    lv= v[lauf];
    while (!aehnlich (lv, einsq))
      {
      lpos= elposition (lv);
      if (zykel[lpos] == 0)
        zykel[lpos]= lzykel;
        else
        if (zykel[lpos] < 92000000000000000) // Größe von integer - 2 Stellen wegen Hundertergröße 9.2e16
          zykel[lpos]= zykel[lpos]*100 + lzykel;
      lv= lv*v[lauf];
      }
    lzykel++;
    }

  // Elemente nach Zykel innerhalb der Ordnung sortieren

  }

void cvektor4liste::ermittlepaarung ()
  {
  for (integer elauf= 0; elauf < anz; elauf++)
    {
    gepaart[elauf]= -1;
    for (integer plauf= 0; plauf < anz; plauf++)
      if (aehnlich (-v[elauf], v[plauf]))
        {
        gepaart[elauf]= plauf;
        break;
        }
    }
  }

//---------------------------------------------- CCluster ---------------------------------------------------------

ccluster::ccluster (integer pmaxanz)
  {
  gruppe= new cbasis3liste (pmaxanz);
  kern= new cbasis3liste (pmaxanz);
  partliste= new cvektor3liste (pmaxanz);
  
  fliste= new cvektor3liste (pmaxanz);
  eliste= new cvektor3liste (pmaxanz);
  kliste= new cvektor3liste (pmaxanz);
  
  spieg= new cbasis3liste (pmaxanz);
  spiegkoord= new cbasis3liste (pmaxanz);
  }

void ccluster::vervollstaendige ()
  {
  gruppe->schliesseab ();
  gruppe->ermittleordnung ();
  bildekern (*gruppe, *kern);
  kern->ermittleordnung ();
  ermittlepartvektoren (*gruppe, *partliste);
  
  // Ecken, Kanten Flächenliste füllen
  for (integer lauf= 0; lauf < partliste->anz; lauf++)
    if (partliste->ord[lauf] == 2)
      {
      vertausche (partliste->v[0], partliste->v[lauf]);
      vertausche (partliste->ord[0], partliste->ord[lauf]);
      break;
      }
  for (integer lauf= 1; lauf < partliste->anz; lauf++)
    if (partliste->ord[lauf] == kern->maxord)
      {
      vertausche (partliste->v[1], partliste->v[lauf]);
      vertausche (partliste->ord[1], partliste->ord[lauf]);
      break;
      }
  kliste->elementhinzu (partliste->v[0], partliste->ord[0]);
  kliste->schliesseab (*gruppe);
  eliste->elementhinzu (partliste->v[1], partliste->ord[1]);
  eliste->schliesseab (*gruppe);
  if (partliste->anz > 2)
    {
    fliste->elementhinzu (partliste->v[2], partliste->ord[2]);
    fliste->schliesseab (*gruppe);
    }
  
  bildespieg (*gruppe, *spieg);
  getspiegkoord (*spieg, *spiegkoord);
  }

