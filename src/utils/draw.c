/* My own, very good drawing program (NOT). But it works. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <dpmi.h>
#include <sys/farptr.h>
#include <go32.h>
#include <stdarg.h>

#define p(x,y) 		picture[(x)+pic_x*(y)]
#define b(x,y)		buffer[(x)+pic_x*(y)]
#define in_range(x,y)   ((x)>=0 && (x)<pic_x && (y)>=0 && (y)<pic_y)
#define round(x)	((int)((x)>0? ((x)+0.5):((x)-0.5)))
#define sqr(x)		((x)*(x))

#define TRUE		(0==0)
#define FALSE		(0==1)

#define MAX_PATH	128

typedef struct
{
  unsigned char red, green, blue;

} palettetype[256];

extern unsigned char start_5x5_font[];

long m13_overwrite_text(long x, long y, long color, char *text, ...);
void m13_bar(long x, long y, long nx, long ny, long color);
void m13_xpixel(long x, long y, long color);
void m13_pixel(long x, long y, long color);
void m13_clear_screen(void);
void set_pal(palettetype *pal);
void get_pal(palettetype *pal);
long m13_init(void);
void m3_init(void);
long in_win(long x, long y);
long in_color(long x, long y);
long in_palette(long x, long y);
void error_msg(char *msg);
void write_all_pixels(void);
void write_active_color(void);
void write_all_colors(void);
void set_new_palette(void);
void write_marker(long x, long y);
void write_pixel(long x, long y, long c);
void fill(long x, long y, long fc, long c);
void write_position(void);
void mirror_y(void);
void rotate_90(void);
void rotate_angle(float sin, float cos);
void move(void);
void dline(long x1, long y1, long x2, long y2);
void circle(long x1, long y1, long x2, long y2);
void zoom_out(void);
void mirror_x(void);
void delay(long d);
void save_file(long x1, long y1, long x2, long y2);
unsigned wait_key(void);
long reset_mouse(void);
long get_mouse(long *x, long *y);

long pic_x, pic_y, start_x=0, start_y=0, scr_x=50, scr_y=50, m_x=0, m_y=0;
unsigned char *buffer, *buffer2, *picture;
char *tmp, name[80];
palettetype pal;
long ac=255;

void main(long argc, char **argv)
{ long x, y, i, last_x=-1, last_y=-1, last_col=0;
  long ms, kb, col, palette;
  long size;
  FILE *f, *palet;

  setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<4)
   error_msg("USE: DRAW <name> <size_x> <size_y> [add picture]\n\n"
	 "Mouse summary:\n\n"
	 "left   - pick color/put pixel/increase intensity\n"
	 "middle - select color below pointer\n"
	 "right  - undo last pixel/decrease intensity\n\n"
	 "Key summary:\n\n"
	 "f      - fill\n"
	 "d      - big dot\n"
	 "r      - replace color\n"
	 "-      - zoom out\n"
	 "2      - rotate 22.5 deg right\n"
	 "@      - rotate 22.5 deg left\n"
	 "4      - rotate 45 deg right\n"
	 "9      - reflect in x==y axis\n"
	 "x      - reflect in x-axis\n"
	 "y      - reflect in y-axis\n"
	 "space  - view whole picture\n"
	 "grey + - next color\n"
	 "grey - - prev. color\n"
	 "[      - set point P\n"
	 "]      - draw line from P\n"
	 "}      - draw circle with center in P\n"
	 "{      - save image with other corner P\n"
	 "alt-x  - exit without save\n"
	 "esc    - exit with save\n"
	 "alt-p  - exit save picture but not palette\n");
  
  strupr(argv[1]);

  sscanf(argv[2], "%ld", &pic_x);
  sscanf(argv[3], "%ld", &pic_y);

  if (pic_x<scr_x) scr_x=pic_x;
  if (pic_y<scr_y) scr_y=pic_y;

  if (!m13_init())
   error_msg("Unable to initialize graph mode.\n");

  if (!reset_mouse())
   error_msg("Unable to initialize mouse driver.\n");

  size=pic_x*pic_y, buffer=malloc(size), buffer2=malloc(size);
  picture=malloc(size);

  if (picture==NULL || buffer==NULL || buffer2==NULL)
   error_msg("Insuffient memory.\n");

  if ((f=fopen(argv[1], "rb"))!=NULL)
  { fread(picture, size, 1, f);
    fclose(f);
  } else for (x=0; x<size; x++) picture[x]=255;

  if (argc==5 && (f=fopen(argv[4], "rb"))!=NULL)
  { fread(buffer, size, 1, f);
    fclose(f);

    for (x=0; x<size; x++)
     if (buffer[x]!=255)
      picture[x]=buffer[x];

    //for (x=0; x<size; x++)
    // if (buffer[x]==255)
    //  picture[x]=0;
  }

  strcpy(name, argv[1]);
  tmp=strchr(name, '.');
  if (tmp!=NULL) *tmp=0;
  strcat(name, ".PAL");
  
  if ((palet=fopen(name, "rb+"))!=NULL || (palet=fopen("DATA\\PALETTE.DAT", "rb+"))!=NULL)
  { fread((char *)pal, sizeof(pal), 1, palet);
  } else
  { get_pal(&pal);
    pal[255].red=10;
    pal[255].green=10;
    pal[255].blue=10;
  }

  do
  { set_new_palette();
    write_all_colors();
    write_all_pixels();
    write_active_color();

    do
    { ms=get_mouse(&x, &y);
      kb=wait_key();
      write_marker(x, y);
      write_position();

      if (ms!=0)
      { write_marker(-1, -1);

	switch(ms)
	{ case 1:
	   if (in_win(x, y))
	   { col=p(x/4+start_x, y/4+start_y);

	     if (last_x==-1 || col!=ac)
	      last_col=col, last_x=x/4+start_x, last_y=y/4+start_y;

	     p(x/4+start_x, y/4+start_y)=ac;
	     write_pixel(x/4, y/4, ac);
	   }

	   else if ((col=in_color(x, y))!=-1)
	   { ac=col;
	     write_active_color();
	     set_new_palette();
	   }

	   else if ((palette=in_palette(x, y))!=-1)
	   { switch(palette)
	     { case 0:
		if (pal[ac].red!=63) pal[ac].red++;
		break;

	       case 1:
		if (pal[ac].green!=63) pal[ac].green++;
		break;

	       case 2:
		if (pal[ac].blue!=63) pal[ac].blue++;
		break;
	     }

	     set_new_palette();
	     delay(5);
	   }
	   break;

	  case 2:
	   if (in_win(x, y) && last_x!=-1)
	   { p(last_x, last_y)=last_col;
	     write_all_pixels();
	   }

	   else if ((palette=in_palette(x, y))!=-1)
	   { switch(palette)
	     { case 0:
		if (pal[ac].red!=0) pal[ac].red--;
		break;

	       case 1:
		if (pal[ac].green!=0) pal[ac].green--;
		break;

	       case 2:
		if (pal[ac].blue!=0) pal[ac].blue--;
		break;
	     }

	     set_new_palette();
	     delay(5);
	   }
	   break;

	  case 4:
	   if (in_win(x, y))
	   { ac=p(x/4+start_x, y/4+start_y);
	     write_active_color();
	     set_new_palette();
	   }
	   break;
	}
      }

      if (kb!=0)
      { write_marker(-1, -1);

	switch(kb)
	{ case 0x4e2b:
	   ac++;
	   write_active_color();
	   set_new_palette();
	   break;

	  case 0x4a2d:
	   ac--;
	   write_active_color();
	   set_new_palette();
	   break;

	  case 0x48e0:
	   if (start_y!=0)
	   { start_y--;
	     write_all_pixels();
	   }
	   break;

	  case 0x4be0:
	   if (start_x!=0)
	   { start_x--;
	     write_all_pixels();
	   }
	   break;

	  case 0x50e0:
	   if (start_y<pic_y-scr_y)
	   { start_y++;
	     write_all_pixels();
	   }
	   break;

	  case 0x4de0:
	   if (start_x<pic_x-scr_x)
	   { start_x++;
	     write_all_pixels();
	   }
	   break;

	  case 0x2064:
	   if (in_win(x, y))
	   { x/=4, y/=4;
	     i=(y+start_y)*pic_x+x+start_x;

	     picture[i]=ac;
	     if (x>0) picture[i-1]=ac;
	     if (x<pic_x-1) picture[i+1]=ac;
	     if (y>0) picture[i-pic_x]=ac;
	     if (y<pic_y-1) picture[i+pic_x]=ac;
	     if (x>0 && y>0) picture[i-pic_x-1]=ac;
	     if (x>0 && y<pic_y-1) picture[i+pic_x-1]=ac;
	     if (x<pic_x-1 && y>0) picture[i-pic_x+1]=ac;
	     if (x<pic_x-1 && y<pic_y-1) picture[i+pic_x+1]=ac;

	     write_all_pixels();
	   }
	   break;

	  case 0x1372:
	   do
	   { ms=get_mouse(&x, &y);
	     write_marker(x, y);
	     col=in_color(x, y);
	     m13_overwrite_text(232, 152, 15, col!=-1? "Repl: %.3d  ":"           ", col);
	     m13_overwrite_text(232, 160, 15, "          ");
	     m13_overwrite_text(232, 168, 15, "          ");
	   } while (ms==0);

	   write_marker(-1, -1);

	   if (col!=-1)
	    for (x=0; x<pic_x*pic_y; x++)
	     if (picture[x]==ac) picture[x]=col;

	   write_all_pixels();
	   break;

	  case 0x2166:
	   if (in_win(x, y) && p(x/4+start_x, y/4+start_y)!=ac)
	    fill(x/4+start_x, y/4+start_y, p(x/4+start_x, y/4+start_y), 0);

	   write_all_pixels();
	   break;

	  case 0x2d78:
	   mirror_x();
	   write_all_pixels();
	   break;

	  case 0x1579:
	   mirror_y();
	   write_all_pixels();
	   break;

	  case 0xa39:
	   m13_clear_screen();
	   rotate_90();
	   write_all_colors();
	   write_all_pixels();
	   write_active_color();
	   set_new_palette();
	   break;

	  case 0x534:
	   rotate_angle(0.7071067, 0.7071067);
	   write_all_pixels();
	   break;

	  case 0x0332:
	   rotate_angle(0.3826834, 0.9238795);
	   write_all_pixels();
	   break;

	  case 0x0340:
	   rotate_angle(-0.3826834, 0.9238795);
	   write_all_pixels();
	   break;

	  case 0x326d:
	   move();
	   write_all_pixels();
	   break;

	  case 0x1a5b:
	   if (in_win(x, y)) m_x=x/4+start_x, m_y=y/4+start_y;
	   break;

	  case 0x1a7b:
	   if (in_win(x, y))
	    save_file(m_x, m_y, x/4+start_x, y/4+start_y);
	   break;

	  case 0x1b5d:
	   if (!in_win(x, y)) break;
	   dline(m_x, m_y, x/4+start_x, y/4+start_y);
	   write_all_pixels();
	   break;

	  case 0x1b7d:
	   if (!in_win(x, y)) break;
	   circle(m_x, m_y, x/4+start_x, y/4+start_y);
	   write_all_pixels();
	   break;

	  case 0x0c2d:
	   zoom_out();
	   write_all_pixels();
	   break;
	}
      }

    } while (kb!=0x11b && kb!=0x2d00 && kb!=0x1900 && kb!=0x3920);

    if (kb==0x3920)
    { m13_clear_screen();

      for (x=0; x<pic_x; x++)
       for (y=0; y<pic_y; y++)
	m13_pixel(x, y, p(x, y));

      while(wait_key()==0 && (ms=get_mouse(&x, &y))==0)
       write_marker(x, y);

      while(get_mouse(&x, &y)!=0);

      write_marker(-1, -1);

      if (ms!=0)
      { x-=scr_x/2;
	y-=scr_y/2;

	if (x<0) start_x=0;
	 else if (x<pic_x-scr_x) start_x=x;
	  else start_x=pic_x-scr_x;

	if (y<0) start_y=0;
	 else if (y<pic_y-scr_y) start_y=y;
	  else start_y=pic_y-scr_y;
      }

      m13_clear_screen();
    }
  } while (kb!=0x11b && kb!=0x2d00 && kb!=0x1900);

  m3_init();

  if (kb!=0x2d00 && kb!=0x1900)
  { printf("Writing palette.\n");

    if (palet==NULL)
     palet=fopen(name, "wb");

    rewind(palet);
    fwrite(pal, sizeof(pal), 1, palet);
    fclose(palet);
  }

  if (kb!=0x2d00)
  { printf("Writing picture: %s\n", argv[1]);

    f=fopen(argv[1], "wb");
    fwrite(picture, size, 1, f);
    fclose(f);
  }
}

void error_msg(char *message)
{
  m3_init();
  printf(message);
  exit(1);
}

void delay(long d)
{ long i=clock()+d;
  while (clock()<i);
}

unsigned wait_key(void)
{ __dpmi_regs regs;

  regs.h.ah=0x11;
  __dpmi_int(0x16, &regs);

  if ((regs.x.flags&0x0040)!=0)
   return(0);

  regs.h.ah=0x10;
  __dpmi_int(0x16, &regs);

  return(regs.x.ax);
}

void write_marker(long x, long y)
{ static long a=-1, b=0, c;

  if (a==x && b==y) return;

  if (a!=-1)
  { for (c=a-5; c<a+6; c++) if (c>=0 && c<320) m13_xpixel(c, b, 15);
    for (c=b-5; c<b+6; c++) if (c>=0 && c<200) m13_xpixel(a, c, 15);
  }

  a=x; b=y;

  if (x!=-1 && y!=-1)
  { for (c=x-5; c<x+6; c++) if (c>=0 && c<320) m13_xpixel(c, y, 15);
    for (c=y-5; c<y+6; c++) if (c>=0 && c<200) m13_xpixel(x, c, 15);
  } else a=-1;
}

void write_pixel(long x, long y, long c)
{ m13_bar(x*4, y*4, 3, 3, c);
  m13_pixel(224+x, 98+y, c);
}

void write_all_colors(void)
{ long x, y;

  for (x=0; x<16; x++)
   for (y=0; y<16; y++)
    m13_bar(224+x*6, y*6, 5, 5, x+16*y);
}

void write_all_pixels(void)
{ long x, y;

  for (x=0; x<scr_x; x++)
   for (y=0; y<scr_y; y++)
    write_pixel(x, y, p(x+start_x, y+start_y));
}

void write_active_color(void)
{ long x, y;

  for (x=0; x<9; x++)
   for (y=0; y<7; y++)
    m13_bar(283+x*4, 98+y*4, 5, 5, ac);

  m13_overwrite_text(291, 128, 15, "%.3d", ac);
}

void set_new_palette(void)
{ set_pal(&pal);

  m13_overwrite_text(232, 176, 15, "Red:   %.2d", pal[ac].red);
  m13_overwrite_text(232, 184, 15, "Green: %.2d", pal[ac].green);
  m13_overwrite_text(232, 192, 15, "Blue:  %.2d", pal[ac].blue);
}

void write_position(void)
{ long x, y, col;

  get_mouse(&x, &y);
  col=in_color(x, y);

  if (in_win(x, y))
  { m13_overwrite_text(232, 152, 15, "%.3ld, %.3ld", x/4+start_x, y/4+start_y);
    m13_overwrite_text(232, 160, 15, "%+.3ld, %+.3ld", x/4+start_x-pic_x/2, y/4+start_y-pic_y/2);
    m13_overwrite_text(232, 168, 15, "Color: %.3d ", p(x/4+start_x, y/4+start_y));
  }

  else
  { m13_overwrite_text(232, 152, 15, "          ");
    m13_overwrite_text(232, 160, 15, "          ");
    m13_overwrite_text(232, 168, 15, col!=-1? "Color: %.3d ":"          ", col);
  }
}

long in_win(long x, long y)
{ return(x<scr_x*4 && y<scr_y*4? TRUE:FALSE);
}

long in_color(long x, long y)
{ return(x>=224 && y<96? (x-224)/6+(y/6)*16:-1);
}

long in_palette(long x, long y)
{ return(x>=232 && y>=176? (y-176)/8:-1);
}

void fill(long x, long y, long fc, long c)
{ if (p(x, y)!=fc && c<3000) return;

  p(x, y)=ac;

  if (x>0) fill(x-1, y, fc, c+1);

  if (x<pic_x-1) fill(x+1, y, fc, c+1);

  if (y>0) fill(x, y-1, fc, c+1);

  if (y<pic_y-1) fill(x, y+1, fc, c+1);
}

void exchange(unsigned char *p1, unsigned char *p2)
{ unsigned char t;

  t=*p1;
  *p1=*p2;
  *p2=t;
}

void exchange_ptr(unsigned char **p1, unsigned char **p2)
{ unsigned char *t;

  t=*p1;
  *p1=*p2;
  *p2=t;
}

void exchange_long(long *p1, long *p2)
{ long t;

  t=*p1;
  *p1=*p2;
  *p2=t;
}

void lsort(long *p1, long *p2)
{ if (*p1>*p2) exchange_long(p1, p2);
}

void mirror_x(void)
{ long x, y;

  for (y=0; y<pic_y/2; y++)
   for (x=0; x<pic_x; x++)
    exchange(&p(x, y), &p(x, pic_y-y-1));
}

void mirror_y(void)
{ long x, y;

  for (y=0; y<pic_y; y++)
   for (x=0; x<pic_x/2; x++)
    exchange(&p(x, y), &p(pic_x-x-1, y));
}

void rotate_90(void)
{ long x, y;

  exchange_ptr(&picture, &buffer);

  for (y=0; y<pic_y; y++)
   for (x=0; x<pic_x; x++)
    picture[y+pic_y*x]=buffer[x+pic_x*y];

  exchange_long(&pic_x, &pic_y);
  exchange_long(&scr_x, &scr_y);
  exchange_long(&start_x, &start_y);
  exchange_long(&m_x, &m_y);
}

void rotate_angle(float sin, float cos)
{ long x, y, x2, y2, x3, y3;
  long i;

  exchange_ptr(&picture, &buffer);

  for (i=0, y=0; y<pic_y; y++)
   for (x=0; x<pic_x; x++, i++)
   { x2=x-pic_x/2;
     y2=y-pic_y/2;

     x3=round(x2*cos+y2*sin);
     y3=round(-x2*sin+y2*cos);

     x3+=pic_x/2;
     y3+=pic_y/2;

     if (x3>0 && y3>0 && x3<pic_x && y3<pic_y)
      picture[i]=b(x3, y3);
       else picture[i]=255;
   }
}

void move(void)
{ long x, y;

  for (y=1; y<=pic_y; y++)
   for (x=0; x<pic_x; x++)
    if (y!=pic_y) p(x, y-1)=p(x, y);
     else p(x, y-1)=255;
}

void dline(long x1, long y1, long x2, long y2)
{ float x, y, r, dx=x2-x1, dy=y2-y1;
  long i;

  if (dx!=0 && dy/dx<=1 && dy/dx>=-1)
  { r=dy/dx;
    if (x1>x2) i=y1, y1=y2, y2=i, i=x1, x1=x2, x2=i;

    for (y=y1; x1!=x2+1; x1++, y+=r)
     p(x1, round(y))=ac;
  }

  else if (dy!=0)
  { r=dx/dy;
    if (y1>y2) i=y1, y1=y2, y2=i, i=x1, x1=x2, x2=i;

    for (x=x1; y1!=y2+1; y1++, x+=r)
     p(round(x), y1)=ac;
  }
}

#define zr 0.8
#define px2 (pic_x/2)
#define py2 (pic_y/2)

void zoom_out(void)
{ long x, y, rx, ry;

  exchange_ptr(&picture, &buffer);

  for (x=0; x<pic_x; x++)
   for (y=0; y<pic_y; y++)
    p(x, y)=-1;

  for (y=0, ry=0; y<py2; y++, ry=round(y*zr))
   for (x=0, rx=0; x<px2; x++, rx=round(x*zr))
   { p(px2+rx, py2+ry)=b(px2+x, py2+y);
     p(px2-rx, py2+ry)=b(px2-x, py2+y);
     p(px2+rx, py2-ry)=b(px2+x, py2-y);
     p(px2-rx, py2-ry)=b(px2-x, py2-y);
   }
}

void circle(long x1, long y1, long x2, long y2)
{ float sqr_r=sqr(x1-x2)+sqr(y1-y2);
  long x, y, r=round(sqrt(sqr_r));

  for (x=0; x<r; x++)
  { y=round(sqrt(sqr_r-sqr(x)));

    if (in_range(x1+x, y1+y))
     p(x1+x, y1+y)=ac;

    if (in_range(x1-x, y1+y))
     p(x1-x, y1+y)=ac;

    if (in_range(x1+x, y1-y))
     p(x1+x, y1-y)=ac;

    if (in_range(x1-x, y1-y))
     p(x1-x, y1-y)=ac;

    if (in_range(x1+y, y1+x))
     p(x1+y, y1+x)=ac;

    if (in_range(x1-y, y1+x))
     p(x1-y, y1+x)=ac;

    if (in_range(x1+y, y1-x))
     p(x1+y, y1-x)=ac;

    if (in_range(x1-y, y1-x))
     p(x1-y, y1-x)=ac;
  }
}

void save_file(long x1, long y1, long x2, long y2)
{ static long num=0;
  char ffn[MAX_PATH];
  long x, y;
  FILE *f;

  lsort(&x1, &x2);
  lsort(&y1, &y2);
  num++;

  sprintf(ffn, "%ldx%ld.%ld", x2-x1+1, y2-y1+1, num);
  if ((f=fopen(ffn, "wb"))==NULL)
   return;

  for (y=y1; y<=y2; y++)
   for (x=x1; x<=x2; x++)
    fputc(p(x, y), f);

  fclose(f);
}

long reset_mouse(void)
{ __dpmi_regs regs;

  regs.x.ax=0x0000;
  __dpmi_int(0x33, &regs);

  return(regs.x.ax);
}

long get_mouse(long *x, long *y)
{ __dpmi_regs regs;

  regs.x.ax=0x0003;
  __dpmi_int(0x33, &regs);

  *x=regs.x.cx>>1;
  *y=regs.x.dx;

  return(regs.x.bx);
}

extern unsigned char start_5x5_font[];

void set_pal(palettetype *pal)
{
  asm volatile ("
	movw $0x3c8, %%dx		# reset PEL
	xorb %%al, %%al
	outb %%al, %%dx

	movl $768, %%ecx		# write palette
	incw %%dx
	rep
	outsb"
       :
       : "S" (pal)
       : "eax", "ecx", "edx", "esi");
}

void get_pal(palettetype *pal)
{ asm volatile ("
	movw $0x3c7, %%dx		# reset PEL
	xorb %%al, %%al
	outb %%al, %%dx

	movl $768, %%ecx		# get palette
	movw $0x3c9, %%dx
	rep
	insb"
       :
       : "D" (pal)
       : "eax", "edx", "edi");
}

long _overwrite_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y)
{ long s=0;

  if (y>=0 && y<res_y-4)
   for (; *text!=0; text++, x+=6, s+=6)
    if (x>=0 && x<res_x-6)
    { asm volatile ("
	pushw %%es

	pushw %0				# load es register
	popw %%es

	movb %2, %%ah				# get color in ah

	movl $5, %%ecx
    0:
	pushl %%ecx
	movl $6, %%ecx

	movb (%%esi), %%bl			# get letter byte
	incl %%esi

    1:
	xorb %%al, %%al 			# decide which color to write
	shlb $1, %%bl
	jnc 2f
	movb %%ah, %%al
    2:
	stosb
	loop 1b

	popl %%ecx				# next scanline
	addl %1, %%edi

	loop 0b

	popw %%es"
       :
       : "g" (ldt), "g" (res_x-6), "g" (color),
	 "S" (&start_5x5_font[*text*5]), "D" (addr+x+y*res_x)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
    }

  return(s);
}

void m3_init(void)
{
  __dpmi_regs regs;

  regs.h.ah=0;
  regs.h.al=3;
  __dpmi_int(0x10, &regs);
}

long m13_init(void)
{ __dpmi_regs regs;
  
  regs.x.ax=0x0013;
  __dpmi_int(0x10, &regs);

  regs.h.ah=0x0f;
  __dpmi_int(0x10, &regs);

  return(regs.h.al==0x13);
}

void m13_clear_screen(void)
{ asm volatile ("
	pushw %%es

	pushw %0			# load es with vga segment
	popw %%es

	movl $0xa0000, %%edi		# clear video memory
	xorl %%eax, %%eax
	movl $0x4000, %%ecx
	rep
	stosl

	popw %%es"
       :
       : "g" (_dos_ds)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_pixel(long x, long y, long color)
{ if (x>=0 && x<320 && y>=0 && y<200)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	movb %%al, %%es:(%%edi)

	popw %%es"
       :
       : "g" (_dos_ds), "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_xpixel(long x, long y, long color)
{ if (x>=0 && x<320 && y>=0 && y<200)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	xorb %%al, %%es:(%%edi)

	popw %%es"
       :
       : "g" (_dos_ds), "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_bar(long x, long y, long nx, long ny, long color)
{ if (nx>0 && ny>0)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	movl %1, %%ecx
  0:
	pushl %%ecx
	pushl %%edi

	movl %2, %%ecx
	rep
	stosb

	popl %%edi
	popl %%ecx

	addl $320, %%edi
	loop 0b
		 
	popw %%es"
       :
       : "g" (_dos_ds), "g" (ny), "g" (nx),
	 "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

long m13_overwrite_text(long x, long y, long color, char *text, ...)
{ char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  return(_overwrite_text(x, y, color, text_string, _dos_ds, (unsigned char *)0xa0000, 320, 200));
}


