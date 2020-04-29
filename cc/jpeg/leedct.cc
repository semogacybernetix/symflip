#include "leedct.h"

/*

This is the Byeong Gi Lee algorithm from IEEE Trans. Accoustics,
Speech, and Signal Processing, Vol ASSP-32, No. 6, December 1984, pp.
1243 -1245.

*/

#define LS(r,s) ((r) << (s))
#define RS(r,s) ((r) >> (s)) /* Caution with rounding... */

#define MSCALE(expr)  RS((expr),9)

#define IDCTSCALE(x) (((x<0) ? (x-8) : (x+8))/16);

/* Cos Table */

#define itwoc1d4 362L
#define itwoc1d8 277L
#define itwoc3d8 669L
#define itwoc1d16 261L
#define itwoc3d16 308L
#define itwoc5d16 461L
#define itwoc7d16 1312L
#define isqrt2 362L

#define x0 tx0
#define x1 tx1
#define x2 tx2
#define x3 tx3
#define x4 ex4
#define x5 ex5
#define x6 ex6
#define x7 ex7

#define r0 rx0
#define r1 rx1
#define r2 rx2
#define r3 rx3

#define s0 rx0
#define s1 rx1
#define s2 rx2
#define s3 rx3

#define f0 ex0
#define f1 ex1
#define f2 ex2
#define f3 ex3

#define g0 ex4
#define g1 ex5
#define g2 ex6
#define g3 ex7

#define b1 gx0
#define b2 gx0
#define b3 gx1

#define a1 gx2
#define a3 gx2
#define c1 gx2
#define c3 gx2

#define ihold gx1

void LeeIDct(short *x,short *y)
{
  register int ex0,ex1,ex2,ex3,ex4,ex5,ex6,ex7;
  register int tx0,tx1,tx2,tx3;
  register int rx0,rx1,rx2,rx3;
  register int gx0,gx1,gx2;
  register short *iptr,*jptr;
  register int i;

  /* Do rows */

  for(jptr=y,iptr=x,i=0;i<8;i++)
  {
      x0 = MSCALE(isqrt2*LS(*(iptr++),2));
      x1 = LS(*(iptr++),2);
      x2 = LS(*(iptr++),2);
      x3 = LS(*(iptr++),2);
      x4 = LS(*(iptr++),2);
      x5 = LS(*(iptr++),2);
      x6 = LS(*(iptr++),2);
      x7 = LS(*(iptr++),2);

      a1 = MSCALE(itwoc1d4*x4);
      r0 = x0+a1;
      r1 = x0-a1;

      a3 = MSCALE(itwoc1d4*(x2+x6));
      r2 = MSCALE(itwoc1d8*(x2+a3));
      r3 = MSCALE(itwoc3d8*(x2-a3));

      f0 = r0+r2;
      f1 = r1+r3;
      f2 = r0-r2;
      f3 = r1-r3;

      b1 = x3+x5;
      c1 = MSCALE(itwoc1d4*b1);
      s0 = x1+c1;
      s1 = x1-c1;

      b2 = x1+x3;
      b3 = x5+x7;
      c3 = MSCALE(itwoc1d4*(b2+b3));
      s2 = MSCALE(itwoc1d8*(b2+c3));
      s3 = MSCALE(itwoc3d8*(b2-c3));

      g0 = MSCALE(itwoc1d16*(s0+s2));
      g1 = MSCALE(itwoc3d16*(s1+s3));
      g2 = MSCALE(itwoc7d16*(s0-s2));
      g3 = MSCALE(itwoc5d16*(s1-s3));

      *(jptr++) = f0+g0;
      *(jptr++) = f1+g1;
      *(jptr++) = f3+g3;
      *(jptr++) = f2+g2;

      *(jptr++) = f2-g2;
      *(jptr++) = f3-g3;
      *(jptr++) = f1-g1;
      *(jptr++) = f0-g0;
  }

#undef x1
#undef x2
#undef x3
#undef x4
#undef x5
#undef x6
#undef x7

#define x1 iptr[8]
#define x2 iptr[16]
#define x3 iptr[24]
#define x4 iptr[32]
#define x5 iptr[40]
#define x6 iptr[48]
#define x7 iptr[56]

  /* Do columns */

  for(i=0;i<8;i++)
  {
      jptr = iptr = y+i;

      x0 = MSCALE(isqrt2*(*iptr));

      a1 = MSCALE(itwoc1d4*x4);
      r0 = x0+a1;
      r1 = x0-a1;

      a3 = MSCALE(itwoc1d4*(x2+x6));
      r2 = MSCALE(itwoc1d8*(x2+a3));
      r3 = MSCALE(itwoc3d8*(x2-a3));

      f0 = r0+r2;
      f1 = r1+r3;
      f2 = r0-r2;
      f3 = r1-r3;

      b1 = x3+x5;
      c1 = MSCALE(itwoc1d4*b1);
      s0 = x1+c1;
      s1 = x1-c1;

      b2 = x1+x3;
      b3 = x5+x7;
      c3 = MSCALE(itwoc1d4*(b2+b3));
      s2 = MSCALE(itwoc1d8*(b2+c3));
      s3 = MSCALE(itwoc3d8*(b2-c3));

      g0 = MSCALE(itwoc1d16*(s0+s2));
      g1 = MSCALE(itwoc3d16*(s1+s3));
      g2 = MSCALE(itwoc7d16*(s0-s2));
      g3 = MSCALE(itwoc5d16*(s1-s3));

      ihold = f0+g0; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f1+g1; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f3+g3; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f2+g2; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f2-g2; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f3-g3; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f1-g1; (*jptr) = IDCTSCALE(ihold); jptr += 8;
      ihold = f0-g0; (*jptr) = IDCTSCALE(ihold);
  }
}
