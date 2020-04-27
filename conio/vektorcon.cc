// Ausgabe und Eingabe der Vektorklasse auf das Terminal

#include "vektorcon.h"
#include <iostream>     // cout, printf, scanf

//------------------------------------------------------------------------ Ausgabe ---------------------------------------------------------------------------------------------------

void printtext (const char* ptext)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  printf ("%s", ptext);
  fprintf (datei, "%s", ptext);
  fclose (datei);
  }
  
void printinteger (integer pn)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  printf ("%lld\n", pn);
  fprintf (datei, "%lld\n", pn);
  fclose (datei);
  }

void printreal (real pr)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  printf ("%12.9Lf \n", pr);
  fprintf (datei, "%12.9Lf \n", pr);
  fclose (datei);
  }

void printvektor3ord (cvektor3 pv, integer pord)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("%12.9Lf %12.9Lf %12.9Lf %8lld\n", pv.x, pv.y, pv.z, pord);
  fprintf (datei, "%12.9Lf %12.9Lf %12.9Lf %8lld\n", pv.x, pv.y, pv.z, pord);
  fclose (datei);
  }

void printvektor3wi (cvektor3 pv, real wi)
  {
  wi*= 180/M_PIl;
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("    %12.9Lf %12.9Lf %12.9Lf %10.3Lf°\n", pv.x, pv.y, pv.z, wi);
  fprintf (datei, "    %12.9Lf %12.9Lf %12.9Lf %10.3Lf°\n", pv.x, pv.y, pv.z, wi);
  fclose (datei);
  }

void printbasis3mat (cbasis3 pb, integer pord, integer pzykel)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("        %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %12.9Lf  %4.1Lf%6lld%24lld\n", pb.x.x, pb.x.y, pb.x.z, pb.y.x, pb.y.y, pb.y.z, pb.z.x, pb.z.y, pb.z.z, det (pb), pord, pzykel);
  fprintf (datei, "        %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %15.9Lf  %4.1Lf%6lld%24lld\n", pb.x.x, pb.x.y, pb.x.z, pb.y.x, pb.y.y, pb.y.z, pb.z.x, pb.z.y, pb.z.z, det (pb), pord, pzykel);
  fclose (datei);
  }

void printbasis3dreh (cbasis3 pb, integer pord, integer pzykel, integer ppaar)
  {
  cvektor4 aw;
  real detpb= det (pb);
  if (detpb > 0)
    aw= winkelachsefrommatrix (pb);
    else
    aw= winkelachsefrommatrix (-pb);  // Inversion aus der Drehspiegelungsmatrix rausholen und die übrigbleibende Drehmatrix weiterverarbeiten
  real wi= aw.r*180/M_PIl;
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("    %12.9Lf %12.9Lf %12.9Lf %10.3Lf  %4.1Lf%6lld%6lld%24lld\n", aw.i, aw.j, aw.ij, wi, det (pb), ppaar, pord, pzykel);
  fprintf (datei, "    %12.9Lf %12.9Lf %12.9Lf %10.3Lf  %4.1Lf%6lld%6lld%24lld\n", aw.i, aw.j, aw.ij, wi, det (pb), ppaar, pord, pzykel);
  fclose (datei);
  }

void printvektor4quat (cvektor4 pv, integer pord, integer pzykel)
  {
  cvektor4 aw= winkelachsefromquaternion (pv);
  real wi= aw.r*180/M_PIl;
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("%24.9Lf %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %12.9Lf %10.3Lf%6lld%24lld\n", pv.r, pv.i, pv.j, pv.ij, aw.i, aw.j, aw.ij, wi, pord, pzykel);
  fprintf (datei, "%24.9Lf %12.9Lf %12.9Lf %12.9Lf        %12.9Lf %12.9Lf %12.9Lf %10.3Lf%6lld%24lld\n", pv.r, pv.i, pv.j, pv.ij, aw.i, aw.j, aw.ij, wi, pord, pzykel);
  fclose (datei);
  }

void printvektor4dreh (cvektor4 pv, integer pord, integer pzykel, integer ppaar)
  {
  cvektor4 aw= winkelachsefromquaternion (pv);
  real wi= aw.r*180/M_PIl;
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
          printf ("    %12.9Lf %12.9Lf %12.9Lf %10.3Lf  %3lld    %6lld%24lld\n", aw.i, aw.j, aw.ij, wi, ppaar, pord, pzykel);
  fprintf (datei, "    %12.9Lf %12.9Lf %12.9Lf %10.3Lf  %3lld    %6lld%24lld\n", aw.i, aw.j, aw.ij, wi, ppaar, pord, pzykel);
  fclose (datei);
  }

//------------------------------------------------------------------------ Eingabe ---------------------------------------------------------------------------------------------------

void realeingabe (real& pv)
  {
  integer scanret;
  printf ("\n");
  printf ("r:  ");
  scanret= scanf ("%Lf", &pv);
  scanret++; //scanret benutzen damit Compiler nicht meckert
  }

void vektor3eingabe (cvektor3& pv)
  {
  integer scanret;
  printf ("\n");
  printf ("x:  ");
  scanret= scanf ("%Lf", &pv.x);
  printf ("y:  ");
  scanret= scanf ("%Lf", &pv.y);
  printf ("z:  ");
  scanret= scanf ("%Lf", &pv.z);
  printf ("\n");
  scanret++; //scanret benutzen damit Compiler nicht meckert
  }

void quaternioneneingabe (cvektor4& pv)
  {
  integer scanret;
  printf ("\n");
  printf ("r:  ");
  scanret= scanf ("%Lf", &pv.r);
  printf ("i:  ");
  scanret= scanf ("%Lf", &pv.i);
  printf ("j:  ");
  scanret= scanf ("%Lf", &pv.j);
  printf ("ij  ");
  scanret= scanf ("%Lf", &pv.ij);
  printf ("\n");
  scanret++; //scanret benutzen damit Compiler nicht meckert
  }

cvektor4 winkelachseeingabe ()
  {
  integer scanret;
  cvektor3 achse (0, 0, 0);
  long double wi;
  printf ("\n");
  printf ("Drehachse x:       ");
  scanret= scanf ("%Lf", &achse.x);
  printf ("Drehachse y:       ");
  scanret= scanf ("%Lf", &achse.y);
  printf ("Drehachse z:       ");
  scanret= scanf ("%Lf", &achse.z);
  achse= normiere (achse);
  printf ("Drehwinkel [°]:    ");
  scanret= scanf ("%Lf", &wi);
  printf ("\n");
  wi= wi/180*M_PIl;
  return cvektor4 (wi, achse);
  scanret++; //scanret benutzen damit Compiler nicht meckert
  }

