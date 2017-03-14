/* All kinds of functions to load pictures etc. at game startup. */

#include <tank.h>
#include <string.h>
#include <conio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>

extern turretinfo ti[];
extern pictureinfotype pc0[], pc1[], pc2[], pc3[];
extern long missing, unused;

unsigned char *back[max_back*4], *pstart[8][4], gray[256];
unsigned char *rock[4][5], *scr, *ubuf, *buf[2], *icons[2][15];
unsigned char *man[10], *b52pics[4][3][4], *train[num_wagons+1][5];
unsigned char *zeppelin[8*4*2];
pictureinfotype *pictures[max_back]={pc0, pc1, pc2, pc3}, *pic;
long cmap[256], cmap_dark[256], gray_normal[256];
long buf_x, buf_y, ssx, ssy;
long res_x = 640, res_y = 480, graph_drv = GFX_AUTODETECT, num_pages = 1;
PALETTE pal_orig, pal, black;
long num_pic[max_back];
BITMAP *scrbuf, *panel;
FONT *big_font;

static indextype *pic_index;
static unsigned char *pic_data=NULL;
static long pic_size, picture_file_size;

void error(char *msg, ...)
{
  va_list arg;

  va_start(arg, msg);

  /* Restore to text mode */

  allegro_exit();

  /* write message to screen */

  vprintf(msg, arg);

  /* exit to dos */

  exit(1);
}

void *allocate(long size)
{
  void *m;

  m = malloc(size);

  if (m == 0)
   error("Allocation error: unable to allocate %ld bytes.\n", size);

  return(m);
}

FILE *open_file(char *fn)
{
  FILE *f;

  f = fopen(fn, "rb");

  if (f  == 0)
    error("Missing file: %s\n", fn);

  return(f);
}

long get_file_size(FILE *f)
{ long size, old;

  old=ftell(f);
  fseek(f, 0, SEEK_END);
  size=ftell(f);
  fseek(f, old, SEEK_SET);

  return(size);
}

unsigned char *read_file(char *fn)
{ long size;
  unsigned char *buf;
  FILE *f;

  f=open_file(fn);
  size=get_file_size(f);
  buf=allocate(size);
  fread(buf, size, 1, f);
  fclose(f);

  return(buf);
}

void init_screen_buffer(void)
{ long i, j;	     

  scrbuf = create_bitmap(res_x, res_y);

  scr = scrbuf->dat;

  for (i=0; i<2; i++)
   buf[i]=allocate(res_x*res_y);

  mapm.g=allocate(max_map_x*max_map_y);

  mapm.b=allocate(max_map_x*max_map_y);

  for (i=0; i<2; i++)
  { mapm.man[i]=allocate(max_map_x*max_map_y);

    for (j=0; j<3; j++)
     strength[i][j]=allocate(2*max_map_x*max_map_y);
  }

  for (i=0; i<3; i++)
   mapm.m[i]=allocate(2*max_map_x*max_map_y);
}

unsigned char *file_ptr(char *fn)
{ long start=0, mid, end=pic_size;
  long cmp;

  if (fn!=NULL)
  { while (start<end)
    { mid=(end+start)/2;

      cmp=strncmp(fn, pic_index[mid].name, 12);

      if (cmp==0)
      { if (!unused)
	 return(&pic_data[pic_index[mid].addr]);

	 else
	 { pic_index[mid].addr=0;
	   return(NULL);
	 }
      }
      else if (cmp<0) end=mid;
      else start=mid+1;
    }

    if (missing && strncmp(fn, "TAN", 3)!=0 && strncmp(fn, "FACE", 4)!=0)
    { m3_init();
      printf("%s ", fn);
    }
  }

  return(NULL);
}

void read_picture_file(void)
{ FILE *f;

  f=open_file("DATA/PICTURES.DAT");
  picture_file_size=get_file_size(f);

  if (pic_data==NULL)
   pic_data=allocate(picture_file_size);

  fread(pic_data, picture_file_size, 1, f);
  fclose(f);
}

void init_various_graph(void)
{
  DATAFILE *d;
  PALETTE p;
  long i;

  read_palette("DATA/PALETTE.DAT", &pal_orig);
  
  for (i=0; i<256; i++)
    gray_normal[i]=(pal_orig[i].r*30+pal_orig[i].g*59+pal_orig[i].b*11)/600;

  panel = load_bmp("DATA/BITMAPS.DAT#PANEL.BMP", p);

  if (panel == 0)
    error("Unable to load: DATA/BITMAPS.DAT\n");

  d = load_datafile_object("DATA/SETSOUND.DAT", "SETUP_FONT");

  if (d == 0)
    error("Unable to load: DATA/SETSOUND.DAT\n");

  big_font = d->dat;
}

void init_pictures(void)
{ char str[20];
  long i, j, k;
  FILE *f;

  read_picture_file();

  f=open_file("DATA/PICTURES.IDX");
  pic_size=get_file_size(f);
  pic_index=allocate(pic_size);
  fread(pic_index, pic_size, 1, f);
  fclose(f);

  pic_size/=sizeof(indextype);

  for (i=0; i<max_back*4; i++)
  { sprintf(str, "BACK%ld", i+1);
    back[i]=file_ptr(str);
  }

  for (j=0; j<max_back; j++)
  { for (k=0; pictures[j][k].name[0][0]!=0; k++)
     for (i=0; i<3; i++)
       pictures[j][k].ptr[i]=file_ptr(pictures[j][k].name[i]);

    num_pic[j]=k;
  }

  for (i=0; i<num_tanks; i++)
  { for (j=0; j<54; j++)
    { sprintf(str, "TAN%02ld-%ld", i+1, j+1);
      tank[i].pic[j]=file_ptr(str);
    }

    for (j=0; j<5; j++)
    { sprintf(str, "FACE%ld-%ld", i+1, j+1);
      tank[i].face[j]=file_ptr(str);
    }
  }

  for (i=0; i<num_wagons+1; i++)
   for (j=0; j<5; j++)
   { sprintf(str, "WAGO%ld-%ld", i, j+1);
     train[i][j]=file_ptr(str);
   }

  for (k=0; k<4; k++)
   for (j=0; j<4; j++)
   { sprintf(str, "B52%c%c", 'A'+(char)k, 'A'+(char)j);
     b52pics[k][0][j]=file_ptr(str);

     sprintf(str, "_52%c%c", 'A'+(char)k, 'A'+(char)j);
     b52pics[k][1][j]=file_ptr(str);

     sprintf(str, "B52B%ld-%ld", k+1, j+1);
     b52pics[k][2][j]=file_ptr(str);
   }

  for (i=0; i<num_turrets; i++)
   for (j=0; j<5; j++)
   { sprintf(str, "TUR%ld-%ld", i+1, j+1);
     ti[i].pic[j]=file_ptr(str);
   }

  for (i=0; i<8; i++)
   for (j=0; j<4; j++)
   { sprintf(str, "START%ld-%ld", i+1, j+1);
     pstart[i][j]=file_ptr(str);
   }

  for (k=0; k<2; k++)
   for (i=0; i<2; i++)
    for (j=0; j<5; j++)
    { sprintf(str, "%cOCK%ld-%ld", k==0? 'R':'^', i+1, j+1);
      rock[i+k*2][j]=file_ptr(str);
    }

  for (i=0; eff[i].name[0]!=0; i++)
   eff[i].ptr=file_ptr(eff[i].name);

  for (i=0; intern[i].name[0]!=0; i++)
   intern[i].ptr=file_ptr(intern[i].name);

  for (i=0; i<2; i++)
   for (j = 0; j < 15; j++)
   { sprintf(str, "%cCON%ld", i==0? 'I':'X', j+1);
     icons[i][j]=file_ptr(str);
   }

  for (i=0; i<10; i++)
  { sprintf(str, "MAN-%ld", i+1);
    man[i]=file_ptr(str);
  }

  for (i=0; i<4*8; i++)
  { sprintf(str, "ZEP%c%c", 'A'+(char)(i/26), 'A'+(char)(i%26));
    zeppelin[i]=file_ptr(str);

    sprintf(str, "ZEP2%c%c", 'A'+(char)(i/26), 'A'+(char)(i%26));
    zeppelin[8*4+i]=file_ptr(str);
  }

  if (unused)
  { m3_init();

    for (i=0; i<pic_size; i++)
     if (pic_index[i].addr!=0)
      printf("%s ", pic_index[i].name);

    printf("\n");
  }

  if (unused || missing)
   exit(0);

  free(pic_index);
}

static inline void or_picture(unsigned char *pic, unsigned char *mask)
{ long i;

  if (pic!=NULL && mask!=NULL)
  { for (i=0; i<b_size*b_size; i++)
     if (mask[i]!=0xff) pic[i]|=0x80;
  }
}

void load_pictures(void)
{ static enum {DAY, NIGHT} loaded=DAY;
  long i, j, rq;

  rq=map.night && game.mode!=MAP? NIGHT:DAY;

  if (rq==NIGHT)
  { for (i=0; i<128; i++)
    { gray[i]=night_link[gray_normal[i]];
      gray[i+0x80]=night_link[gray_normal[i]]+0x80;
      cmap[i]=night_link[i]+0x80;
      cmap_dark[i]=night_link[i];
    }
  } else
  { for (i=0; i<256; i++)
    { gray[i]=gray_normal[i];
      cmap[i]=i;
      cmap_dark[i]=i;
    }
  }

  if (rq==loaded)
   return;

  message("RELOADING IMAGE DATA...");
  set_palette(pal);

  if (rq!=NIGHT)
   read_picture_file();

  if (rq==NIGHT)
  { b_convert(pic_data, night_link, picture_file_size);

    for (i=0; i<num_pic[(int)map.background]; i++)
     or_picture(pic[i].ptr[0], pic[i].ptr[2]);

    for (i=0; eff[i].name[0]!=0; i++)
     if (eff[i].night_vis)
      or_picture(eff[i].ptr, eff[i].ptr);

    for (i=0; intern[i].name[0]!=0; i++)
     if (intern[i].night_vis)
      or_picture(intern[i].ptr, intern[i].ptr);

    for (j=0; j<15; j++)
     or_picture(icons[1][j], icons[1][j]);

    for (i=0; i<num_tanks; i++)
     for (j=0; j<5; j++)
      or_picture(tank[i].face[j], tank[i].face[j]);

    for (i=0; i<2; i++)
     for (j=0; j<5; j++)
      or_picture(rock[i][j], rock[i+2][j]);

    for (i=0; i<2*4*8; i++)
     or_picture(zeppelin[i], zeppelin[i]);
  }

  loaded=rq;
}


