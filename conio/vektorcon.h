#ifndef vektorcon_h
#define vektorcon_h

#include "../cc/class/vektor.h"

void printtext (const char* ptext);

void printinteger (integer pn);
void printreal (real pr);
void printvektor3ord (cvektor3 pv, integer pord);
void printvektor3wi (cvektor3 pv, real wi);
void printbasis3mat (cbasis3 pb, integer pord, integer pzykel);
void printbasis3dreh (cbasis3 pb, integer pord, integer pzykel, integer ppaar);
void printvektor4quat (cvektor4 pv, integer pord, integer pzykel);
void printvektor4dreh (cvektor4 pv, integer pord, integer pzykel, integer ppaar);

void realeingabe (real& pv);
void vektor3eingabe (cvektor3& pv);
void quaternioneneingabe (cvektor4& pv);
cvektor4 winkelachseeingabe ();

#endif
