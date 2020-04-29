#include "leedct.h"
#include "jpeg.h"
#include <cstring>

Decoder::Decoder (char* pname)
  : cnt(0), Ri(0)
{
    in= fopen (pname, "rb");
    level = 0;

    picture = 0;
    for (int i=0; i<MAX_FRAME_COMPONENTS; i++)
        frame.comp[i].blocks = 0;

#ifndef FAST_IDCT
    create_costab();
#endif
    create_range_tab();

    filebuf = new uchar[1024];
    readptr = filebuf;
    bufsize = 0;
    at_eof = false;
}
Decoder::~Decoder()
{
    for (int i=0; i<MAX_FRAME_COMPONENTS; i++)
        if (frame.comp[i].blocks)
            delete[] frame.comp[i].blocks;

    delete[] filebuf;
    fclose (in);
}

void Decoder::read_bytes(void *buf, long size)
  {
  for (int i=0; i<size;i++)
  *((uchar*)buf + i) = nextbyte();
  }

  int Decoder::decode_image()
{
    DEB(level, "Decode_Image running\n");
    level+=indent;
    int typ = nextmarker();
    if (typ != JM_SOI) return 1;
    DEB (level, "Start of Image\n");

    typ = nextmarker();
    while (!eof())
    {
        switch (typ)
        {
        default:
            interpret_marker(typ);
            typ = nextmarker();
            break;

        case JM_SOF0:
            {
              int err = decode_frame();
              level-=indent;
              DEB(level, "Decode_Image exit\n");
              return err;
            }

        case JM_SOF2:
            printf ("Progressive Modus (noch?) nicht unterstuetzt");
            level-=indent;
            return 1;

        case JM_SOF1:
        case JM_SOF3:
        case JM_SOF5:
        case JM_SOF6:
        case JM_SOF7:
            printf("Kein Baseline DCT Format");
            level-=indent;
            return 1;

        case JM_SOF8:
        case JM_SOF9:
        case JM_SOF10:
        case JM_SOF11:
        case JM_SOF13:
        case JM_SOF14:
        case JM_SOF15:
            printf("Arithmetische Kodierung nicht unterstuetzt");
            level-=indent;
            return 1;
        }
    }
    printf("Vorzeitiges Dateiende!");
    level-=indent;
    DEB(level, "Decode_Image exit\n");
    return 1;
}

int Decoder::decode_frame()
{
    DEB (level, "Decode_frame running\n");
    level+=indent;
    int err = interpret_frame_header ();
    if (err) return err;

    DEB(level, "Progress Range set to %d-%d\n", 0, frame.Y/incy);

    int typ = nextmarker();
    do
    {
        while (!eof() && typ != JM_SOS) // Start Of Scan
        {
            interpret_marker(typ);     // optional markers
            typ = nextmarker();
        }
        if (eof()) break;

        DEB (level, "Start of Scan\n");
        err = decode_scan();
        if (err) return err;

        typ = nextmarker();
    }
    while (!eof() && typ != JM_EOI);

    if (eof())
    {
        DEB(level, "Illegal file format\n");
        level-=indent;
        return 1;
    }

    if (frame.Nf>1) color_space_convert();

    level-=indent;
    DEB (level, "Decode_frame exit\n");
    return 0;
}

int Decoder::decode_scan()
{
    DEB(level, "Decode_Scan running\n");
    level+=indent;

    int err = interpret_scan_header();
    if (err) return err;

    curx = 0;
    cury = 0;

    num_mcu = Ri ? Ri : total_mcu;

    int num_restarts = total_mcu / num_mcu;
    int last_num_mcu = total_mcu % num_mcu;

    is_last_restart = false;

    for (int rest = 1; rest <= num_restarts; rest++)
    {
        if (rest == num_restarts)
        {
            is_last_restart = true;
            if (last_num_mcu != 0) // last restart may not have Ri MCUs...
                num_mcu = last_num_mcu;
        }
        err = decode_restart_interval();
        if (err) { level-=indent; return err; }
    }

    level-=indent;
    DEB(level, "Decode_Scan exit\n");
    return 0;
}

int Decoder::decode_restart_interval()
{
    DEB (level, "Decode restart interval running\n");
    level+=indent;

    for (int i=0; i<MAX_SCAN_COMPONENTS; i++)
        scan.comp[i].pred = 0;

    cnt = 0; // reset bitbuffer !!

    int mcu=num_mcu;
    do
    {
        int err = decode_MCU();
        if (err) return err;

        curx += incx;
        if (curx >= frame.X)
        {
            curx=0;
            cury += incy;
//            SendDlgItemMessage (hProgressDlg, IDC_PROGRESS, PBM_STEPIT, 0, 0);
        }

    }
    while (--mcu);

    if (!is_last_restart)
        nextmarker();   // skip RSTn marker

    level-=indent;
    DEB (level, "Decode restart interval exit\n");
    return 0;
}

int Decoder::find_frame_comp(uchar c)
{
    for (int i=0; i<frame.Nf; i++)
    {
        if (frame.comp[i].Ci == c) return i;
    }
    return -1;
}

inline int Decoder::decode_MCU()
{
//    DEB ("Decode MCU running\n");

    // Decode the components
    for (scan_comp=0; scan_comp<scan.Ns; scan_comp++)
    {
        frame_comp = scan.comp[scan_comp].Csj;
        uchar Hi = frame.comp[frame_comp].Hi;
        uchar Vi = frame.comp[frame_comp].Vi;
        int   xf = frame.Hmax/Hi;   // x scaling factor
        int   yf = frame.Vmax/Vi;   // y scaling factor

        current_unit = frame.comp[frame_comp].blocks;

        for(int v=0; v<Vi; v++)     // iterate component's v blocks in mcu
        {
            int yoff1 = cury + 8*v;

            for(int h=0; h<Hi; h++) // iterate component's h blocks in mcu
            {
                int xoff1 = curx + 8*h;

                decode_data_unit();

                iquantize_unit();

#ifdef FAST_IDCT
                data_unit output;
                LeeIDct(current_unit->val, output.val);
                memcpy (current_unit->val, output.val, sizeof(data_unit));
#else
                idct_unit();
#endif
                level_shift_unit();
                bound_unit();

                // now write the just decoded matrix into picture
                short *valptr = current_unit->val;
                current_unit++;
                for (int y=0; y<8; y++)    // y field in current matrix
                {
                  int yoff2 = yoff1 + y*yf;

                  for (int x=0; x<8; x++)  // x field in current matrix
                  {
                    int xoff2 = xoff1 + x*xf;

                    uchar val = (uchar)*valptr++;

                    // stretch the pixel if component's res < picture res
                    for(int ys=0;ys<yf;ys++)
                    {
                      int yoff3 = (yoff2 + ys) * picturex;
                      for (int xs=0; xs<xf;xs++)
                      {
                        int xoff3 = xoff2 + xs;
                        Pixel &p = picture[xoff3 + yoff3];

                        if (frame.Nf > 1)
                        {
                            switch(frame_comp)
                            {
                            case 0: p.y = val; break;
                            case 1: p.u = val; break;
                            case 2: p.v = val; break;
                            }
                        }
                        else
                            p.r = p.g = p.b = val;
                      }   // x scale
                    }     // y scale
                  }   // matrix x
                }     // matrix y

            }   // h blocks
        }       // v blocks
    }   // component iteration

    return 0;
}

inline int Decoder::decode_data_unit()
{
//    DEB ("Decode data unit running\n");

    huff = &dc_tab[scan.comp[scan_comp].Tdj];
    uchar t    = decode();
    short v    = receive(t);
    short diff = extend(v,t);

    current_unit->val[0] = scan.comp[scan_comp].pred + diff;
    scan.comp[scan_comp].pred = current_unit->val[0];

    huff = &ac_tab[scan.comp[scan_comp].Taj];
    decode_ac_coefficients();

    return 0;
}

inline int Decoder::decode_ac_coefficients()
{
    for (int i=1; i<64; i++)
        current_unit->val[i] = 0;

    for (int k=1; k<64; k++)
    {
        uchar rs = decode();
        uchar s = uchar(rs & 0xF);   // size
        uchar r = uchar(rs >> 4);    // runlength

        if (s)
        {
            k += r;

            short val = receive(s);
            val = extend(val,s);

            //assert(k<64);
            current_unit->val[unzigzag[k]] = val;
        }
        else
        {
            if (r == 0) return 0;  // EOB found: finished
            k += 15; // ZRL found
        }
    }
    return 0;
}

void Decoder::iquantize_unit()
{
    quanttable *qt = &qtab[frame.comp[frame_comp].Tqi];

    for (int i=0; i<64; i++)
        current_unit->val[i] *= qt->quantval[i];
}

#ifndef FAST_IDCT
void Decoder::create_costab()
{

    DEB(level, "Cosinustabelle:\n");
    for (int x=0; x<8; x++)
    {
        for (int u=0; u<8; u++)
        {
            costab[x][u] = long (cos( double(2*x+1) * double(u) * M_PI / 16.0)*512);

            DEB (0, "%4ld ", costab[x][u]);
        }
        DEB(0, "\n");
    }
}

inline long C(int x)
{
    return x==0 ? 362 : 512;
}

void Decoder::idct_unit()
{
    data_unit f;

    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8;x++)
        {
            long sum = 0;
            for (int u=0; u<8; u++)
            {
                for (int v=0; v<8; v++)
                {
                    long t = (C(u) * C(v))+256 >>9;
                    t = (t * costab[x][u])+256 >>9;
                    t = (t * costab[y][v])+256 >>9;
                    t = t * current_unit->val[v*8+u];
                    sum += t+256 >>9;
                }
            }

            f.val[y*8+x] = short(sum/4);
        }
    }
    memcpy (current_unit->val, f.val, sizeof(data_unit));
}
#endif // !FAST_IDCT

void Decoder::level_shift_unit()
{
    for (int i=0; i<64; i++)
        current_unit->val[i] += 128;
}

void Decoder::create_range_tab()
{
    int i;

    for (i=0;   i<256; i++) range_tab[i] = 0;
    for (i=256; i<512; i++) range_tab[i] = i-256;
    for (i=512; i<768; i++) range_tab[i] = 255;
}

void Decoder::bound_unit()
{
    for (int i=0; i<64; i++)
        current_unit->val[i] = range_tab[current_unit->val[i] + 256];
}

void Decoder::color_space_convert()
{
    for (int y=0; y < frame.Y; y++)
    {
        for (int x=0; x< frame.X; x++)
        {
            Pixel &p = picture[x + picturex*y];

            long y = p.y;
            long u = p.u - 128;
            long v = p.v - 128;

            p.r = range_tab[y                + v*1402/1000 + 256];
            p.g = range_tab[y - u* 344/1000  - v* 714/1000 + 256];
            p.b = range_tab[y + u*1772/1000                + 256];
        }
    }
}

int Decoder::interpret_frame_header()
{
    DEB (level, "interpret frame header running\n");
    level+=indent;

    ushort laenge = nextshort();

    frame.P  = nextbyte();
    frame.Y  = nextshort();
    frame.X  = nextshort();
    frame.Nf = nextbyte();

    DEB (level, "Precision:        %d\n", frame.P);
    DEB (level, "Number of lines:  %d\n", frame.Y);
    DEB (level, "Samples per line: %d\n", frame.X);
    DEB (level, "Frame components: %d\n", frame.Nf);

    if (frame.Nf > MAX_FRAME_COMPONENTS) return 1;

    frame.Hmax = 0;
    frame.Vmax = 0;

    level+=indent;
    for (int i=0; i< frame.Nf; i++)
    {
        DEB (level, "Component %d:\n", i+1);
        frame.comp[i].Ci = nextbyte();
        uchar tmp = nextbyte();
        frame.comp[i].Hi = tmp >> 4;
        frame.comp[i].Vi = tmp & 0xf;
        frame.comp[i].Tqi = nextbyte();

        if (frame.comp[i].Hi > frame.Hmax) frame.Hmax = frame.comp[i].Hi;
        if (frame.comp[i].Vi > frame.Vmax) frame.Vmax = frame.comp[i].Vi;

        level+=indent;
        DEB (level, "Component id: %d\n", frame.comp[i].Ci);
        DEB (level, "Horiz factor: %d\n", frame.comp[i].Hi);
        DEB (level, "Vert  factor: %d\n", frame.comp[i].Vi);
        DEB (level, "Qtable dest:  %d\n", frame.comp[i].Tqi);
        level-=indent;
    }
    level-=indent;

    DEB (level, "Hmax = %d\n", frame.Hmax);
    DEB (level, "Vmax = %d\n", frame.Vmax);

    incx = 8*frame.Hmax;
    incy = 8*frame.Vmax;

    int mcux = (frame.X-1) / incx +1;
    int mcuy = (frame.Y-1) / incy +1;
    total_mcu = mcux * mcuy;

    picturex = Round(frame.X, incx);
    picturey = Round(frame.Y, incy);

    picture = new Pixel[picturex * picturey];
    memset (picture, 0, picturex*picturey*sizeof(Pixel));

    for (int i=0; i< frame.Nf; i++)
        frame.comp[i].blocks = new data_unit[frame.comp[i].Hi * frame.comp[i].Vi];

    if (laenge != 8 + 3*frame.Nf) {level-=indent;return 1;}

    level-=indent;
    DEB (level, "interpret frame header exit\n");
    return 0;
}

int Decoder::interpret_scan_header()
{
    DEB (level,"interpret scan header running\n");
    level+=indent;

    ushort laenge = nextshort();

    scan.Ns = nextbyte();

    if (scan.Ns > MAX_SCAN_COMPONENTS) return 1;

    DEB (level, "Scan components: %d\n", scan.Ns);
    level+=indent;
    for (int i=0; i<scan.Ns; i++)
    {
        DEB (level, "Component %d:\n", i+1);
        level+=indent;
        scan.comp[i].Csj = nextbyte();
        uchar tmp = nextbyte();
        scan.comp[i].Tdj = tmp >> 4;
        scan.comp[i].Taj = tmp & 0xF;

        DEB(level, "component selector: %d\n", scan.comp[i].Csj);
        DEB(level, "DC hufftable dest:  %d\n", scan.comp[i].Tdj);
        DEB(level, "AC hufftable dest:  %d\n", scan.comp[i].Taj);

        int index = find_frame_comp(scan.comp[i].Csj); // ID to index
        if (index == -1)
        {
            printf("Falscher Framekomponenten Verweis in Scan Komponente");
            return -1;
        }
        scan.comp[i].Csj = index;
        level-=indent;
    }
    level-=indent;

    scan.Ss  = nextbyte();
    scan.Se  = nextbyte();
    uchar tmp = nextbyte();
    scan.Ah  = tmp >> 4;
    scan.Al  = tmp & 0xF;

    DEB(level, "Start of spectral:  %d\n", scan.Ss);
    DEB(level, "End of spectral:    %d\n", scan.Se);
    DEB(level, "Approximation high: %d\n", scan.Ah);
    DEB(level, "Approximation low:  %d\n", scan.Al);
    if (laenge != 6 + 2*scan.Ns) {level-=indent;return 1;}

    level-=indent;
    DEB (level,"interpret scan header exit\n");
    return 0;
}

inline ushort Decoder::nextshort ()
{
    int a,b;
    a = nextbyte();
    if (a==EOF) return 0;
    b = nextbyte();
    if (b==EOF) return 0;
    return (a<<8) | b;
}

inline int Decoder::nextbyte ()
{
    if (bufsize == 0)
    {
        bufsize = fread (filebuf, 1, 1024, in);
        if (bufsize == 0) { at_eof = true; return EOF; }
        readptr = filebuf;
    }

    bufsize--;
    return *readptr++;
}

inline uchar Decoder::nextbit()
{
    if (cnt==0)
    {
        b = nextbyte();
        cnt = 8;

        if (b == 0xFF)
        {
            int b2 = nextbyte();
            //if (eof(in)) return 0;
            if (b2)
            {
                int typ = (b<<8) | b2;
                if (typ == JM_DNL)
                    DEB(level, "TODO: Process DNL Marker and terminate scan\n");
                else
                {
                    DEB(level, "nextbit: Unexpected marker\n");
                    return (uchar)-1;
                }
            }
        }
    }

    cnt--;
    return (b>>cnt) & 1;
}

int Decoder::nextmarker()
{
    int typ,c1,c2;
    do
    {
        c1 = nextbyte();
        if (c1==0xFF)
        {
            c2 = nextbyte();
            typ = (c1<<8) | c2;
            if (typ != 0xFF00) return typ;
        }
    }
    while (c1 != EOF);

    return c1 == EOF;
}

inline ushort Decoder::receive(uchar ssss)
{
    ushort v = 0;

    for (int i=0; i<ssss; i++)
        v = (v<<1) | nextbit();

    return v;
}

int Decoder::interpret_marker (int typ)
{
    switch(typ)
    {
    case JM_APP0:
         return handle_APP0();
    case JM_APP2:
    case JM_APP3:
    case JM_APP4:
    case JM_APP5:
    case JM_APP6:
    case JM_APP7:
    case JM_APP8:
    case JM_APP9:
    case JM_APP10:
    case JM_APP11:
    case JM_APP12:
         DEB (level,"Unknown APPx marker. (%04x)\n",typ);
         skip_bytes(nextshort());
         return 0;
    case JM_APP13:
         DEB (level,"APP13 marker (IPTC data block)\n",typ);
         skip_bytes(nextshort());
         return 0;

    case JM_DHT:
         return handle_DHT();
    case JM_DAC:
         return handle_DAC();
    case JM_DRI:
         return handle_DRI();
    case JM_DQT:
         return handle_DQT();
    case JM_COM:
         return handle_COM();
    default:
         DEB (level,"Unknown marker %04x\n",typ);
         skip_bytes(nextshort());
         return 0;
    }
}

int Decoder::handle_APP0()
{
    DEB (level, "APP0 marker:\n");
    level+=indent;

    int laenge = nextshort() - 2;
    App0 header;

    header.laenge = laenge+2;
    DEB (level, "length: %d\n", header.laenge);
    read_bytes (header.id, sizeof(header.id));
    laenge -= sizeof(header.id);

    DEB (level, "ID: %s\n", header.id);
    if (!strncmp(header.id, "JFIF", 4))
    {
        header.JFIF.version = nextshort();
        header.JFIF.units   = nextbyte();
        header.JFIF.xdichte = nextshort();
        header.JFIF.ydichte = nextshort();
        header.JFIF.thumbx  = nextbyte();
        header.JFIF.thumby  = nextbyte();
        laenge -= 2+1+2+2+1+1;

        DEB (level, "Version: %04x\n", header.JFIF.version);
        DEB (level, "Units:   %d\n", header.JFIF.units);
        DEB (level, "Xdens:   %d\n", header.JFIF.xdichte);
        DEB (level, "Ydens:   %d\n", header.JFIF.ydichte);
        DEB (level, "ThumbX:  %d\n", header.JFIF.thumbx);
        DEB (level, "ThumbY:  %d\n", header.JFIF.thumby);
    }
    else if (!strncmp(header.id, "JFXX", 4))
    {
        DEB (level, "Thumbnail Picture: ");
        uchar typ = nextbyte();
        laenge--;
        switch (typ)
        {
        case 0x10: DEB (level, "JPEG compressed\n"); break;
        case 0x11: DEB (level, "1 byte/pixel\n");    break;
        case 0x13: DEB (level, "3 bytes/pixel\n");   break;
        default:   DEB (level, "unknown/illegal thumbnail type\n"); break;
        }
    }

    level-=indent;
    return skip_bytes (laenge);
}

int Decoder::handle_DHT()
{
    DEB (level, "Define Huffman tables\n");
    level+=indent;

    int laenge = nextshort() - 2;

    while (laenge > 0)
    {
        int i;
        int n = nextbyte();

        uchar tc = n>>4;      // table class: 0=dc 1=ac
        uchar th = n & 0x3;   // table dest: 0..3

        DEB(level, "%s Table -> %d:\n", tc?"AC":"DC", th);

        if (tc) huff = &ac_tab[th];
        else    huff = &dc_tab[th];

        int mt = 0;
        huff->bits[0] = 0; // not used
        for (i=1; i<= 16; i++)
        {
            huff->bits[i] = nextbyte();
            mt += huff->bits[i];
        }

        for (i=0; i<mt; i++)
            huff->huffval[i] = nextbyte();

        laenge -= 17 + mt;

        generate_size_table ();
        generate_code_table ();
        generate_decode_tables ();
    }

    if (laenge < 0)
    {
        DEB(level, "Illegal DHT marker length\n");
        level-=indent;
        return 1;
    }

    level-=indent;
    DEB (level, "Define Huffman tables exit\n");
    return 0;
}

int Decoder::handle_DAC()
{
    DEB (level, "Define arithmetic conditioning (not supported)\n");
    DEB (level, "  Skipping this marker...\n");
    int laenge = nextshort()-2;
    return skip_bytes(laenge);
}

int Decoder::handle_DRI()
{
    DEB(level, "Define restart interval\n");
    int laenge = nextshort();
    if (laenge != 4) return 1;

    Ri = nextshort();

    DEB(level, "  Restart Interval = %d\n", Ri);
    DEB(level, "Define restart interval exit\n");
    return 0;
}

int Decoder::handle_DQT ()
{
    DEB (level, "Define quantization tables:\n");
    level+=indent;

    int laenge = nextshort() - 2;

    while (laenge > 0)
    {
        int i;
        int n = nextbyte();

        int p = n>>4;  // precision 0=8bit 1=16bit
        n &= 0x3;      // index     0..3

        DEB (level, "Table %d Precision %d:\n", n,8+p*8);

        for (i=0; i<64; i++)
            qtab[n].quantval[unzigzag[i]] = p ? nextshort() : nextbyte();

        level+=indent;
        DEB(level,"");
        for (i=0;i<64;i++)
        {
            DEB (0, "%3hd ",qtab[n].quantval[i]);
            if ((i+1)%8 == 0) { DEB(0,"\n"); if (i!=63) DEB(level,""); }
        }
        level-=indent;
        laenge -= 1 + (64<<p);
    }
    level-=indent;
    DEB (level, "Define quantization tables exit\n");
    return 0;
}

int Decoder::handle_COM ()
{
    int laenge = nextshort() - 2;

    char c[200];
    read_bytes (c, laenge);
    c[laenge] = '\0';
    DEB (level, "Comment:\n  %s\n", c);

    return 0;
}

void Decoder::generate_size_table()
{
    int k=0;

    for (int i=1; i<= 16; i++)
        for (int j=1; j <= huff->bits[i]; j++)
            huff->huffsize[k++] = i;

    huff->huffsize[k] = 0;
    huff->lastk = k;
}

void Decoder::generate_code_table()
{
    int k = 0;
    ushort code = 0;
    uchar si = huff->huffsize[0];

    for(;;)
    {
        do
            huff->huffcode[k++] = code++;
        while (huff->huffsize[k] == si);

        if (huff->huffsize[k] == 0) break;

        do
            code <<= 1;
        while (huff->huffsize[k] != ++si);
    }
}

void Decoder::generate_decode_tables()
{
    int j = 0;

    for(int i=1;i<=16;i++)
    {
        if (huff->bits[i])
        {
            huff->valptr[i] = j;
            huff->mincode[i] = huff->huffcode[j];
            j += huff->bits[i];
            huff->maxcode[i] = huff->huffcode[j-1];
        }
        else huff->maxcode[i] = -1;
    }
}

inline uchar Decoder::decode()
{
    int i;
    ushort code = nextbit();

    for (i=1; code > huff->maxcode[i]; i++)
        code = code<<1 | nextbit();

    int hc = huff->valptr[i] + code - huff->mincode[i];
    hc &= 0xFF;
    return huff->huffval[ hc ];
}

inline short Decoder::extend(ushort v, uchar t)
{
    if (v < (1 << (t-1)))
       return (short)v + (65535 << t) + 1;

    return (short)v;
}

int Decoder::skip_bytes (int laenge)
{
    for (int i=0; i<laenge; ++i)
    {
        int byte = nextbyte();
        if (byte == EOF) return 1;
    }
    return 0;
}
