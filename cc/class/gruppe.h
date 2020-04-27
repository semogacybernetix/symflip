#ifndef gruppe_h
#define gruppe_h

#include "vektor.h"

struct cbasis3liste
  {
  integer uebergelaufen;
  integer maxanz;                   // maximale Anzahl von Elementen, die die Liste haben kann
  integer anz;
  cbasis3* b;
  integer* ord;
  integer maxord;
  integer* ordanz;
  integer* zykel;
  integer* gepaart;  // Flag, ob negative einer Matrix in der Liste ist

  cbasis3liste (integer pmaxanz);       //constructor
  void kopiere (cbasis3liste& pbasis3liste);
  integer elposition (cbasis3 pb);
  void elementhinzu (cbasis3 pb);
  void winkelachsehinzu (cvektor4 pq);
  void winkelachseihinzu (cvektor4 pq);
  void laufedurch ();
  void schliesseab ();
  integer ermittleelementordnung (integer pel);
  void ermittleordnung ();               // Ordnungen der Elemente ermitteln und Anzahlen der jeweiligen Ordnungen ermitteln, maximale Ordnung ermitteln
  void sortierenachordnung ();
  void ermittlepaarung ();
};

struct cvektor3liste
  {
  integer uebergelaufen;
  integer maxanz;
  integer anz;
  cvektor3* v;
  integer* ord;

  cvektor3liste (integer pmaxanz);
  integer elposition (cvektor3 pv);
  void elementhinzu (cvektor3 pv, integer pord);
  void laufedurch (cbasis3liste& pbliste);
  void schliesseab (cbasis3liste& pbliste);
  };

struct cvektor4liste
  {
  integer uebergelaufen;
  integer maxanz;
  integer anz;
  cvektor4* v;       // Zeiger auf einen Vektor der Liste
  integer* ord;      // Zeiger auf die Ordnung des Vektors
  integer* ordanz;   // Anzahl der Elemente zu einer Ordnung
  integer* zykel;
  integer* gepaart;  // Flag, ob Komplement des Drehquaternions auch in der Liste ist

  cvektor4liste (integer pmaxanz);
  integer elposition (cvektor4 pq);
  void elementhinzu (cvektor4 pq);
  void winkelachsehinzu (cvektor4 pq);
  void laufedurch ();
  void schliesseab ();
  void ermittleordnung ();
  void sortierenachordnung ();
  void ermittlepaarung ();
  };

struct ccluster
  {
  cbasis3liste*    gruppe;
  cbasis3liste*    kern;
  cvektor3liste*   partliste;

  cvektor3liste*   fliste;
  cvektor3liste*   eliste;
  cvektor3liste*   kliste;

  cbasis3liste*    spieg;
  cbasis3liste*    spiegkoord;
  
  ccluster (const integer pmaxanz);
  void vervollstaendige ();
  };

//------------------------ Gruppenfunktionen ---------------------------------------------------------------------------------

cbasis3 drehachsenvonspiegelungen (const cbasis3& pb);
void drehungenvonspiegelungen (cbasis3liste& pliste, const cbasis3& pb);
void spiegelungenvondrehungen (cbasis3liste& pliste, const cvektor4 pd1, const cvektor4 pd2);
cbasis3 spiegelachsenvonebenenwinkele (const cvektor3 wi);   // 3 Winkel zwischen 3 Ebenen in 3 Spiegelachsen umrechnen Rausdrehung aus der gemeinsamen Ebene
cbasis3 spiegelachsenvonebenenwinkeld (const cvektor3 wi);   // 3 Winkel zwischen 3 Ebenen in 3 Spiegelachsen umrechnen  Innenwinkel eines Kugeldreiecks
cbasis3 spiegelachsenvonebenenwinkel2 (const cvektor3 wi);  // 3 Winkel zwischen 3 Ebenen in 3 Spiegelachsen umrechnen
cbasis3 spiegelachsenvonebenenwinkel3 (const cvektor3 wi);  // 3 Winkel zwischen 3 Ebenen in 3 Spiegelachsen umrechnen
cbasis3 spiegelachsenvonebenenwinkelk (const cvektor3 wi);  // 3 Winkel zwischen 3 Ebenen in 3 Spiegelachsen umrechnen
cbasis3 getsternbasis (const real pwi);                     // 3 Achsen spannen ein Dreibein im ppp-Quadranten auf. pwi: Winkel zwischen den Achsen [0° - 120°]
cbasis3 getsternbasis2 (const real pwi);                    // 3 Achsen spannen ein Dreibein im ppp-Quadranten auf. pwi: Winkel zwischen den Achsen [0° - 120°]
void ermittlepartvektoren (cbasis3liste& pgruppe, cvektor3liste& pordliste);           // Vektorgruppe in Ordnungsvektoren partitionieren
void bildekern (const cbasis3liste& pgruppe, cbasis3liste& pkern);                     // alle Matrizen mit positiver Determinante nach pkern kopieren
void bildespieg (const cbasis3liste& pgruppe, cbasis3liste& pspieg);
#endif
