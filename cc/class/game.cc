#include "game.h"
#include "gruppe.h"        // Besuchsliste der Kugel beim Spiel

#include <iostream>        // cout, printf, scanf
#include <unistd.h>        // usleep
#include <sys/times.h>     // tms, times
#include <thread>

using namespace std;  // cout

tms flugsimuzeit;

//--------------------------------- Thread -------------------------------------------------

signed long rektiefe;         // globale Variable für threadrekursiv (lässt sich nicht lokal definieren, weil sie threadübergreifend sein muss und nicht pro Instanz)
cflugsimu* flugsimuthread;

void arbeit (signed long pthreadnr)
  {
  flugsimuthread->welttoscreenthread (pthreadnr);
  }

void* threadrekursiv (void*)
  {
  signed long lokaltiefe= rektiefe;   // rektiefe kopieren, da sie jederzeit von einer anderen Instanz von threadrekursiv geändert werden kann
  
                                      // Für den Fall, dass threadrekursiv von außen mit weniger als einem Thread aufgerufen wird
  if (lokaltiefe < 1)
    {
    return 0;
    }

  pthread_t threadid;                 // id des erzeugten Threads
  signed long threadreturnwert;       // Rückgabe ob Threaderzeugung funktioniert hat
  
                                      // neuen Aufruf von threadrekursiv erzeugen falls Tiefe mindestens 2 ist
  if (lokaltiefe > 1)
    {
    rektiefe--;
    threadreturnwert= pthread_create (&threadid, NULL, threadrekursiv, NULL);
    if (threadreturnwert != 0)
      printf ("threadrekursiv-%ld:----Aufruf threadrekursiv mit Tiefe  %3ld fehlgeschlagen\n", lokaltiefe, lokaltiefe - 1);
    }
  
                                     // Arbeitsfunktion aufrufen
  arbeit (lokaltiefe);
  
                                     // auf Beendigung von neuem Aufruf warten
  if (lokaltiefe > 1)
    {
    pthread_join (threadid, NULL);
    }
  return 0;
  }

//--------------------------------- Thread-Ende -------------------------------------------------------------------------------------

//---------------------------------------------------- cflugsimu ----------------------------------------------------------

cflugsimu::cflugsimu (cwelt* pwelt, clkeyboard* plkeyboard, clscreen* plscreen)
  : welt (pwelt), keyboard (plkeyboard), screen (plscreen),
    xoff ((1 - screen->xanz)/(real) (2)), yoff ((1 - screen->yanz)/(real) (2)),
    bewstep (10), drehstep (1000)
  {
  flugsimuthread= this;
  threadanz= 1;
  framerate= 10;
  flugsimutps= sysconf (_SC_CLK_TCK);
  flugsimudatum= times (&flugsimuzeit);
  
  // pixelarray erzeugen
  pixelpos= 0;
  srand48 (56);
  integer pixanz= screen->xanz*screen->yanz;
  pixels= new cpixel[pixanz];
  for (integer n= 0; n < pixanz; n++)
    {
    pixels[n].x= n % screen->xanz;
    pixels[n].y= n / screen->xanz;
    }
  for (integer n2= 0; n2 < pixanz; n2++)
    {
    integer z= random () % pixanz;
    cpixel s= pixels[n2];
    pixels[n2]= pixels[z];
    pixels[z]= s;
    }
  }

void cflugsimu::welttoscreenl ()    // linearer Pixeldurchgang
  {
  for (integer ypp= 0; ypp < screen->yanz; ++ypp)
    for (integer xpp= 0; xpp < screen->xanz; ++xpp)
      {
      cvektor3 fb= welt->getpunkt (cvektor2 (xpp + xoff, ypp + yoff));
      real r= fb.x*screen->rmax;
      real g= fb.y*screen->gmax;
      real b= fb.z*screen->bmax;
      screen->putpixel (xpp, ypp, r, g, b);
      }
  }

void cflugsimu::welttoscreenz ()        // zufallsreihenfolgiger Pixeldurchgang
  {
  integer nanz= screen->xanz*screen->yanz;
  integer xpp, ypp;
  cvektor3 fb;
  for (integer n= 0; n < nanz; n++)
    {
    xpp= pixels[n].x;
    ypp= pixels[n].y;
    fb= welt->getpunkt (cvektor2 (xpp + xoff, ypp + yoff));
    real r= fb.x*screen->rmax;
    real g= fb.y*screen->gmax;
    real b= fb.z*screen->bmax;
    screen->putpixel (xpp, ypp, r, g, b);
    }
  }

void cflugsimu::welttoscreentakt ()        // zufallsreihenfolgiger Pixeldurchgang
  {
  integer nanz= screen->xanz*screen->yanz;
  integer ticks= llrint (real (flugsimutps)/framerate);
  integer xpp, ypp;
  cvektor3 fb;
  
  while ((times (&flugsimuzeit) - flugsimudatum) < ticks)
    {
    xpp= pixels[pixelpos].x;
    ypp= pixels[pixelpos].y;
    fb= welt->getpunkt (cvektor2 (xpp + xoff, ypp + yoff));
    real r= fb.x*screen->rmax;
    real g= fb.y*screen->gmax;
    real b= fb.z*screen->bmax;
    screen->putpixel (xpp, ypp, r, g, b);
    pixelpos= (pixelpos + 1) % nanz;
    }
  flugsimudatum= flugsimudatum + ticks;
  }

void cflugsimu::welttoscreenthread (integer pthreadnr)
  {
//  printf ("Threadnr    %lld\n", pthreadnr);
  integer nanz= screen->xanz*screen->yanz;
  integer panz= nanz/threadanz;
  integer ug= (pthreadnr - 1)*panz;
  integer og= ug + panz;
  integer xpp, ypp;
  cvektor3 fb;
  for (integer n= ug; n < og; n++)
    {
//    xpp= pixels[n].x;
//    ypp= pixels[n].y;
    xpp= n % screen->xanz;
    ypp= n/screen->xanz;
    fb= welt->getpunkt (cvektor2 (xpp + xoff, ypp + yoff));
    real r= fb.x*screen->rmax;
    real g= fb.y*screen->gmax;
    real b= fb.z*screen->bmax;
  //  printf ("putpixel   thread: %lld  xpp: %lld  ypp: %lld\n", pthreadnr, xpp, ypp);
    screen->putpixel (xpp, ypp, r, g, b);
    }
  }

// Tastenbelegung

void cflugsimu::fliegethread ()   // Multithreadfliegen
  {
  
  cvektor3 flugw (0, 0, 0);
  cvektor4 drehaw;
  cbasis3 achsbasis;

  tms zeit;
  real ticksps= real (sysconf (_SC_CLK_TCK));
  clock_t ticks= times (&zeit);
  cout << "ticks/sek: " << ticksps << endl;

  integer koerper= 0;
  real framezeit;
  keyboard->putkey (19, 5, 1);
    do
    {
    if (keyboard->getkey (19, 5))
      {
      rektiefe= threadanz;
//      printf ("vor arbeit %ld\n", rektiefe);
      threadrekursiv (0);
      screen->flush ();
      //welttoscreenz ();
      framezeit= (times (&zeit) - ticks)/ticksps;
      //cout << "Zeit: " << framezeit << "  fps: " << 1/framezeit << endl;
      printf ("Zeit: %5.2Lf  fps: %5.2Lf\n", framezeit, 1/framezeit);
      flugw= eulerwinkelfrommatrix (welt->augbasis);
      flugw= 180/M_PI*flugw;
      drehaw= winkelachsefrommatrix (welt->augbasis);

      //nwinkel= 180/M_PI*drehaw.r;
      // Zeile löschen \r
      //printf ("Eulerwinkel: %15.9Lf %15.9Lf %15.9Lf          drehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", flugw.x, flugw.y, flugw.z, drehaw.i, drehaw.j, drehaw.ij, nwinkel);
      //printf ("                          augdrehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", welt->augdrehaw.i, welt->augdrehaw.j, welt->augdrehaw.ij, welt->augdrehaw.r/M_PI*180);
      //drehmatrixausgabe (welt->augbasis);

      fflush (stdout);
      ticks= times (&zeit);
      }
      else
        usleep (50000);

    keyboard->flush ();
    if (koerper == 0)
      {
      // Bewegungen
      if (keyboard->getkey (2, 2)) welt->verschiebeauge (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->verschiebeauge (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep/3, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep/3, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep/3, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep/3, 0));

      // Bewegungszoomfaktor ändern
      if (keyboard->getkey (5, 2)) bewstep/= 2.5;
      if (keyboard->getkey (5, 3)) bewstep*= 2.5;

      // Drehzoomquotient ändern
      if (keyboard->getkey (7, 2)) drehstep*= 2;
      if (keyboard->getkey (7, 3)) drehstep/= 2;

      // Drehungen
      if (keyboard->getkey (8, 2))
        {
        welt->dreheaugex (M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (8, 3))
        {
        welt->dreheaugex (-M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 2))
        {
        welt->dreheaugey (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 3))
        {
        welt->dreheaugey (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 2))
        {
        welt->dreheaugez (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 3))
        {
        welt->dreheaugez (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (7, 1)) welt->dreheaugenorm (M_PI/drehstep);
      if (keyboard->getkey (8, 1)) welt->dreheaugeein (M_PI/drehstep);
      if (keyboard->getkey (9, 1)) welt->dreheaugeachse (-M_PI/drehstep);
      if (keyboard->getkey (10, 1)) welt->dreheaugeachse (M_PI/drehstep);
      }
      if (keyboard->getkey (1, 1))
        {
        koerper= 0; usleep (250000);
        }
    }
  while (!keyboard->getkey (0, 0));
  }

void cflugsimu::fliege ()   // ohne Körper drehen zu können
  {
  cvektor3 flugw (0, 0, 0);
  cvektor4 drehaw;
  cbasis3 achsbasis;

  tms zeit;
  real ticksps= real (sysconf (_SC_CLK_TCK));
  clock_t ticks= times (&zeit);
  cout << "ticks/sek: " << ticksps << endl;
  cout << "ticks/sek: " << ticksps << endl;

  integer koerper= 0;
  real framezeit;
  keyboard->putkey (19, 5, 1);
    do
    {
    if (keyboard->getkey (19, 5))
      {
      welttoscreenz ();
      screen->flush ();
      framezeit= (times (&zeit) - ticks)/ticksps;
      //cout << "Zeit: " << framezeit << "  fps: " << 1/framezeit << endl;
      printf ("Zeit: %5.2Lf  fps: %5.2Lf\n", framezeit, 1/framezeit);
      flugw= eulerwinkelfrommatrix (welt->augbasis);
      flugw= 180/M_PI*flugw;
      drehaw= winkelachsefrommatrix (welt->augbasis);

      //nwinkel= 180/M_PI*drehaw.r;
      // Zeile löschen \r
      //printf ("Eulerwinkel: %15.9Lf %15.9Lf %15.9Lf          drehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", flugw.x, flugw.y, flugw.z, drehaw.i, drehaw.j, drehaw.ij, nwinkel);
      //printf ("                          augdrehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", welt->augdrehaw.i, welt->augdrehaw.j, welt->augdrehaw.ij, welt->augdrehaw.r/M_PI*180);
      //drehmatrixausgabe (welt->augbasis);

      fflush (stdout);
      ticks= times (&zeit);
      }
      else
        usleep (50000);

    keyboard->flush ();
    if (koerper == 0)
      {
      // Bewegungen
      if (keyboard->getkey (2, 2)) welt->verschiebeauge (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->verschiebeauge (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep/3, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep/3, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep/3, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep/3, 0));

      // Bewegungszoomfaktor ändern
      if (keyboard->getkey (1, 2)) bewstep*= 2.5;
      if (keyboard->getkey (1, 3)) bewstep/= 2.5;

      // Drehzoomquotient ändern
      if (keyboard->getkey (11, 2)) drehstep/= 2;
      if (keyboard->getkey (11, 3)) drehstep*= 2;

      // Drehungen
      if (keyboard->getkey (8, 2))
        {
        welt->dreheaugex (M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (8, 3))
        {
        welt->dreheaugex (-M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 2))
        {
        welt->dreheaugey (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 3))
        {
        welt->dreheaugey (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 2))
        {
        welt->dreheaugez (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 3))
        {
        welt->dreheaugez (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (7, 1)) welt->dreheaugenorm (M_PI/drehstep);
      if (keyboard->getkey (8, 1)) welt->dreheaugeein (M_PI/drehstep);
      if (keyboard->getkey (9, 1)) welt->dreheaugeachse (-M_PI/drehstep);
      if (keyboard->getkey (10, 1)) welt->dreheaugeachse (M_PI/drehstep);
      }
      if (keyboard->getkey (1, 1))
        {
        koerper= 0; usleep (250000);
        }
    }
  while (!keyboard->getkey (0, 0));
  }

void cflugsimu::fliegespiel (cbasis3& spiegelebenen, ckoerper* bewkugel)
  {
  cvektor3 flugw (0, 0, 0);
  cvektor4 drehaw;
  cbasis3 achsbasis;
  
  // Bewegungskugel initialisieren
  cvektor3 bewkugelpos;
  integer zeitpos;
  cbasis3 spiegbasis1, spiegbasis2, spiegbasis3;
  spiegbasis1= getspiegbasis (spiegelebenen.x);
  spiegbasis2= getspiegbasis (spiegelebenen.y);
  spiegbasis3= getspiegbasis (spiegelebenen.z);

  tms zeit;
  real ticksps= real (sysconf (_SC_CLK_TCK));
  clock_t ticks= times (&zeit);
  cout << "ticks/sek: " << ticksps << endl;
  cout << "ticks/sek: " << ticksps << endl;

  integer koerper= 0;
  real framezeit;
  keyboard->putkey (19, 5, 1);
    do
    {
    if (1)
      {
      welt->aktualisiere ();
      welttoscreenz ();
      screen->flush ();
      framezeit= (times (&zeit) - ticks)/ticksps;
      zeitpos= times (&zeit);
      //cout << "Zeit: " << framezeit << "  fps: " << 1/framezeit << endl;
      printf ("Zeit: %5.2Lf  fps: %5.2Lf\n", framezeit, 1/framezeit);
      flugw= eulerwinkelfrommatrix (welt->augbasis);
      flugw= 180/M_PI*flugw;
      drehaw= winkelachsefrommatrix (welt->augbasis);

      //nwinkel= 180/M_PI*drehaw.r;
      // Zeile löschen \r
      //printf ("Eulerwinkel: %15.9Lf %15.9Lf %15.9Lf          drehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", flugw.x, flugw.y, flugw.z, drehaw.i, drehaw.j, drehaw.ij, nwinkel);
      //printf ("                          augdrehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", welt->augdrehaw.i, welt->augdrehaw.j, welt->augdrehaw.ij, welt->augdrehaw.r/M_PI*180);
      //drehmatrixausgabe (welt->augbasis);

      fflush (stdout);
      ticks= times (&zeit);
      }
      else
        usleep (50000);

    keyboard->flush ();
    if (koerper == 0)
      {
      // Bewegungen
      if (keyboard->getkey (2, 2)) welt->verschiebeauge (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->verschiebeauge (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep/3, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep/3, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep/3, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep/3, 0));

      // Bewegungszoomfaktor ändern
      if (keyboard->getkey (1, 2)) bewstep*= 2.5;
      if (keyboard->getkey (1, 3)) bewstep/= 2.5;

      // Drehzoomquotient ändern
      if (keyboard->getkey (11, 2)) drehstep/= 2;
      if (keyboard->getkey (11, 3)) drehstep*= 2;

      // Drehungen
      if (keyboard->getkey (8, 2))
        {
        welt->dreheaugex (M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (8, 3))
        {
        welt->dreheaugex (-M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 2))
        {
        welt->dreheaugey (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 3))
        {
        welt->dreheaugey (M_PI/drehstep/0.7);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 2))
        {
        welt->dreheaugez (M_PI/drehstep/0.7);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 3))
        {
        welt->dreheaugez (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      
      // an den Spiegelebenen spiegeln
      if (keyboard->getkey (1, 1))
        {
        bewkugelpos= bewkugel->stoppos;
        bewkugel->startpos= bewkugelpos;
        bewkugelpos= spiegbasis1*bewkugelpos;
        bewkugel->stoppos= bewkugelpos;
        bewkugel->startzeit= zeitpos;
        bewkugel->stopzeit= bewkugel->startzeit + 500;
        }
      if (keyboard->getkey (2, 1))
        {
        bewkugelpos= bewkugel->stoppos;
        bewkugel->startpos= bewkugelpos;
        bewkugelpos= spiegbasis2*bewkugelpos;
        bewkugel->stoppos= bewkugelpos;
        bewkugel->startzeit= zeitpos;
        bewkugel->stopzeit= bewkugel->startzeit + 500;
        }
      if (keyboard->getkey (3, 1))
        {
        bewkugelpos= bewkugel->stoppos;
        bewkugel->startpos= bewkugelpos;
        bewkugelpos= spiegbasis3*bewkugelpos;
        bewkugel->stoppos= bewkugelpos;
        bewkugel->startzeit= zeitpos;
        bewkugel->stopzeit= bewkugel->startzeit + 500;
        }
     
      /*
      if (keyboard->getkey (2, 1)) welt->dreheaugenorm (M_PI/drehstep);
      if (keyboard->getkey (3, 1)) welt->dreheaugenorm (M_PI/drehstep);
      */
      
      if (keyboard->getkey (7, 1)) welt->dreheaugenorm (M_PI/drehstep);
      if (keyboard->getkey (8, 1)) welt->dreheaugeein (M_PI/drehstep);
      if (keyboard->getkey (9, 1)) welt->dreheaugeachse (-M_PI/drehstep);
      if (keyboard->getkey (10, 1)) welt->dreheaugeachse (M_PI/drehstep);
      }
      if (keyboard->getkey (1, 1))
        {
        koerper= 0; usleep (250000);
        }
    }
  while (!keyboard->getkey (0, 0));
  }

void cflugsimu::fliegespieltakt (cbasis3& spiegelebenen, ckoerper* bewkugel)
  {
  cvektor3 flugw (0, 0, 0);
  cvektor4 drehaw;
  cbasis3 achsbasis;

  // Bewegungskugel initialisieren
  cvektor3 bewkugelpos;
  cvektor3liste besuchsliste (1000);
  real zeitpos;
  cbasis3 spiegbasis1, spiegbasis2, spiegbasis3;
  spiegbasis1= getspiegbasis (spiegelebenen.x);
  spiegbasis2= getspiegbasis (spiegelebenen.y);
  spiegbasis3= getspiegbasis (spiegelebenen.z);

  // Bewegungskugel
  real kzoom= 100;
  cschachfeld* textur1= new cschachfeld (cvektor3 (0,1,0), cvektor3 (1,0,0), 10/M_PIl, 10/M_PIl);
  ckoerper* beskugel= new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, nullvektor3, 0.1*kzoom*einsbasis3);

  tms zeit;
  real ticksps= real (sysconf (_SC_CLK_TCK));
  clock_t ticks= times (&zeit);
  //cout << "ticks/sek: " << ticksps << endl;

  integer koerper= 0;
  real framezeit;
  keyboard->putkey (19, 5, 1);
    do
    {
    if (1)
      {
      welt->aktualisiere ();
      welttoscreentakt ();
      screen->flush ();
      framezeit= (times (&zeit) - ticks)/ticksps;
      cout << "Zeit: " << framezeit << "  fps: " << 1/framezeit << endl;
      //printf ("Zeit: %5.2Lf  fps: %5.2Lf\n", framezeit, 1/framezeit);
      zeitpos= times (&zeit);
      flugw= eulerwinkelfrommatrix (welt->augbasis);
      flugw= 180/M_PI*flugw;
      drehaw= winkelachsefrommatrix (welt->augbasis);

      //nwinkel= 180/M_PI*drehaw.r;
      // Zeile löschen \r
      //printf ("Eulerwinkel: %15.9Lf %15.9Lf %15.9Lf          drehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", flugw.x, flugw.y, flugw.z, drehaw.i, drehaw.j, drehaw.ij, nwinkel);
      //printf ("                          augdrehaw: %12.9Lf %12.9Lf %12.9Lf          Drehwinkel:%12.4Lf\n", welt->augdrehaw.i, welt->augdrehaw.j, welt->augdrehaw.ij, welt->augdrehaw.r/M_PI*180);
      //drehmatrixausgabe (welt->augbasis);

      fflush (stdout);
      ticks= times (&zeit);
      }
      else
        usleep (0);

    keyboard->flush ();
    if (koerper == 0)
      {
      // Bewegungen
      if (keyboard->getkey (2, 2)) welt->verschiebeauge (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->verschiebeauge (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep/3, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep/3, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep/3, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep/3, 0));

      // Bewegungszoomfaktor ändern
      if (keyboard->getkey (1, 2)) bewstep*= 2.5;
      if (keyboard->getkey (1, 3)) bewstep/= 2.5;

      // Drehzoomquotient ändern
      if (keyboard->getkey (11, 2)) drehstep/= 2;
      if (keyboard->getkey (11, 3)) drehstep*= 2;

      // Drehungen
      if (keyboard->getkey (8, 2))
        {
        welt->dreheaugex (M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (8, 3))
        {
        welt->dreheaugex (-M_PI/drehstep/1.5);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 2))
        {
        welt->dreheaugey (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (9, 3))
        {
        welt->dreheaugey (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 2))
        {
        welt->dreheaugez (M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      if (keyboard->getkey (10, 3))
        {
        welt->dreheaugez (-M_PI/drehstep);
        welt->augdrehaw=winkelachsefrommatrix (welt->augbasis);
        }
      
      // an den Spiegelebenen spiegeln
      if (keyboard->getkey (1, 1))
        {
        if (zeitpos > bewkugel->stopzeit)
          {
          bewkugelpos= bewkugel->stoppos;
          if (besuchsliste.elposition (bewkugelpos) < 0)
            {
            // Besuchskugel erzeugen
            beskugel= new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, bewkugel->startpos, 0.08*kzoom*einsbasis3);
            beskugel->startpos= bewkugelpos;
            beskugel->stoppos= spiegbasis1*bewkugelpos;
            beskugel->startzeit= zeitpos;
            beskugel->stopzeit= beskugel->startzeit + 500;
            welt->addkoerper (beskugel);
            }
          besuchsliste.elementhinzu (bewkugelpos, 0);
          bewkugel->startpos= bewkugelpos;
          bewkugelpos= spiegbasis1*bewkugelpos;
          bewkugel->stoppos= bewkugelpos;
          bewkugel->startzeit= zeitpos;
          bewkugel->stopzeit= bewkugel->startzeit + 500;
          }
        }
      if (keyboard->getkey (2, 1))
        {
        if (zeitpos > bewkugel->stopzeit)
          {
          bewkugelpos= bewkugel->stoppos;
          if (besuchsliste.elposition (bewkugelpos) < 0)
            {
            // Besuchskugel erzeugen
            beskugel= new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, bewkugel->startpos, 0.08*kzoom*einsbasis3);
            beskugel->startpos= bewkugelpos;
            beskugel->stoppos= spiegbasis2*bewkugelpos;
            beskugel->startzeit= zeitpos;
            beskugel->stopzeit= beskugel->startzeit + 500;
            welt->addkoerper (beskugel);
            }
          besuchsliste.elementhinzu (bewkugelpos, 0);
          bewkugel->startpos= bewkugelpos;
          bewkugelpos= spiegbasis2*bewkugelpos;
          bewkugel->stoppos= bewkugelpos;
          bewkugel->startzeit= zeitpos;
          bewkugel->stopzeit= bewkugel->startzeit + 500;
          }
        }
      if (keyboard->getkey (3, 1))
        {
        if (zeitpos > bewkugel->stopzeit)
          {
          bewkugelpos= bewkugel->stoppos;
          if (besuchsliste.elposition (bewkugelpos) < 0)
            {
            // Besuchskugel erzeugen
            beskugel= new ckoerper (new cskugel, new cparawkugel, new cbegrkeine, textur1, bewkugel->startpos, 0.08*kzoom*einsbasis3);
            beskugel->startpos= bewkugelpos;
            beskugel->stoppos= spiegbasis3*bewkugelpos;
            beskugel->startzeit= zeitpos;
            beskugel->stopzeit= beskugel->startzeit + 500;
            welt->addkoerper (beskugel);
            }
          besuchsliste.elementhinzu (bewkugelpos, 0);
          bewkugel->startpos= bewkugelpos;
          bewkugelpos= spiegbasis3*bewkugelpos;
          bewkugel->stoppos= bewkugelpos;
          bewkugel->startzeit= zeitpos;
          bewkugel->stopzeit= bewkugel->startzeit + 500;
          }
        }
     
      if (keyboard->getkey (7, 1)) welt->dreheaugenorm (M_PI/drehstep);
      if (keyboard->getkey (8, 1)) welt->dreheaugeein (M_PI/drehstep);
      if (keyboard->getkey (9, 1)) welt->dreheaugeachse (-M_PI/drehstep);
      if (keyboard->getkey (10, 1)) welt->dreheaugeachse (M_PI/drehstep);
      }
      if (keyboard->getkey (1, 1))
        {
        koerper= 0; usleep (0);
        }
    }
  while (!keyboard->getkey (0, 0));
  }

void cflugsimu::fliege2 ()              // mit Körperdrehungen
  {
  cvektor3 flugw (0, 0, 0);
  cvektor4 drehaw;
  real nwinkel;
  //tms zeit;
  //real ticksps= real (sysconf (_SC_CLK_TCK));
  //clock_t ticks= times (&zeit);
  //cout << "ticks/sek: " << ticksps << endl;
  signed long koerper= 0;
  keyboard->putkey (19, 5, 1);
    do
    {
    if (keyboard->getkey (19, 5))  // wenn eine Taste gedrückt wird (egal welche)
      {
      welttoscreenz ();
      screen->flush ();
      //cout << "Zeit: " << (times (&zeit) - ticks)/ticksps << endl;
      flugw= eulerwinkelfrommatrix (welt->augbasis);
      nwinkel= 180/M_PIl*acosl (cosl (flugw.x)*cosl(flugw.y));
      flugw= 180/M_PI*flugw;
      drehaw= winkelachsefrommatrix (welt->augbasis);
      drehaw.r*= 180/M_PI;
      printf ("\r%15.9Lf %12.9Lf %12.9Lf W:%15.9Lf %12.9Lf %12.9Lf %12.9Lf w:%15.9Lf",
              flugw.x, flugw.y, flugw.z, drehaw.r, drehaw.i, drehaw.j, drehaw.ij, nwinkel);
      fflush (stdout);
      //ticks= times (&zeit);
      }
      else usleep (50000);
    keyboard->flush ();
    if (koerper == 0)
      {
      if (keyboard->getkey (2, 2)) welt->verschiebeauge (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->verschiebeauge (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep, 0));
      if (keyboard->getkey (8, 2)) welt->dreheaugex (M_PI/drehstep);
      if (keyboard->getkey (8, 3)) welt->dreheaugex (-M_PI/drehstep);
      if (keyboard->getkey (9, 2)) welt->dreheaugey (-M_PI/drehstep);
      if (keyboard->getkey (9, 3)) welt->dreheaugey (M_PI/drehstep);
      if (keyboard->getkey (10, 2)) welt->dreheaugez (M_PI/drehstep);
      if (keyboard->getkey (10, 3)) welt->dreheaugez (-M_PI/drehstep);
      if (keyboard->getkey (8, 1)) welt->dreheaugeein (M_PI/drehstep);
      if (keyboard->getkey (7, 1)) welt->dreheaugenorm (M_PI/drehstep);
      }
    if (koerper == 1)
      {
      printf ("k0 drestep %Lf\n", drehstep);
      if (keyboard->getkey (2, 2)) welt->koerperliste.koerper[0]->verschiebe (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->koerperliste.koerper[0]->verschiebe (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep, 0));
      if (keyboard->getkey (8, 2)) welt->koerperliste.koerper[0]->drehe (getrotx (M_PI/drehstep));
      if (keyboard->getkey (8, 3)) welt->koerperliste.koerper[0]->drehe (getrotx (-M_PI/drehstep));
      if (keyboard->getkey (9, 2)) welt->koerperliste.koerper[0]->drehe (getroty (M_PI/drehstep));
      if (keyboard->getkey (9, 3)) welt->koerperliste.koerper[0]->drehe (getroty (-M_PI/drehstep));
      if (keyboard->getkey (10, 2)) welt->koerperliste.koerper[0]->drehe (getrotz (M_PI/drehstep));
      if (keyboard->getkey (10, 3)) welt->koerperliste.koerper[0]->drehe (getrotz (-M_PI/drehstep));
      if (keyboard->getkey (8, 1)) welt->koerperliste.koerper[0]->dreheein ();
      }
    if (koerper == 2)
      {
      printf ("k1 drestep %Lf\n", drehstep);
      if (keyboard->getkey (2, 2)) welt->koerperliste.koerper[1]->verschiebe (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->koerperliste.koerper[1]->verschiebe (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep, 0));
      if (keyboard->getkey (8, 2)) welt->koerperliste.koerper[1]->drehe (getrotx (M_PI/drehstep));
      if (keyboard->getkey (8, 3)) welt->koerperliste.koerper[1]->drehe (getrotx (-M_PI/drehstep));
      if (keyboard->getkey (9, 2)) welt->koerperliste.koerper[1]->drehe (getroty (M_PI/drehstep));
      if (keyboard->getkey (9, 3)) welt->koerperliste.koerper[1]->drehe (getroty (-M_PI/drehstep));
      if (keyboard->getkey (10, 2)) welt->koerperliste.koerper[1]->drehe (getrotz (M_PI/drehstep));
      if (keyboard->getkey (10, 3)) welt->koerperliste.koerper[1]->drehe (getrotz (-M_PI/drehstep));
      if (keyboard->getkey (8, 1)) welt->koerperliste.koerper[1]->dreheein ();
      }
    if (koerper == 3)
      {
      printf ("k2 drestep %Lf\n", drehstep);
      if (keyboard->getkey (2, 2)) welt->koerperliste.koerper[2]->verschiebe (cvektor3 (0, 0, bewstep));
      if (keyboard->getkey (2, 3)) welt->koerperliste.koerper[2]->verschiebe (cvektor3 (0, 0, -bewstep));
      if (keyboard->getkey (3, 2)) welt->verschiebeauge (cvektor3 (bewstep, 0, 0));
      if (keyboard->getkey (3, 3)) welt->verschiebeauge (cvektor3 (-bewstep, 0, 0));
      if (keyboard->getkey (4, 2)) welt->verschiebeauge (cvektor3 (0, bewstep, 0));
      if (keyboard->getkey (4, 3)) welt->verschiebeauge (cvektor3 (0, -bewstep, 0));
      if (keyboard->getkey (8, 2)) welt->koerperliste.koerper[2]->drehe (getrotx (M_PI/drehstep));
      if (keyboard->getkey (8, 3)) welt->koerperliste.koerper[2]->drehe (getrotx (-M_PI/drehstep));
      if (keyboard->getkey (9, 2)) welt->koerperliste.koerper[2]->drehe (getroty (M_PI/drehstep));
      if (keyboard->getkey (9, 3)) welt->koerperliste.koerper[2]->drehe (getroty (-M_PI/drehstep));
      if (keyboard->getkey (10, 2)) welt->koerperliste.koerper[2]->drehe (getrotz (M_PI/drehstep));
      if (keyboard->getkey (10, 3)) welt->koerperliste.koerper[2]->drehe (getrotz (-M_PI/drehstep));
      if (keyboard->getkey (8, 1)) welt->koerperliste.koerper[2]->dreheein ();
      }
    if (keyboard->getkey (1, 2))
      {
      bewstep*= 2;
      usleep (250000);
      }
    if (keyboard->getkey (1, 3)) {bewstep/= 2; usleep (250000);}
    if (keyboard->getkey (10, 1))
      {
      if (drehstep > 2) drehstep/= 2;
      usleep (250000);
      }
    if (keyboard->getkey (9, 1))
      {
      if (drehstep < 256) drehstep*= 2;
      usleep (250000);
      }
    if (keyboard->getkey (1, 1)) {koerper= 0; usleep (250000);}
    if (keyboard->getkey (2, 1)) {koerper= 1; usleep (250000);}
    if (keyboard->getkey (3, 1)) {koerper= 2; usleep (250000);}
    if (keyboard->getkey (4, 1)) {koerper= 3; usleep (250000);}
    }
  while (!keyboard->getkey (0, 0));
  }
