/* Functions to load/write the light bundles in the night missions. */

#include <tank.h>
#include <light.h>

typedef struct
{
  int name;
  long sx, sy;
  long offx, offy;
  long *sb, *nb;

} bundlestruct;

static bundlestruct b[]={{LLIGHT1, 75,150,37,149},
			 {LLIGHT2, 100,150,0,149},
			 {LLIGHT3, 150,150,0,149},
			 {LLIGHT4, 150,100,0,99},
			 {LLIGHT5, 150,75,0,37},
			 {MLIGHT1, 29,76,14,75},
			 {MLIGHT2, 39,71,0,70},
			 {MLIGHT3, 58,58,0,57},
			 {MLIGHT4, 71,39,0,38},
			 {MLIGHT5, 76,29,0,14},
			 {SLIGHT1, 13,61,6,60},
			 {SLIGHT2, 28,56,0,55},
			 {SLIGHT3, 46,46,0,45},
			 {SLIGHT4, 56,28,0,27},
			 {SLIGHT5, 61,13,0,6},
			 {-1}};

PALETTE night_pal, night_pal_orig;
unsigned char night_link[256];

void init_night_mission(void)
{
  long i, n, *sb, *nb;
  char *d, *p;
  unsigned x, y;
  DATAFILE *data;
  FILE *f;
	
  f=open_file("DATA/NIGHT.LNK");
  fread(night_link, sizeof(night_link), 1, f);
  fclose(f);

  read_palette("DATA/NIGHT.PAL", &night_pal_orig);

  data = load_datafile("DATA/LIGHT.DAT");

  if (data == 0)
    error("Unable to open: DATA/LIGHT.DAT\n");
					 
  for (i=0; b[i].name != -1; i++)
  {
    d = data[b[i].name].dat;

    sb = allocate(b[i].sy * sizeof(long));
    nb = allocate(b[i].sy * sizeof(long));

    for (y=0; y<b[i].sy; y++)
    {
      for (x = 0, n = 0, p = &d[y * b[i].sx]; *p == -1 && x < b[i].sx; x++, n++, p++);
      sb[y] = n;

      for (n = 0; *p != -1 && x < b[i].sx; x++, n++, p++);
      nb[y] = n;
    }

    b[i].sb = sb, b[i].nb = nb;
  }

  unload_datafile(data);
}

static inline void or_part(long x, long y, long nx)
{ unsigned char *s;

  if (x+nx>=0 && x<buf_x && y>=0 && y<buf_y)
  { s=scr+y*buf_x+x;

    if (x<0) nx+=x, s-=x, x=0;
    if (x+nx>buf_x) nx=buf_x-x;

    for (; nx>0; nx--, s++)
     *s|=0x80;
  }
}

static void write_pattern(long x, long y, long sx, long sy, long *sb, long *nb)
{ if (x<-sx || x>buf_x+sx || y<-sy || y>buf_y+sy || sx==NULL) return;

  while(sy>0)
  { or_part(x+*sb, y, *nb);

    sy--;
    y++;
    sb++;
    nb++;
  }
}

static void write_pattern_xrev(long x, long y, long sx, long sy, long *sb, long *nb)
{ if (x<-sx || x>buf_x+sx || y<-sy || y>buf_y || sx==NULL) return;

  while(sy>0)
  { or_part(x-*sb-*nb, y, *nb);

    sy--;
    y++;
    sb++;
    nb++;
  }
}

static void write_pattern_yrev(long x, long y, long sx, long sy, long *sb, long *nb)
{ if (x<-sx || x>buf_x+sx || y<0 || y>buf_y+sy || sx==NULL) return;

  while(sy>0)
  { or_part(x+*sb, y, *nb);

    sy--;
    y--;
    sb++;
    nb++;
  }
}

static void write_pattern_xyrev(long x, long y, long sx, long sy, long *sb, long *nb)
{ if (x<-sx || x>buf_x+sx || y<0 || y>buf_y+sy || sx==NULL) return;

  while(sy>0)
  { or_part(x-*sb-*nb, y, *nb);

    sy--;
    y--;
    sb++;
    nb++;
  }
}

#define sx(j)	b[i*5+j].sx
#define sy(j)	b[i*5+j].sy
#define sb(j)	b[i*5+j].sb
#define nb(j)	b[i*5+j].nb
#define offx(j) b[i*5+j].offx
#define offy(j) b[i*5+j].offy

void write_bundle(long i, long x, long y, long d)
{ x+=16-ssx, y+=16-ssy;

  switch(d)
  { case 0:
     write_pattern(x-offx(0), y-offy(0), sx(0), sy(0), sb(0), nb(0));
     break;

    case 1:
     write_pattern(x-offx(1), y-offy(1), sx(1), sy(1), sb(1), nb(1));
     break;

    case 2:
     write_pattern(x-offx(2), y-offy(2), sx(2), sy(2), sb(2), nb(2));
     break;

    case 3:
     write_pattern(x-offx(3), y-offy(3), sx(3), sy(3), sb(3), nb(3));
     break;

    case 4:
     write_pattern(x-offx(4), y-offy(4), sx(4), sy(4), sb(4), nb(4));
     break;

    case 5:
     write_pattern_yrev(x-offx(3), y+offy(3), sx(3), sy(3), sb(3), nb(3));
     break;

    case 6:
     write_pattern_yrev(x-offx(2), y+offy(2), sx(2), sy(2), sb(2), nb(2));
     break;

    case 7:
     write_pattern_yrev(x-offx(1), y+offy(1), sx(1), sy(1), sb(1), nb(1));
     break;

    case 8:
     write_pattern_yrev(x-offx(0), y+offy(0), sx(0), sy(0), sb(0), nb(0));
     break;

    case 9:
     write_pattern_xyrev(x+offx(1), y+offy(1), sx(1), sy(1), sb(1), nb(1));
     break;

    case 10:
     write_pattern_xyrev(x+offx(2), y+offy(2), sx(2), sy(2), sb(2), nb(2));
     break;

    case 11:
     write_pattern_xyrev(x+offx(3), y+offy(3), sx(3), sy(3), sb(3), nb(3));
     break;

    case 12:
     write_pattern_xrev(x+offx(4), y-offy(4), sx(4), sy(4), sb(4), nb(4));
     break;

    case 13:
     write_pattern_xrev(x+offx(3), y-offy(3), sx(3), sy(3), sb(3), nb(3));
     break;

    case 14:
     write_pattern_xrev(x+offx(2), y-offy(2), sx(2), sy(2), sb(2), nb(2));
     break;

    case 15:
     write_pattern_xrev(x+offx(1), y-offy(1), sx(1), sy(1), sb(1), nb(1));
     break;
  }
}
