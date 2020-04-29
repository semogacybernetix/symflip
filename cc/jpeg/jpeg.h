#ifndef jpeg_h
#define jpeg_h

#include <cstdio>

#define FAST_IDCT

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197169399375105
#endif

const unsigned short JM_SOF0  = 0xFFC0;  // Huffman, Baseline DCT
const unsigned short JM_SOF1  = 0xFFC1;  // Huffman, Extended sequencial DCT
const unsigned short JM_SOF2  = 0xFFC2;  // Huffman, Progressive DCT
const unsigned short JM_SOF3  = 0xFFC3;  // Huffman, Spatial lossles
const unsigned short JM_SOF5  = 0xFFC5;  // Huffman, Differential sequential DCT
const unsigned short JM_SOF6  = 0xFFC6;  // Huffman, Differential progressive DCT
const unsigned short JM_SOF7  = 0xFFC7;  // Huffman, Differential spatial
const unsigned short JM_SOF8  = 0xFFC8;  // Arithmetic, Reserved
const unsigned short JM_SOF9  = 0xFFC9;  // Arithmetic, Extended sequential DCT
const unsigned short JM_SOF10 = 0xFFCA;  // Arithmetic, Progressive DCT
const unsigned short JM_SOF11 = 0xFFCB;  // Arithmetic, Spatial lossless
const unsigned short JM_SOF13 = 0xFFCD;  // Arithmetic, Differential sequential DCT
const unsigned short JM_SOF14 = 0xFFCE;  // Arithmetic, Differential progressive DCT
const unsigned short JM_SOF15 = 0xFFCF;  // Arithmetic, Differential spatial
const unsigned short JM_DHT   = 0xFFC4;  // define huffman tables
const unsigned short JM_DAC   = 0xFFCC;  // define arithmetic conditioning
const unsigned short JM_SOI   = 0xFFD8;  // Start of Image
const unsigned short JM_EOI   = 0xFFD9;  // end of image
const unsigned short JM_SOS   = 0xFFDA;  // start of scan
const unsigned short JM_DQT   = 0xFFDB;  // define quantization tables
const unsigned short JM_DNL   = 0xFFDC;  // define number of lines
const unsigned short JM_DRI   = 0xFFDD;  // define restart interval
const unsigned short JM_APP0  = 0xFFE0;  // ...
const unsigned short JM_APP1  = 0xFFE1;  // ...
const unsigned short JM_APP2  = 0xFFE2;  // ...
const unsigned short JM_APP3  = 0xFFE3;  // ...
const unsigned short JM_APP4  = 0xFFE4;  // ...
const unsigned short JM_APP5  = 0xFFE5;  // ...
const unsigned short JM_APP6  = 0xFFE6;  // ...
const unsigned short JM_APP7  = 0xFFE7;  // ...
const unsigned short JM_APP8  = 0xFFE8;  // ...
const unsigned short JM_APP9  = 0xFFE9;  // ...
const unsigned short JM_APP10 = 0xFFEA;  // ...
const unsigned short JM_APP11 = 0xFFEB;  // ...
const unsigned short JM_APP12 = 0xFFEC;  // ...
const unsigned short JM_APP13 = 0xFFED;  // IPTC data block
const unsigned short JM_APP14 = 0xFFEE;  // ...
const unsigned short JM_APP15 = 0xFFEF;  // ...
const unsigned short JM_COM   = 0xFFFE;  // comment

const int MAX_FRAME_COMPONENTS = 3;
const int MAX_SCAN_COMPONENTS  = 3;

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#ifdef _DEBUG
void DEB(int level, const char *str,...)
{
  char buf[200];
  char *p = buf;

  for (int i=0; i<level; i++)
    *p++ = ' ';

  va_list m;
  va_start (m,str);
  vsprintf(p, str, m);
  va_end(m);
  printf("%s", buf);
}
#else
inline void DEB(int level, const char *str,...)
  {
  level= level - level;
  str= 0;
  str+= 0;
  }
#endif

// rounds x to multiples of y (y must be a power of 2)
inline ushort Round (ushort x, ushort y)
{
    y--;
    return ushort((x + y) & ~y);
}

const uchar unzigzag[]=
{
     0, 1, 8,16, 9, 2, 3,10,
    17,24,32,25,18,11, 4, 5,
    12,19,26,33,40,48,41,34,
    27,20,13, 6, 7,14,21,28,
    35,42,49,56,57,50,43,36,
    29,22,15,23,30,37,44,51,
    58,59,52,45,38,31,39,46,
    53,60,61,54,47,55,62,63,
    0,0,0,0,0,0,0,0,0,0,0,0  // this saves us an if in inner loop of decode()
};

struct App0
{
    ushort laenge;
    char   id[5];
    union
    {
        struct
        {
            uchar byte;
        }
        JFXX;
        struct
        {
            ushort version;
            uchar  units;
            ushort xdichte;
            ushort ydichte;
            uchar  thumbx;
            uchar  thumby;
            uchar  RGB[1];  // thumbx*thumby*3 bytes
        }
        JFIF;
    };
};

struct hufftable
{
    uchar  bits[17];      // number of elements using [i] length of bits
    uchar  huffval[256];  // values associatd with huffman codes

    uchar  huffsize[257]; // sizes (number of bits) of codes
    ushort huffcode[256]; // huffman codes

    ushort mincode[17]; // smallest code value for length [i]
    long   maxcode[17]; // largest code value for length [i]
    ushort valptr[17];  // index to start of values in huffval for length [i]

    uchar  lastk;         // number of codes
};

struct quanttable
{
    ushort quantval[64];
};

struct data_unit
{
    short val[64];
};

struct Pixel
{
    union { uchar b,v; };
    union { uchar g,u; };
    union { uchar r,y; };
};

class Decoder
{
public:
    Decoder(char* pname);
    ~Decoder();

    int decode_image();
    Pixel *get_picture() { return picture; }
    int get_X_size() { return frame.X; }
    int get_Y_size() { return frame.Y; }
    int get_line_length() { return Round(frame.X, 8*frame.Hmax); }

private:
    int level; // indent level for debug printfs...
    static const int indent=2;

    uchar *filebuf;
    uchar *readptr;
    int bufsize;
    bool at_eof;
    bool eof() { return at_eof; }
    void read_bytes(void *buf, long size);

    int decode_frame();
    int decode_scan();
    int decode_restart_interval();
    inline int decode_MCU();
    inline int decode_data_unit();
    inline int decode_ac_coefficients();

    void iquantize_unit();
    void idct_unit();
    void level_shift_unit();
    void bound_unit();

    data_unit *current_unit;

    void color_space_convert();

    int interpret_frame_header();
    int interpret_scan_header();
    int interpret_marker(int typ);

    inline ushort nextshort();
    inline int    nextbyte();
    inline uchar  nextbit();
    inline ushort receive(uchar ssss);  // receive ssss bits
    int    nextmarker();         // skip to next marker, return type or EOF
    int    skip_bytes(int laenge);

    int handle_APP0();
    int handle_DHT();
    int handle_DAC();
    int handle_DRI();
    int handle_DQT();
    int handle_COM();

    void generate_size_table();
    void generate_code_table();
    void generate_decode_tables();

    inline uchar decode();
    inline short extend(ushort v, uchar t);

    int find_frame_comp(uchar c); // return index of component with Ci==c

    Pixel *picture;
    ushort picturex,picturey;
    ushort curx,cury;
    ushort incx,incy;
    ushort blocksx,blocksy;

    FILE *in;    // current JPEG input stream
    uchar b;     // bitbuffer
    uchar cnt;   // bits in bitbuffer

    ushort Ri;   // MCUs in Restart Interval
    bool is_last_restart;
    int num_mcu;
    int total_mcu;

    hufftable dc_tab[4]; // dc entropy encoding tables
    hufftable ac_tab[4]; // ac entropy encoding tables

    hufftable *huff;     // current hufftable

    quanttable qtab[4];  // quantization tables

#ifndef FAST_IDCT
    long costab[8][8];
    void create_costab();
#endif

    uchar range_tab[768];
    void create_range_tab();

    // The names of the members of the following structs
    // are directly taken from the ISO 10918-1 specs

    struct
    {
        uchar  P;   // Precision (8 for baseline)
        ushort Y;   // number of lines   ( Y resolution )
        ushort X;   // samples per line  ( X resolution )
        uchar  Nf;  // Number of image components in frame (1 is grayscale)
        struct
        {
            uchar Ci;       // Component identifier
            uchar Hi  :4;   // Horizontal sampling factor
            uchar Vi  :4;   // Vertical sampling factor
            uchar Tqi;      // Quantization table destination

            data_unit *blocks;
        }
        comp[MAX_FRAME_COMPONENTS];

        uchar Hmax :4;
        uchar Vmax :4;
    }
    frame;
    int frame_comp;  // current frame component

    struct
    {
        uchar Ns;   // Number of scan components (>1 is interleaved mode)
        struct
        {
            uchar Csj;    // Scan component selector
            uchar Tdj :4; // dc coding table destination
            uchar Taj :4; // ac coding table destination

            short pred;     // previous dc value
        }
        comp[MAX_SCAN_COMPONENTS];
        uchar Ss;     // Start of spectral/predictor selection
        uchar Se;     // End of spectral selection
        uchar Ah  :4; // successive approximation bit position high
        uchar Al  :4; // successive approximation bit position low
    }
    scan;
    int scan_comp;    // current scan component;
};

#endif
