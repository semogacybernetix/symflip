// Ausgabe und Eingabe der Gruppenklasse auf das Terminal

#include "gruppecon.h"
#include <iostream>    //cout, printf, scanf, FILE*, fopen

void printv3liste (cvektor3liste& pvliste)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  for (integer lauf= 0; lauf < pvliste.anz; lauf++)
    printvektor3ord (pvliste.v[lauf], pvliste.ord[lauf]);
          printf ("\nVektor Liste: %10lld Elemente", pvliste.anz);
  fprintf (datei, "\nVektor Liste: %10lld Elemente", pvliste.anz);
  if (pvliste.uebergelaufen)
    {
            printf ("    übergelaufen");
    fprintf (datei, "    übergelaufen");
    }
          printf ("\n\n");
  fprintf (datei, "\n\n");
  fclose (datei);
  }

void printb3liste (cbasis3liste& pbliste)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  for (integer lauf= 0; lauf < pbliste.anz; lauf++)
    printbasis3dreh (pbliste.b[lauf], pbliste.ord[lauf], pbliste.zykel[lauf], pbliste.gepaart[lauf]);
    //printbasis3mat (pbliste.b[lauf], pbliste.ord[lauf], pbliste.zykel[lauf]);
  printf ("\nMatrix Liste: %10lld Elemente", pbliste.anz);
  fprintf (datei, "\nMatrix Liste: %10lld Elemente", pbliste.anz);
  if (pbliste.uebergelaufen)
    {
    printf ("    übergelaufen");
    fprintf (datei, "    übergelaufen");
    }
  printf ("\n\n");
  fprintf (datei, "\n\n");
  for (integer lauf= 0; lauf < pbliste.anz + 1; lauf++)
    if (pbliste.ordanz[lauf] > 0)
      {
      printf ("%5lld %5lld\n", pbliste.ordanz[lauf], lauf);
      fprintf (datei, "%5lld %5lld\n", pbliste.ordanz[lauf], lauf);
      }
  printf ("\n");
  fprintf (datei, "\n");
  fclose (datei);
  }

void printv4liste (cvektor4liste& pqliste)
  {
  FILE* datei= fopen ("quaternionenoutput.txt", "ab");
  for (integer lauf= 0; lauf < pqliste.anz; lauf++)
    printvektor4dreh (pqliste.v[lauf], pqliste.ord[lauf], pqliste.zykel[lauf], pqliste.gepaart[lauf]);
  printf ("\nQuaternionen Liste: %10lld Elemente", pqliste.anz);
  fprintf (datei, "\nQuaternionen Liste: %10lld Elemente", pqliste.anz);
  if (pqliste.uebergelaufen)
    {
    printf ("    übergelaufen");
    fprintf (datei, "    übergelaufen");
    }
  printf ("\n\n");
  fprintf (datei, "\n\n");
  for (integer lauf= 0; lauf < pqliste.anz + 1; lauf++)
    if (pqliste.ordanz[lauf] > 0)
      {
      printf ("%5lld %5lld\n", pqliste.ordanz[lauf], lauf);
      fprintf (datei, "%5lld %5lld\n", pqliste.ordanz[lauf], lauf);
      }
  printf ("\n\n");
  fprintf (datei, "\n\n");
  fclose (datei);
  }

