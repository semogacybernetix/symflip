#include "xkbd.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

Display*              kdisplay;

cxkeyboard::cxkeyboard ()
  {
  kdisplay= XOpenDisplay ("");
  }

cxkeyboard::~cxkeyboard ()
  {
  XCloseDisplay (kdisplay);
  }

void cxkeyboard::flush ()
  {
  char keytab[32];
  XQueryKeymap (kdisplay, keytab);
  char kb;

  // Zahlenreihe
  kb= XKeysymToKeycode (kdisplay, XK_Escape);
  key[0][0]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_1);
  key[1][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_2);
  key[2][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_3);
  key[3][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_4);
  key[4][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_5);
  key[5][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_6);
  key[6][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_7);
  key[7][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_8);
  key[8][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_9);
  key[9][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_0);
  key[10][1]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  // Bewegungen
  kb= XKeysymToKeycode (kdisplay, XK_Q);
  key[1][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_A);
  key[1][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_W);
  key[2][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_S);
  key[2][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_E);
  key[3][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_D);
  key[3][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_R);
  key[4][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_F);
  key[4][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_T);
  key[5][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_G);
  key[5][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  kb= XKeysymToKeycode (kdisplay, XK_U);
  key[7][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  kb= XKeysymToKeycode (kdisplay, XK_J);
  key[7][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  kb= XKeysymToKeycode (kdisplay, XK_I);
  key[8][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  kb= XKeysymToKeycode (kdisplay, XK_K);
  key[8][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  
  kb= XKeysymToKeycode (kdisplay, XK_O);
  key[9][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_L);
  key[9][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_P);
  key[10][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_odiaeresis);
  key[10][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_semicolon);
  key[10][3]= char (key[10][3] | ((keytab[kb >> 3] >> (kb & 7)) & 1));
  kb= XKeysymToKeycode (kdisplay, XK_udiaeresis);
  key[11][2]= (keytab[kb >> 3] >> (kb & 7)) & 1;
  kb= XKeysymToKeycode (kdisplay, XK_adiaeresis);
  key[11][3]= (keytab[kb >> 3] >> (kb & 7)) & 1;

  // alle Tasten
  key[19][5]= key[1][2] | key[1][3] | key[2][2] | key[2][3] | key[3][2] | key[3][3] | key [4][2] | key [4][3]    // Bewegungen + Bewegungsschrittweite
            | key[8][2] | key[8][3] | key[9][2] | key[9][3] | key[10][2] | key[10][3] | key[11][2] | key[11][3]  // Drehungen + Drehschrittweite
		         	| key[7][1] | key[8][1] | key[9][1] | key[10][1] | key[9][1];                                        // Zahlenreihe 
  }
