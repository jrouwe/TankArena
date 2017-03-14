/* Batch Draw: for automated drawing operations like rotating tanks */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define p(x,y) 		picture[(x)+pic_x*(y)]
#define b(x,y) 		buffer[((unsigned)(x))+pic_x*((unsigned)(y))]
#define round(x)	((int)((x)>0? ((x)+0.5):((x)-0.5)))
#define sqr(x)          ((x)*(x))

void error_msg(char *msg);
void mirror_y(void);
void rotate_90(void);
void rotate_angle(float sin, float cos);
void zoom_out(void);
void mirror_x(void);

char *buffer, *buffer2, *picture;
long size, pic_x, pic_y;

void main(long argc, char **argv)
{ FILE *f;
  long i;

  setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<5)
   error_msg("USE: BDRAW <name> <size_x> <size_y> <commands: xy94+-z>\n");

  sscanf(argv[2], "%ld", &pic_x);
  sscanf(argv[3], "%ld", &pic_y);

  size=pic_x*pic_y, buffer=malloc(size), buffer2=malloc(size);
  picture=malloc(size);

  if (picture==NULL || buffer==NULL || buffer2==NULL)
   error_msg("Insuffient memory.\n");

  if ((f=fopen(argv[1], "rb"))!=NULL)
  { fread(picture, size, 1, f);
    fclose(f);
  } else error_msg("Unable to open file.\n");

  for (i=0; argv[4][i]!=0; i++)
   switch(toupper(argv[4][i]))
   { case 'X':
      mirror_x();
      break;

     case 'Y':
      mirror_y();
      break;

     case '9':
      rotate_90();
      break;

     case '4':
      rotate_angle(0.7071067, 0.7071067);
      break;

     case '+':
      rotate_angle(0.3826834, 0.9238795);
      break;

     case '-':
      rotate_angle(-0.3826834, 0.9238795);
      break;

     case 'Z':
      zoom_out();
      break;
   }

  f=fopen(argv[1], "wb");
  fwrite(picture, size, 1, f);
  fclose(f);

  printf("Saved file: %s.\n", argv[1]);
}

void error_msg(char *message)
{ printf(message);
  exit(1);
}

void exchange(char *p1, char *p2)
{ char t;

  t=*p1;
  *p1=*p2;
  *p2=t;
}

void exchange_ptr(char **p1, char **p2)
{ char *t;

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

void mirror_x(void)
{ unsigned x, y;

  for (y=0; y<pic_y/2; y++)
   for (x=0; x<pic_x; x++)
    exchange(&p(x, y), &p(x, pic_y-y-1));
}

void mirror_y(void)
{ unsigned x, y;

  for (y=0; y<pic_y; y++)
   for (x=0; x<pic_x/2; x++)
    exchange(&p(x, y), &p(pic_x-x-1, y));
}

void rotate_90(void)
{ unsigned x, y;

  exchange_ptr(&picture, &buffer);

  for (y=0; y<pic_y; y++)
   for (x=0; x<pic_x; x++)
    picture[y+pic_y*x]=buffer[x+pic_x*y];

  exchange_long(&pic_x, &pic_y);
}

void rotate_angle(float sin, float cos)
{ long x, y, x2, y2, x3, y3;
  unsigned i;

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
