/*
problematische Spezialfälle

norm (nullvektor): welcher Vektor soll zurückgegeben werden?
  1. der Nullvektor
  2. der Einsvektor

winkelachse von 180° Drehungen: welche der beiden Achsen soll zurückgegeben werden?
  1. die mit den wenigsten Minuszeichen
  2. die mit positiven ersten von 0 verschiedenen Wert

*/

#include "vektor.h"

real vquant= 1e-8;

cvektor2::cvektor2 () {}

cvektor2::cvektor2 (const real &px, const real &py)
  : x (px), y (py) {}

cbasis2::cbasis2 () {}

cbasis2::cbasis2 (const cvektor2 &px, const cvektor2 &py)
  : x (px), y (py) {}

cvektor3::cvektor3 () {}

cvektor3::cvektor3 (const real &pr)
  : x (pr), y (pr), z (pr) {}

cvektor3::cvektor3 (const real &px, const real &py, const real &pz)
  : x (px), y (py), z (pz) {}

cbasis3::cbasis3 () {}

cbasis3::cbasis3 (const real &pk)
  : x (pk, 0, 0), y (0, pk, 0), z (0, 0, pk) {}

cbasis3::cbasis3 (const cvektor3 &pv)
  : x (pv.x, 0, 0), y (0, pv.y, 0), z (0, 0, pv.z) {}

cbasis3::cbasis3 (const cvektor3 &px, const cvektor3 &py, const cvektor3 &pz)
  : x (px), y (py), z (pz) {}

cvektor4::cvektor4 () {}

cvektor4::cvektor4 (const real &pr, const real &pi, const real &pj, const real &pij)
  : r (pr), i (pi), j (pj), ij (pij) {}

cvektor4::cvektor4 (const real &pr, const cvektor3 &pv)
  : r (pr), i (pv.x), j (pv.y), ij (pv.z) {}

cvektor7::cvektor7 () {}

cvektor7::cvektor7 (const real &pr, const real &pi, const real &pj, const real &pij, const real &pji, const real &piji, const real &pjij)
  : r (pr), i (pi), j (pj), ij (pij), ji (pji), iji (piji), jij (pjij) {}

//--------------------- integer Funktionen ------------------------------------------------------------------------------

void vertausche (integer& a, integer& b)
  {
  integer c;
  c= a;
  a= b;
  b= c;
  }

integer max (integer a, integer b)
  {
  if (a >= b)
    return a;
  return b;
  }

void savemax (integer& pmax, integer pr)
  {
  pmax= max (pmax, pr);
  }
  
//--------------------- cvektor2 Vektor Funktionen ------------------------------------------------------------------------------

real norm (const cvektor2 pv)
  {
  return pv%pv;
  }

real abs (const cvektor2 pv)
  {
  return sqrtl (norm (pv));
  }

// Kreuzfunktion: unärer Operator, senkrechten Vektor in positver Orientierung ermitteln
cvektor2 senk (const cvektor2 &pv)
  {
  return cvektor2 (-pv.y, pv.x);
  }

//--------------------- cvektor2 Funktionen für komplexe Zahlen -----------------------------------------------------------------

// komplexer Kehrwert
cvektor2 kw (const cvektor2 pv)
  {
  return cvektor2 (pv.x, -pv.y)/norm (pv);
  }

cvektor2 polar (const cvektor2 pv)
  {
  if (fabsl (pv.x) + fabsl (pv.y) == 0)
    return cvektor2 (0, 0);
  real phi;
  if (fabsl (pv.x) >= fabsl (pv.y))
    {
    phi= atanl (pv.y/pv.x);
    if (pv.x < 0)
      {
      if (pv.y >= 0)  // 180° statt -180°
        phi+= M_PIl;
        else
        phi+= -M_PIl;
      }
    }
    else
    {
    phi= atanl (pv.x/pv.y);
    if (pv.y >= 0)
      phi=0.5*M_PIl - phi;
      else
      phi=-0.5*M_PIl - phi;
    }
  return cvektor2 (abs (pv), phi);
  }

cvektor2 kartes (const cvektor2 pv)
  {
  return cvektor2 (pv.x*cosl (pv.y), pv.x*sinl (pv.y));
  }

cvektor2 exp (const cvektor2 pv)
  {
  return kartes (cvektor2 (expl (pv.x), (pv.y)));
  }

cvektor2 log (const cvektor2 pv)
  {
  cvektor2 lv= polar (pv);
  if (lv.x > 0)
    lv.x= logl (lv.x);
   else
   lv.x= -16;
  return lv;
  }

cvektor2 cosh (const cvektor2 pv)
  {
  cvektor2 m= cvektor2 (-1, 0);
  return (exp (pv) + exp (m*pv))/2;
  }

cvektor2 sinh (const cvektor2 pv)
  {
  cvektor2 m= cvektor2 (-1, 0);
  return (exp (pv) - exp (m*pv))/2;
  }

cvektor2 cos (const cvektor2 pv)
  {
  cvektor2 i= cvektor2 (0, 1);
  cvektor2 mi= cvektor2 (0, -1);
  return (exp (i*pv) + exp (mi*pv))/2;
  }

cvektor2 sin (const cvektor2 pv)
  {
  cvektor2 i= cvektor2 (0, 1);
  cvektor2 mi= cvektor2 (0, -1);
  return (exp (i*pv) - exp (mi*pv))/(2*i);
  }

cvektor2 sqrt (const cvektor2 pv)
  {
  cvektor2 wv= polar (pv);
  wv.x= sqrtl (wv.x);
  wv.y= wv.y/2;
  return kartes (wv);
  }

//---------- cvektor2 Vektor Operatoren ----------------------------------------------------------------------------------------------

// Operatoren vergleichen
integer operator == (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return (pv1.x == pv2.x) && (pv1.y == pv2.y);
  }

// Vorzeichen des Vektors ändern
cvektor2 operator - (const cvektor2 &pv)
  {
  return -1*pv;
  }

// Vektoraddition
cvektor2 operator + (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return cvektor2 (pv1.x + pv2.x, pv1.y + pv2.y);
  }

// Vektorsubtraktion
cvektor2 operator - (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return pv1 + -pv2;
  }

// Skalarmultiplikation
cvektor2 operator * (const real &pf, const cvektor2 &pv)
  {
  return cvektor2 (pf*pv.x, pf*pv.y);
  }

// Skalardivision
cvektor2 operator / (const cvektor2 &pv, const real &pf)
  {
  return cvektor2 (pv.x/pf, pv.y/pf);
  }

// Skalarprodukt
real operator % (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return pv1.x*pv2.x + pv1.y*pv2.y;
  }

// ------------- cvektor2 Operatoren für komplexe Zahlen ---------------------------------------------------------

// komplexe Multiplikation
cvektor2 operator * (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return cvektor2 (pv1.x*pv2.x - pv1.y*pv2.y, pv1.x*pv2.y + pv1.y*pv2.x);
  }

// komplexe Division
cvektor2 operator / (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return pv1*kw (pv2);
  }

// Division reelle Zahl durch komplexe Zahl
cvektor2 operator / (const real &pf, const cvektor2 &pv)
  {
  return cvektor2 (pf, 0)/pv;
  }

// komplexe Potentation
cvektor2 operator ^ (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return exp (pv2*log (pv1));
  }

// Winkel zwischen 2 (Vektoren in Polarkoordinaten)
real operator | (const cvektor2 &pv1, const cvektor2 &pv2)
  {
  return acosl (sinl (pv1.y)*sinl (pv2.y) + cosl (pv1.y)*cosl (pv2.y)*cosl (pv1.x - pv2.x));
  }

//---------- cvektor2, cbasis2 ----------------------------------------------------------------------------------

// Matrix mal Vektor
// Erklärung: die Koordinaten des Vektors beziehen sich auf das Koordinatensystem der Matrix. Die Multiplikation rechnet die kanonischen Koordinaten aus.
cvektor2 operator * (const cbasis2 &pb, const cvektor2 &pv)
  {
  return pv.x*pb.x + pv.y*pb.y;
  }

// Vektor mal transponierte Matrix
cvektor2 operator & (const cvektor2 &pv, const cbasis2 &pb)
  {
  return cvektor2 (pv%pb.x, pv%pb.y);
  }

// Vektor durch eine Matrix teilen
// Erklärung: die Koordinaten des Vektors beziehen sich auf das kanonische Koordinatensystem. Die Multiplikation rechnet die Koordinaten des Vektors in Koordinaten
// des Koordinatensystems der Matrix aus.
cvektor2 operator / (const cvektor2 &pv, const cbasis2 &pb)
  {
  // Vektor transponiert mit senkrechtem Raum multiplizieren
  return pv & ~pb;
  }

//---------- cbasis2 Funktionen -------------------------------------------------------------------------------------------

real det (const cbasis2 &pb)
  {
  return senk (pb.x) % pb.y;
  }

cbasis2 transp (const cbasis2 &pb)
  {
  return cbasis2 (cvektor2 (pb.x.x, pb.y.x),
                  cvektor2 (pb.x.y, pb.y.y));
  }

//---------- cbasis2 Operatoren -------------------------------------------------------------------------------------------

// transponierte inverse Matrix bestimmen (senkrechter Raum zur Basis)
cbasis2 operator ~ (const cbasis2 &pb)
  {
  // Haupdiagonale vertauschen, Nebendiagonale Vorzeichen ändern, durch die Determinante teilen
  return cbasis2 (-senk (pb.y), senk (pb.x))/(senk (pb.x) % pb.y);
  }

// Matrixaddition
cbasis2 operator + (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  return cbasis2 (pb1.x + pb2.x, pb1.y + pb2.y);
  }

// Matrixsubtraktion
cbasis2 operator - (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  return cbasis2 (pb1.x - pb2.x, pb1.y - pb2.y);
  }

// Skalarmultiplikation
cbasis2 operator * (const real &pf, const cbasis2 &pb)
  {
  return cbasis2 (pf*pb.x, pf*pb.y);
  }

// Skalardivision
cbasis2 operator / (const cbasis2 &pb, const real &pf)
  {
  return cbasis2 (pb.x/pf, pb.y/pf);
  }

// Matrixmultiplikation
// Erklärung: die Koordinaten der Vektoren der 2. Basis beziehen sich auf die 1. Basis.
// Die Multiplikation rechnet die kanonischen Koordinaten der Vektoren der 2. Basis aus.
cbasis2 operator * (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  return cbasis2 (pb1*pb2.x, pb1*pb2.y);
  }

// Matrixlinksdivision
// Erklärung: die Koordinaten der Vektoren der 2. Basis beziehen sich auf die kanonische Basis
// Die Division rechnet die Koordinaten der Vektoren der 2. Basis in Koordinaten der 1. Basis aus
cbasis2 operator / (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  // transponiert mit dem senkrechten Raum multiplizieren
  return pb1 & ~pb2;
  }

// Matrixrechtsdivision
cbasis2 operator | (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  // von rechts mit dem Inversen multiplizieren
  return pb1 * transp (~pb2);
  }

// Matrix mal transponierte Matrix
cbasis2 operator & (const cbasis2 &pb1, const cbasis2 &pb2)
  {
  return cbasis2 (pb1.x & pb2, pb1.y & pb2);
  }

//---------- cvektor3 Funktionen ------------------------------------------------------------------------------------------

void vertausche (cvektor3& a, cvektor3& b)
  {
  cvektor3 c;
  c= a;
  a= b;
  b= c;
  }

real norm (const cvektor3 &pv)
  {
  return pv%pv;
  }

real abs (const cvektor3 &pv)
  {
  return sqrtl (norm (pv));
  }

// Kreuzfunktion: unärer Operator, senkrechten Vektor in positver Orientierung ermitteln: kürzeste Komponente Null setzen, die anderen beiden senkrechtisieren
cvektor3 senk (const cvektor3 &pv)
  {
  if ((fabsl (pv.x) <= fabsl (pv.y)) && (fabsl (pv.x) <= fabsl (pv.z)))
    return cvektor3 (0, -pv.z, pv.y);
  if (fabsl (pv.y) <= fabsl (pv.z))
    return cvektor3 (pv.z, 0, -pv.x);
  return cvektor3 (-pv.y, pv.x, 0);
  }

cvektor3 normiere (const cvektor3 &pv)
  {
  real l (abs (pv));
  if (l <= vquant)
    return cvektor3 (0, 0, 0);  // (0, 0,0) oder (1, 1, 1) ?
  return pv/l;
  }

cvektor3 orientiere (const cvektor3 &pv)   // erster von 0 verschiedener Wert wird positiv gesetzt
  {
  cvektor3 ret (pv);
  if (ret.x < 0)
    ret= -ret;
  if (fabsl (ret.x) <= vquant)
    {
    ret.x= 0;
    if (ret.y < 0)
      ret= -ret;
    if (fabsl (ret.y) <= vquant)
      {
      ret.y= 0;
      if (ret.z < 0)
        ret= -ret;
      if (fabsl (ret.z) <= vquant)
        ret.z= 0;
      }
    }
  return ret;
  }

real winkelb (const cvektor3 &pv1, const cvektor3 &pv2)   // Betrag des Winkels zwischen 2 Vektoren ermitteln, Ergebnis: [0..pi]
  {
  real nenner (abs (pv1)*abs (pv2));
  if (nenner <= vquant)
    return 0;
  real cw= (pv1%pv2)/nenner;
  if (cw >= 1)
    return 0;
  if (cw <= -1)
    return M_PIl;
  return acosl (cw);
  }

real winkelg (const cvektor3 &pv1, const cvektor3 &pv2)   // Betrag des Winkels zwischen 2 Geraden ermitteln, Ergebnis: [0..pi/2]
  {
  real nenner (abs (pv1)*abs (pv2));
  if (nenner <= vquant)
    return 0;
  real cw= (pv1%pv2)/nenner;
  if (cw < 0)
    cw= -cw;
  if (cw >= 1)
    return 0;
  return acosl (cw);
  }

//---------- cvektor3 Operatoren ------------------------------------------------------------------------------------------

// Vektoren vergleichen
integer operator == (const cvektor3 &pv1, const cvektor3 &pv2)
  {
  return (pv1.x == pv2.x) && (pv1.y == pv2.y) && (pv1.z == pv2.z);
  }

// Vorzeichen des Vektors ändern
cvektor3 operator - (const cvektor3 &pv)
  {
  return cvektor3 (-pv.x, -pv.y, -pv.z);
  }

// Vektoradditon
cvektor3 operator + (const cvektor3 &pv1, const cvektor3 &pv2)
  {
  return cvektor3 (pv1.x + pv2.x, pv1.y + pv2.y, pv1.z + pv2.z);
  }

// Vektorsubtraktion
cvektor3 operator - (const cvektor3 &pv1, const cvektor3 &pv2)
  {
  return cvektor3 (pv1.x - pv2.x, pv1.y - pv2.y, pv1.z - pv2.z);
  }

// Skalarmultiplikation
cvektor3 operator * (const real &pf, const cvektor3 &pv)
  {
  return cvektor3 (pf*pv.x, pf*pv.y, pf*pv.z);
  }

// Skalardivision
cvektor3 operator / (const cvektor3 &pv, const real &pf)
  {
  return cvektor3 (pv.x/pf, pv.y/pf, pv.z/pf);
  }

// Skalarprodukt
real operator % (const cvektor3 &pv1, const cvektor3 &pv2)
  {
  return pv1.x*pv2.x + pv1.y*pv2.y + pv1.z*pv2.z;
  }

// Kreuzprodukt (senkrecht stehenden Vektor zu 2 Vektoren in positiver Orientierung ermitteln)
cvektor3 operator ^ (const cvektor3 &pv1, const cvektor3 &pv2)
  {
  return cvektor3 (pv1.y*pv2.z - pv1.z*pv2.y, pv1.z*pv2.x - pv1.x*pv2.z, pv1.x*pv2.y - pv1.y*pv2.x);
  }

// einen Vektor aus der Ebene wählen, auf dem der Vektor senkrecht steht
cvektor3 operator ~ (const cvektor3 &pv)
  {
  if ((fabsl (pv.x) < fabsl (pv.y)) && (fabsl (pv.x) < fabsl (pv.z)))
    return cvektor3 (0, pv.z, -pv.y);
  if (fabsl (pv.y) < fabsl (pv.z))
    return cvektor3 (-pv.z, 0, pv.x);
  return cvektor3 (pv.y, -pv.x, 0);
  }

//---------- cvektor3, cbasis3 ---------------------------------------------------------------------------------

// Matrix mal Vektor
cvektor3 operator * (const cbasis3 &pb, const cvektor3 &pv)
  {
  return pv.x*pb.x + pv.y*pb.y + pv.z*pb.z;
  }

// Vektor mal transponierte Matrix
cvektor3 operator & (const cvektor3 &pv, const cbasis3 &pb)
  {
  return cvektor3 (pv%pb.x, pv%pb.y, pv%pb.z);
  }

// Vektor durch eine Matrix teilen
cvektor3 operator / (const cvektor3 &pv, const cbasis3 &pb)
  {
  return pv & ~pb;
  }

//---------- cbasis3 Funktionen ------------------------------------------------------------------------------------------

real norm (cbasis3 pb)
  {
  return norm (pb.x) + norm (pb.y) + norm (pb.z);
  }

real abs (cbasis3 pb)
  {
  return sqrtl (norm (pb));
  }

real det (const cbasis3 &pb)
  {
  return (pb.x^pb.y)%pb.z;
  }

cbasis3 normiere (const cbasis3 &pb)
  {
  const cvektor3 x (normiere (pb.x));
  const cvektor3 y (normiere (pb.z^x));
  return cbasis3 (x, y, normiere (x^y));
  }

cbasis3 normiere2 (const cbasis3 &pb)
  {
  const cvektor3 x (normiere (pb.x));
  const cvektor3 y (normiere (pb.y - pb.y%x*x));
  const cvektor3 z (normiere (pb.z - pb.z%x*x - pb.z%y*y));
  return cbasis3 (x, y, z);
  }

cbasis3 transp (const cbasis3 &pb)
  {
  return cbasis3 (cvektor3 (pb.x.x, pb.y.x, pb.z.x),
                  cvektor3 (pb.x.y, pb.y.y, pb.z.y),
                  cvektor3 (pb.x.z, pb.y.z, pb.z.z));
  }

cvektor3 geteigen (const cbasis3 &pb)    // Eigenvektor von einer homogenen Basis berechnen (det = 0)
  {
  const cbasis3 orthobasis (transp (cbasis3 (pb.y^pb.z, pb.z^pb.x, pb.x^pb.y)));
  const real sx (orthobasis.x%orthobasis.x);
  const real sy (orthobasis.y%orthobasis.y);
  const real sz (orthobasis.z%orthobasis.z);

  // längsten Eigenvektor zurückgeben
  if ((sx >= sy) && (sx >= sz))
    return orthobasis.x;
  if (sy >= sz)
    return orthobasis.y;
  return orthobasis.z;
  }

cbasis3 getrotx (const real &pf)
  {
  const real s (sinl(pf));
  const real c (cosl(pf));
  return cbasis3 (cvektor3 (1, 0, 0), cvektor3 (0, c, s), cvektor3 (0, -s, c));
  }

cbasis3 getroty (const real &pf)
  {
  const real s (sinl(pf));
  const real c (cosl(pf));
  return cbasis3 (cvektor3 (c, 0, -s), cvektor3 (0, 1, 0), cvektor3 (s, 0, c));
  }

cbasis3 getrotz (const real &pf)
  {
  const real s (sinl(pf));
  const real c (cosl(pf));
  return cbasis3 (cvektor3 (c, s, 0), cvektor3 (-s, c, 0), cvektor3 (0, 0, 1));
  }

// Spiegelungsmatrix direkt berechnen
cbasis3 getspiegbasis (const cvektor3 &pv)
  {
  if (abs (pv) <= vquant)   // Nullvektor als Spiegelachse ist Punktspiegelung also Inversionsmatrix zurückgeben
    return cbasis3 (-1);
  cvektor3 nv= normiere (pv);
  return cbasis3 (1) - 2*cbasis3 (nv, nv, nv)*cbasis3 (nv);
  }

// Spiegelungsmatrix aus (Drehung von 180° um Spiegelachse) berechnen
cbasis3 getdrehspiegbasis (const cvektor3 &pv)
  {
  if (abs (pv) <= vquant)   // Nullvektor als Spiegelachse ist Punktspiegelung also Inversionsmatrix zurückgeben
    return cbasis3 (-1);
  return -matrixfromwinkelachse (cvektor4 (M_PIl, pv.x, pv.y, pv.z));
  }

cvektor3 getspiegachse (const cbasis3 &pb)
  {
  return normiere (geteigen (pb + cbasis3 (1)));
  }

integer aehnlich (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  real dif= abs (pb1 - pb2);
  if (dif <= vquant)
    return 1;
  return 0;
  }

//---------- cbasis3 Operatoren ------------------------------------------------------------------------------------------

// transponierte inverse Matrix bestimmen
cbasis3 operator ~ (const cbasis3 &pb)
  {
  return cbasis3 (pb.y^pb.z, pb.z^pb.x, pb.x^pb.y)/((pb.x^pb.y)%pb.z);
  }

// Matrixaddition
cbasis3 operator - (const cbasis3  &pb)
  {
  return -1*pb;
  }

cbasis3 operator + (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  return cbasis3 (pb1.x + pb2.x, pb1.y + pb2.y, pb1.z + pb2.z);
  }

// Matrixsubtraktion
cbasis3 operator - (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  return cbasis3 (pb1.x - pb2.x, pb1.y - pb2.y, pb1.z - pb2.z);
  }

// Skalarmultiplikation
cbasis3 operator * (const real &pf, const cbasis3 &pb)
  {
  return cbasis3 (pf*pb.x, pf*pb.y, pf*pb.z);
  }

// Skalardivision
cbasis3 operator / (const cbasis3 &pb, const real &pf)
  {
  return cbasis3 (pb.x/pf, pb.y/pf, pb.z/pf);
  }

// Matrixmultiplikation
cbasis3 operator * (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  return cbasis3 (pb1*pb2.x, pb1*pb2.y, pb1*pb2.z);
  }

// Matrix mal transponierte Matrix
cbasis3 operator & (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  return cbasis3 (pb1.x & pb2, pb1.y & pb2, pb1.z & pb2);
  }

// Matrixlinksdivision
cbasis3 operator / (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  // transponiert mit dem senkrechten Raum multiplizieren
  return pb1 & ~pb2;
  }

// Matrixrechtsdivision
cbasis3 operator | (const cbasis3 &pb1, const cbasis3 &pb2)
  {
  // von rechts mit dem Inversen multiplizieren
  return pb1 * transp (~pb2);
  }

//---------- cvektor4 Vektor Funktionen -----------------------------------------------------------------------------------------------

real norm (const cvektor4 &pv)
  {
  return pv%pv;
  }

real abs (const cvektor4 &pv)
  {
  return sqrtl (norm (pv));
  }

// Kreuzfunktion: unärer Operator, senkrechten Vektor in positver Orientierung ermitteln
cvektor4 senk (const cvektor4 &pv)
  {
  return cvektor4 (-pv.i, pv.r, -pv.ij, pv.j);
  }

cvektor4 normiere (const cvektor4 &pv)
  {
  const real q= norm (pv);
  if (q <= vquant)
    return cvektor4 (0, 0, 0, 0);
  return pv/sqrtl (q);
  }

// Kreuzfunktion (einen senkrecht stehenden Vektor ermitteln)
cvektor4 operator ~ (const cvektor4 &pv)
  {
  return cvektor4 (-pv.i, pv.r, -pv.ij, pv.j);
  }

integer aehnlich (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  real dif= abs (pv1 - pv2);
  if (dif <= vquant)
    return 1;
  return 0;
  }

//---------- cvektor4 Funktionen für Quaternionen -------------------------------------------------------------------------------------

// Kehrwert
cvektor4 kw (const cvektor4 &pv)
  {
  return cvektor4 (pv.r, -pv.i, -pv.j, -pv.ij)/norm (pv);
  }

cvektor3 getachse (const cvektor4 &pv)
  {
  return cvektor3 (pv.i, pv.j, pv.ij);
  }
//---------- cvektor4 Vektor Operatoren ------------------------------------------------------------------------------------------------

// Vergleich
integer operator == (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return (pv1.r == pv2.r) && (pv1.i == pv2.i) && (pv1.j == pv2.j) && (pv1.ij == pv2.ij);
  }

// Vorzeichen ändern
cvektor4 operator - (const cvektor4 &pv)
  {
  return cvektor4 (-pv.r, -pv.i, -pv.j, -pv.ij);
  }

// Vektoraddition
cvektor4 operator + (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return cvektor4 (pv1.r + pv2.r, pv1.i + pv2.i, pv1.j + pv2.j, pv1.ij + pv2.ij);
  }

// Vektorsubtraktion
cvektor4 operator - (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return cvektor4 (pv1.r - pv2.r, pv1.i - pv2.i, pv1.j - pv2.j, pv1.ij - pv2.ij);
  }

// Skalarmultiplikation
cvektor4 operator * (const real &pf , const cvektor4 &pv)
  {
  return cvektor4 (pf*pv.r, pf*pv.i, pf*pv.j, pf*pv.ij);
  }

// Skalardivision
cvektor4 operator / (const cvektor4 &pv, const real &pf)
  {
  return cvektor4 (pv.r/pf, pv.i/pf, pv.j/pf, pv.ij/pf);
  }

// Skalarprodukt
real operator % (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return pv1.r*pv2.r + pv1.i*pv2.i + pv1.j*pv2.j + pv1.ij*pv2.ij;
  }

//--------------------- Operatoren für Quaternionen ---------------------------------------------------------

// Quaternionenmultiplikation
cvektor4 operator * (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return cvektor4 (pv1.r*pv2.r  -  pv1.i*pv2.i  -  pv1.j*pv2.j  -  pv1.ij*pv2.ij,
                   pv1.r*pv2.i  +  pv1.i*pv2.r  +  pv1.j*pv2.ij -  pv1.ij*pv2.j,
                   pv1.r*pv2.j  +  pv1.j*pv2.r  +  pv1.ij*pv2.i -  pv1.i*pv2.ij,
                   pv1.r*pv2.ij +  pv1.ij*pv2.r +  pv1.i*pv2.j  -  pv1.j*pv2.i);
  }

// Quaternionenlinksdivision: von links mit dem Inversen multiplizieren: b*a = c  <=>  a = b^-1 * c  <=>  a = c/b
cvektor4 operator / (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return kw (pv2) * pv1;
  }

// Quaternionenrechtsdivision: von rechts mit dem Inversen multiplizieren: b*a = c  <=>  b = c * a^-1  <=>  b = c|a
cvektor4 operator | (const cvektor4 &pv1, const cvektor4 &pv2)
  {
  return pv1 * kw (pv2);
  }

// 3D-Vektor mit Drehquaternion drehen
cvektor3 operator * (const cvektor4 &pq, const cvektor3 &pv)
  {
  //  cvektor4 ret= pq*cvektor4 (0, pv.x, pv.y, pv.z)|pq;      v' =  q * v / q

  cvektor4 pv2= cvektor4 (pv.x*pq.i  + pv.y*pq.j  + pv.z*pq.ij,
                          pv.x*pq.r  - pv.y*pq.ij + pv.z*pq.j,
                          pv.x*pq.ij + pv.y*pq.r  - pv.z*pq.i,
                         -pv.x*pq.j  + pv.y*pq.i  + pv.z*pq.r);

  return cvektor3 (pq.r*pv2.i  +  pq.i*pv2.r  +  pq.j*pv2.ij -  pq.ij*pv2.j,
                   pq.r*pv2.j  +  pq.j*pv2.r  +  pq.ij*pv2.i -  pq.i*pv2.ij,
                   pq.r*pv2.ij +  pq.ij*pv2.r +  pq.i*pv2.j  -  pq.j*pv2.i);
  }

//---------- cvektor7 Funktionen ------------------------------------------------------------------------------------------

//--------------------- Operatoren für Heptanionen ---------------------------------------------------------

// Heptanionenmultiplikation
cvektor7 operator * (const cvektor7 &pv1, const cvektor7 &pv2)
  {
  real lr, li, lj, lij, lji, liji, ljij;

  lr= pv1.r*pv2.r - pv1.i*pv2.i - pv1.ij*pv2.ij - pv1.ji*pv2.ji - pv1.iji*pv2.iji - pv1.jij*pv2.jij - pv1.i*pv2.jij - pv1.jij*pv2.i - pv1.j*pv2.iji - pv1.iji*pv2.j + pv1.ij*pv2.ji + pv1.ji*pv2.ij;
  li= pv1.r*pv2.i + pv1.i*pv2.r - pv1.ij*pv2.j - pv1.j*pv2.ji - pv1.iji*pv2.ji - pv1.ij*pv2.iji + pv1.iji*pv2.ij + pv1.ji*pv2.iji;
  lj= pv1.r*pv2.j + pv1.j*pv2.r - pv1.ji*pv2.i - pv1.i*pv2.ij - pv1.jij*pv2.ij - pv1.ji*pv2.jij + pv1.jij*pv2.ji + pv1.ij*pv2.jij;
  lij= pv1.r*pv2.ij + pv1.ij*pv2.r - pv1.iji*pv2.i - pv1.j*pv2.jij - pv1.iji*pv2.jij + pv1.i*pv2.j;
  lji= pv1.r*pv2.ji + pv1.ji*pv2.r - pv1.jij*pv2.j - pv1.i*pv2.iji - pv1.jij*pv2.iji + pv1.j*pv2.i;
  liji= pv1.r*pv2.iji + pv1.iji*pv2.r + pv1.ij*pv2.i + pv1.i*pv2.ji;
  ljij= pv1.r*pv2.jij + pv1.jij*pv2.r + pv1.ji*pv2.j + pv1.j*pv2.ij;

  return cvektor7 (lr, li, lj, lij, lji, liji, ljij);
  }

// ------------------------------ Drehdarstellungen umrechnen ------------------------------------------------------------------------

cvektor4 quaternionfromwinkelachse (const cvektor4 pwa)
  {
  real wiha= 0.5*pwa.r;
  cvektor3 achse= normiere (cvektor3 (pwa.i, pwa.j, pwa.ij));
  return cvektor4 (cosl (wiha), sinl (wiha)*achse);                // negativer Winkel invertiert die Drehachse statt Winkel
  }                                                                // Darstellung ist eindeutig. Das negative Quaternion, dessen Drehung zur gleichen Endposition führt (Matrix) ist eine andere Drehung
                                                                   // 90° nach rechts drehen führt zur gleichen Endposition wie 270° nach links drehen, aber es sind verschiedene Drehungen
                                                                   // Somit wird nicht eine Drehung durch 2 Quaternionen repräsentiert, sondern jede Drehung ist eindeutig durch genau ein Quaternion repräsentiert.
                                                                   // Mehrdeutigkeit kommt durch Wicklungen zustande, d.h. wenn der Winkel größer als 360° ist.
                                                                   // Jede Endposition wird durch genau 2 Drehungen erreicht. Das gilt auch für die Anfangsposition.
                                                                   // Die Anfangsposition kann durch eine 0° Drehung oder durch eine 360° Drehung erreicht werden. In beiden Fällen spielt die Drehachse keine Rolle.

cvektor4 winkelachsefromquaternion (const cvektor4 pq)
  {
  if (pq.r >= 1 - vquant)
    return cvektor4 (0, 0, 0, 0);
  if (pq.r <= -1 + vquant)
    return cvektor4 (2*M_PIl, 0, 0, 0);
  real drehw= 2*acosl (pq.r);
  cvektor3 achse (normiere (cvektor3 (pq.i, pq.j, pq.ij)));
  if (fabsl (achse.x) <= vquant)
    achse.x= 0;
  if (fabsl (achse.y) <= vquant)
    achse.y= 0;
  if (fabsl (achse.z) <= vquant)
    achse.z= 0;
  return cvektor4 (drehw, achse.x, achse.y, achse.z);
  }

cvektor4 qwafrommatrix (const cbasis3 &pdreh)  // nur für Drehmatrizen, nicht für Drehspiegelungsmatrizen
  {
  // Gibt den Winkel in Quaternionenform [-1 bis 1] und die Drehachse unnormiert aber richtig orientiert zurück.

  // ********************* Realteil direkt aus der Matrix berechnen, Problem bei 0° und 180° Drehungen abfangen
  real qr= 1 + pdreh.x.x + pdreh.y.y + pdreh.z.z;              // qr [4..0] = [0..180]°
  if (qr >= 4 - vquant)             //   0° Drehung
    return cvektor4 (1, 0, 0, 0);

  // ********************* Drehachse durch Eigenwerte berechnen, Eigenvektor numerisch stabil, Länge: [5 1/3 bis 16]
  cbasis3 hdreh (pdreh);
  hdreh.x.x-= 1;
  hdreh.y.y-= 1;
  hdreh.z.z-= 1;
  cvektor3 drehachse (geteigen (hdreh));

  // Drehachse richtig orientieren
  if (qr <= vquant)                 // 180° Drehung: Es gibt 2 Drehachsen statt nur einer
    {
    qr= 0;
    drehachse= orientiere (drehachse);  // bei 180° Drehung Achse mit positven ersten von 0 verschiedenem Wert aussuchen. (besser kleinste Anzahl von Minuszeichen)
    }
    else                          // Drehung zwischen 0° und 180°
    {
    // Drehachse richtig orientieren (Testen ob die Drehung ein positives Volumen erzeugt, versagt bei 0° und 180° Drehungen da Volumen dann 0 ist)
    cvektor3 s1 (~drehachse);
    cvektor3 s2 (pdreh*s1);
    if (((drehachse^s1)%s2) < 0)
      drehachse= -drehachse;
    }
  return cvektor4 (0.5*sqrtl (qr), drehachse.x, drehachse.y, drehachse.z);
  }

cvektor4 winkelachsefrommatrix (const cbasis3 &pdreh)    // Bei Drehspiegelungen wird der Spiegelanteil durch Inversion der Drehspiegelungsmatrix entfernt und dann Winkel und Achse der verbleibenden Drehmatrix bestimmt
  {
  cvektor4 qwa;
  if (det (pdreh) > 0)
    qwa= qwafrommatrix (pdreh);
    else
    qwa= qwafrommatrix (-pdreh);
  if (fabsl (qwa.i) <= vquant)
    qwa.i= 0;
  if (fabsl (qwa.j) <= vquant)
    qwa.j= 0;
  if (fabsl (qwa.ij) <= vquant)
    qwa.ij= 0;
  cvektor3 achse (qwa.i, qwa.j, qwa.ij);
  achse= normiere (achse);
  if (det (pdreh) < 0)
    {
    real wi= 2*acosl (qwa.r);
    if (aehnlich (wi, 0))
      return cvektor4 (-2*M_PI, achse.x, achse.y, achse.z);
    return cvektor4 (-2*acosl (qwa.r), achse.x, achse.y, achse.z);
    }
  return cvektor4 (2*acosl (qwa.r), achse.x, achse.y, achse.z);
  }
                                                                        // pq.r = -360°: Matrix ist die Punktspiegelungsmatrix (Inversion (-id))
cbasis3  matrixfromwinkelachse (const cvektor4 pq)
  {
  return matrixfromquaternion (quaternionfromwinkelachse (pq));
  }

cvektor4 quaternionfrommatrix (const cbasis3 &pdreh)  // Funktioniert bei 0° und 180° Drehungen
  {
  cvektor4 qwa (qwafrommatrix (pdreh));                                 // Mehrdeutigkeit der Achse bei 180° Drehungen
  if (qwa.r >= 1 - vquant)      // Drehung ist 0° Drehung
    return qwa;                // 1 zurückgeben
  cvektor3 achse (cvektor3 (qwa.i, qwa.j, qwa.ij));
  real k= sqrtl (1 - qwa.r*qwa.r)/abs (achse);
  return cvektor4 (qwa.r, k*achse);
  }

cvektor4 quaternionfrommatrix2 (const cbasis3 &pdreh)  // Kreuzproduktversion: versagt bei 180° Drehungen
  {
  real qr= sqrtl (1 + pdreh.x.x + pdreh.y.y + pdreh.z.z);
  if (qr <= vquant)                      // 180° Drehung
    return cvektor4 (0, 1, 0, 0);       // Bullshit zurückgeben
  real qi= (pdreh.y.z - pdreh.z.y)/qr;
  real qj= (pdreh.z.x - pdreh.x.z)/qr;
  real qij=(pdreh.x.y - pdreh.y.x)/qr;
  return 0.5*cvektor4 (qr, qi, qj, qij);
  }

cbasis3 matrixfromquaternion (const cvektor4 pq)   // Es kommen nur 2-fache Produkte vor, da mat (q) = mat (-q)
  {
  real x, y, z;
  x= 1 - 2*(pq.j*pq.j + pq.ij*pq.ij);
  y= 2*pq.r*pq.ij + 2*pq.i*pq.j;
  z= -2*pq.r*pq.j + 2*pq.i*pq.ij;
  cvektor3 m1 (x, y, z);
  x= -2*pq.r*pq.ij + 2*pq.i*pq.j;
  y= 1 - 2*(pq.i*pq.i + pq.ij*pq.ij);
  z= 2*pq.r*pq.i + 2*pq.j*pq.ij;
  cvektor3 m2 (x, y, z);
  x= 2*pq.r*pq.j + 2*pq.i*pq.ij;
  y= -2*pq.r*pq.i + 2*pq.j*pq.ij;
  z= 1 - 2*(pq.i*pq.i + pq.j*pq.j);
  cvektor3 m3 (x, y, z);
  cbasis3 mat (m1, m2, m3);
  mat= normiere (mat);
  return mat;
  }

cbasis3 matrixfromeulerwinkel (cvektor3 pflugw)
  {
  return normiere (getroty (pflugw.x)*getrotx (-pflugw.y)*getrotz (-pflugw.z));
  }

cvektor3 eulerwinkelfrommatrix (const cbasis3& pdm)
  {
  real laenge= 0;
  real vl= 0;
  real breite= 0;
  real vb= 0;
  real roll= 0;
  real vr= 0;

  // Breitengradberechnung
  vb= pdm.z.y;
  if (fabsl (vb) < 1)
    breite= asinl (vb);
  if (vb >= 1)
    breite= M_PIl/2;
  if (vl <= -1)
    breite= -M_PIl/2;

  // Längengrad- und Rollberechnung
  real cosbr= cosl (breite);
  if (cosbr > vquant)  // Breite nicht polar
    {
    // Längengradberechnung
    vl= pdm.z.z/cosbr;
    if (fabsl (vl) < 1)
      laenge= acosl (vl);
    if (vl >= 1)
      laenge= 0;
    if (vl <= -1)
      laenge= M_PIl;
    if (pdm.z.x < 0)
      laenge = -laenge;

    // Rollberechnung
    vr= pdm.y.y/cosbr;
    if (fabsl (vr) < 1)
      roll= acosl (vr);
    if (vr >= 1)
      roll= 0;
    if (vr <= -1)
      roll= M_PIl;
    if (pdm.x.y > 0)
      roll= -roll;
    }
  if (cosbr <= vquant)  // Spezialfall Breite polar
    {
    vl= pdm.x.x;
    if (fabsl (vl) < 1)
      laenge= acosl (vl);
    if (vl >= 1)
      laenge= 0;
    if (vl <= -1)
      laenge= M_PIl;
    if (pdm.x.z > 0)
      laenge= -laenge;
    roll= 0;
    }
  return cvektor3 (laenge, breite, roll);
  }

cvektor4 quaternionfromeulerwinkel (const cvektor3 pflugw)
  {
  cvektor3 wi (0.5*pflugw);
  cvektor4 q1 (cosl (wi.x), 0, sinl (wi.x), 0);
  cvektor4 q2 (cosl (wi.y), -sinl (wi.y), 0, 0);
  cvektor4 q3 (cosl (wi.z), 0, 0, -sinl (wi.z));
  cvektor4 ret= q1*q2*q3;
  if (ret.r >= 0)
    return ret;
  return -ret;
  }
