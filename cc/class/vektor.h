#ifndef vektor_h
#define vektor_h

#include <cmath>  // M_PIl, cosl, sinl

typedef signed long long integer;
typedef long double real;

struct cvektor2
  {
  real x, y;

  cvektor2 ();
  cvektor2 (const real &px, const real &py);
  };

struct cbasis2
  {
  cvektor2 x, y;

  cbasis2 ();
  cbasis2 (const cvektor2 &px, const cvektor2 &py);
  };

struct cvektor3
  {
  real x, y, z;

  cvektor3 ();
  cvektor3 (const real &pr);
  cvektor3 (const real &px, const real &py, const real &pz);
  };

struct cbasis3
  {
  cvektor3 x, y, z;

  cbasis3 ();
  cbasis3 (const real &pk);
  cbasis3 (const cvektor3 &pv);
  cbasis3 (const cvektor3 &px, const cvektor3 &py, const cvektor3 &pz);
  };

struct cvektor4
  {
  real r, i, j, ij;

  cvektor4 ();
  cvektor4 (const real &pr, const real &pi, const real &pj, const real &pij);
  cvektor4 (const real &pr, const cvektor3 &pv);
  };

struct cvektor7
  {
  real r, i, j, ij, ji, iji, jij;

  cvektor7 ();
  cvektor7 (const real &pr, const real &pi, const real &pj, const real &pij, const real &pji, const real &piji, const real &pjij);
  };

const cvektor2 nullc (0, 0);
const cvektor2 einsc (1, 0);
const cvektor2 ic (0, 1);

const cvektor3 nullvektor3 (0, 0, 0);
const cvektor3 xvektor3 (1, 0, 0);
const cvektor3 yvektor3 (0, 1, 0);
const cvektor3 zvektor3 (0, 0, 1);

const cvektor4 nullq (0, 0, 0, 0);
const cvektor4 einsq (1, 0, 0, 0);
const cvektor4 iq (0, 1, 0, 0);
const cvektor4 jq (0, 0, 1, 0);
const cvektor4 ijq (0, 0, 0, 1);

const cbasis2 einsbasis2 (einsc, ic);
const cbasis3 einsbasis3 (xvektor3, yvektor3, zvektor3);

//--------------------- integer Funktionen ------------------------------------------------------------------------------

void vertausche (integer& a, integer& b);
integer max (integer a, integer b);
void savemax (integer& pmax, integer pr);

//--------------------- real Funktionen ------------------------------------------------------------------------------

void vertausche (real& a, real& b);

//--------------------- cvektor2 Vektor Funktionen ------------------------------------------------------------------------------

real     norm    (const cvektor2 pv);
real     abs     (const cvektor2 pv);
cvektor2 senk    (const cvektor2 &pv);

//--------------------- cvektor2 Funktionen für komplexe Zahlen -----------------------------------------------------------------

cvektor2 kw      (const cvektor2 pv);
cvektor2 polar   (const cvektor2 pv);
cvektor2 kartes  (const cvektor2 pv);
cvektor2 exp     (const cvektor2 pv);
cvektor2 log     (const cvektor2 pv);
cvektor2 cosh    (const cvektor2 pv);
cvektor2 sinh    (const cvektor2 pv);
cvektor2 cos     (const cvektor2 pv);
cvektor2 sin     (const cvektor2 pv);
cvektor2 sqrt    (const cvektor2 pv);

//---------- cvektor2 Vektor Operatoren ----------------------------------------------------------------------------------------------

integer  operator == (const cvektor2 &pv1, const cvektor2 &pv2);
cvektor2 operator -  (const cvektor2 &pv);
cvektor2 operator +  (const cvektor2 &pv1, const cvektor2 &pv2);
cvektor2 operator -  (const cvektor2 &pv1, const cvektor2 &pv2);
cvektor2 operator *  (const real &pf, const cvektor2 &pv);
cvektor2 operator /  (const cvektor2 &pv, const real &pf);
real     operator %  (const cvektor2 &pv1, const cvektor2 &pv2);

// ------------- cvektor2 Operatoren für komplexe Zahlen ---------------------------------------------------------

cvektor2 operator * (const cvektor2 &pv1, const cvektor2 &pv2);
cvektor2 operator / (const cvektor2 &pv1, const cvektor2 &pv2);
cvektor2 operator / (const real &pf, const cvektor2 &pv);
cvektor2 operator ^ (const cvektor2 &pv1, const cvektor2 &pv2);
real     operator | (const cvektor2 &pv1, const cvektor2 &pv2);

//---------- cvektor2, cbasis2 --------------------------------------------------------------------------------

// Matrix mal Vektor
cvektor2 operator * (const cbasis2  &pb, const cvektor2 &pv);
cvektor2 operator & (const cvektor2 &pv, const cbasis2  &pb);
cvektor2 operator / (const cvektor2 &pv, const cbasis2  &pb);

//---------- cbasis2 Funktionen -------------------------------------------------------------------------------------------
real     det     (const cbasis2 &pb);
cbasis2  transp  (const cbasis2 &pb);

//---------- cbasis2 Operatoren -------------------------------------------------------------------------------------------

// transponierte inverse Matrix bestimmen (senkrechter Raum zur Basis)
cbasis2 operator ~ (const cbasis2  &pb);

// Matrixaddition
cbasis2 operator + (const cbasis2 &pb1, const cbasis2 &pb2);
cbasis2 operator - (const cbasis2 &pb1, const cbasis2 &pb2);

// Skalarmultiplikation
cbasis2 operator * (const real &pf, const cbasis2 &pb);
cbasis2 operator / (const cbasis2 &pb, const real &pf);

// Matrixmultiplikation
cbasis2 operator * (const cbasis2 &pb1, const cbasis2 &pb2);
cbasis2 operator & (const cbasis2 &pb1, const cbasis2 &pb2);
cbasis2 operator / (const cbasis2 &pb1, const cbasis2 &pb2);
cbasis2 operator | (const cbasis2 &pb1, const cbasis2 &pb2);

//---------- cvektor3 Funktionen ------------------------------------------------------------------------------------------

void vertausche (cvektor3& a, cvektor3& b);

real     norm       (const cvektor3 &pv);
real     abs        (const cvektor3 &pv);
cvektor3 senk       (const cvektor3 &pv);
cvektor3 normiere   (const cvektor3 &pv);
cvektor3 orientiere (const cvektor3 &pv);

// Betrag des Winkels zwischen 2 Vektoren ermitteln [0-pi]
real     winkelb  (const cvektor3 &pv1, const cvektor3 &pv2);
// Betrag des Winkels zwischen 2 Geraden ermitteln  [0-pi/2]
real     winkelg  (const cvektor3 &pv1, const cvektor3 &pv2);

//---------- cvektor3 Operatoren ------------------------------------------------------------------------------------------

// Vektoren vergleichen
integer operator == (const cvektor3 &pv1, const cvektor3 &pv2);

// Vorzeichen des Vektors ändern
cvektor3 operator - (const cvektor3 &pv);

// Vektoradditon
cvektor3 operator + (const cvektor3 &pv1, const cvektor3 &pv2);
cvektor3 operator - (const cvektor3 &pv1, const cvektor3 &pv2);

// Skalarmultiplikation
cvektor3 operator * (const real &pf, const cvektor3 &pv);
cvektor3 operator / (const cvektor3 &pv, const real &pf);

// Skalarprodukt
real operator % (const cvektor3 &pv1, const cvektor3 &pv2);

// Kreuzprodukt (senkrecht stehenden Vektor zu 2 Vektoren in positiver Orientierung ermitteln)
cvektor3 operator ^ (const cvektor3 &pv1, const cvektor3 &pv2);

// einen Vektor aus der Ebene wählen, auf dem der Vektor senkrecht steht
cvektor3 operator ~ (const cvektor3 &pv);

//---------- cvektor3, cbasis3 -----------------------------------------------------------------------------------

cvektor3 operator * (const cbasis3  &pb, const cvektor3 &pv);
cvektor3 operator & (const cvektor3 &pv, const cbasis3  &pb);
cvektor3 operator / (const cvektor3 &pv, const cbasis3  &pb);

//---------- cbasis3 Funktionen ------------------------------------------------------------------------------------------

real     norm      (cbasis3 pb);
real     abs       (cbasis3 pb);
real     det       (const cbasis3 &pb);
cbasis3  normiere  (const cbasis3 &pb);
cbasis3  normiere2 (const cbasis3 &pb);
cbasis3  transp    (const cbasis3 &pb);
cvektor3 geteigen  (const cbasis3 &pb);
cbasis3  getrotx   (const real &pf);
cbasis3  getroty   (const real &pf);
cbasis3  getrotz   (const real &pf);
cbasis3  getspiegbasis (const cvektor3 &pv);
cbasis3  getdrehspiegbasis (const cvektor3 &pv);
cvektor3 getspiegachse (const cbasis3 &pb);
integer  aehnlich  (const cbasis3 &pb1, const cbasis3 &pv2);


//---------- cbasis3 Operatoren ------------------------------------------------------------------------------------------

// transponierte inverse Matrix bestimmen
cbasis3  operator ~ (const cbasis3  &pb);

// Matrixaddition (Vektoraddition von Matrizen)
cbasis3  operator - (const cbasis3  &pb);
cbasis3  operator + (const cbasis3 &pb1, const cbasis3 &pb2);
cbasis3  operator - (const cbasis3 &pb1, const cbasis3 &pb2);

// Skalarmultiplikation (Skalarmultiplikation von Matrizen)
cbasis3  operator * (const real &pf, const cbasis3 &pb);
cbasis3  operator / (const cbasis3 &pb, const real &pf);

// Matrixmultiplikation (Körperoperationen von Matrizen)
cbasis3  operator * (const cbasis3 &pb1, const cbasis3 &pb2);
cbasis3  operator & (const cbasis3 &pb1, const cbasis3 &pb2);
cbasis3  operator / (const cbasis3 &pb1, const cbasis3 &pb2);
cbasis3  operator | (const cbasis3 &pb1, const cbasis3 &pb2);

//---------- cvektor4 Vektor Funktionen -----------------------------------------------------------------------------------------------

real     norm     (const cvektor4 &pv);
real     abs      (const cvektor4 &pv);
cvektor4 senk     (const cvektor4 &pv);
cvektor4 normiere (const cvektor4 &pv);

// Kreuzfunktion (einen senkrecht stehenden Vektor ermitteln)
cvektor4 operator ~ (const cvektor4 &pv);

//---------- cvektor4 Funktionen für Quaternionen -------------------------------------------------------------------------------------

cvektor4 kw       (const cvektor4 &pv);
cvektor3 getachse (const cvektor4 &pv);
integer  aehnlich (const cvektor4 &pv1, const cvektor4 &pv2);

//---------- cvektor4 Vektor Operatoren ------------------------------------------------------------------------------------------------

// Vergleich
integer operator == (const cvektor4 &pv1, const cvektor4 &pv2);

// Vorzeichen ändern
cvektor4 operator - (const cvektor4 &pv);

// Vektorraddition
cvektor4 operator + (const cvektor4 &pv1, const cvektor4 &pv2);
cvektor4 operator - (const cvektor4 &pv1, const cvektor4 &pv2);

// Skalarmultiplikation
cvektor4 operator * (const real &pf, const cvektor4 &pv);
cvektor4 operator / (const cvektor4 &pv, const real &pf);

// Skalarprodukt
real     operator % (const cvektor4 &pv1, const cvektor4 &pv2);


//----------------------------- Operatoren für Quaternionen --------------------------------------------------------
// Quaternionenmultiplikation
cvektor4 operator * (const cvektor4 &pv1, const cvektor4 &pv2);
cvektor4 operator / (const cvektor4 &pv1, const cvektor4 &pv2);
cvektor4 operator | (const cvektor4 &pv1, const cvektor4 &pv2);
cvektor3 operator * (const cvektor4 &pq, const cvektor3 &pv);    // 3D-Vektor mit Drehquaternion drehen

//---------- cvektor7 Vektor Operatoren ------------------------------------------------------------------------------------------------

//----------------------------- Operatoren für Heptanionen --------------------------------------------------------
// Heptanionenmultiplikation
cvektor7 operator * (const cvektor7 &pv1, const cvektor7 &pv2);

// ------------------------------ Drehdarstellungen umrechnen ------------------------------------------------------------------------

cvektor4 quaternionfromwinkelachse (const cvektor4 pdw);
cvektor4 winkelachsefromquaternion (const cvektor4 pq);

cbasis3  matrixfromquaternion (const cvektor4 pq);
cvektor4 quaternionfrommatrix (const cbasis3& pdm);
cvektor4 quaternionfrommatrix2 (const cbasis3 &pdreh);

cvektor4 winkelachsefrommatrix (const cbasis3 &pdreh);
cbasis3  matrixfromwinkelachse (const cvektor4 pq);

cvektor3 eulerwinkelfrommatrix (const cbasis3& pdm);
cbasis3  matrixfromeulerwinkel (cvektor3 pflugw);

cvektor4 quaternionfromeulerwinkel (const cvektor3 pflugw);

#endif
