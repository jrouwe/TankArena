/* Compute palette and color translation table for night mission */

#include <stdio.h>

typedef struct
{

  unsigned char red, green, blue;

} palettetype[256];

#define color_moving_low    144
#define color_moving_high   171

#define TRUE		    (0==0)
#define FALSE		    (0==1)

#define sqr(x)		    ((x)*(x))

FILE *f;
palettetype pal, npal;
long i, j, c, m_j, m_i, first;
float min, dst;
unsigned char link[256], link2[256];

void main(void)
{ setvbuf(stdout, NULL, _IONBF, 0);

  f=fopen("/TANK/DATA/PALETTE.DAT", "rb");
  fread(pal, sizeof(pal), 1, f);
  fclose(f);

  for (i=0; i<256; i++) link[i]=i;

  for (c=0; c<126; c++)
  { first=TRUE;

    for (i=0; i<255; i++)
     if (link[i]==i && !(i>=color_moving_low && i<color_moving_high))
      for (j=i+1; j<255; j++)
       if (link[j]==j && !(j>=color_moving_low && j<color_moving_high))
       { dst=sqr(pal[i].red-pal[j].red)+sqr(pal[i].green-pal[j].green)+sqr(pal[i].blue-pal[j].blue);
	 if (first || dst<min) min=dst, m_j=j, m_i=i, first=FALSE;
       }

    printf(".");

    link[m_j]=m_i;
  }

  i=128;

  for (j=0; j<255; j++)
  { if (j>=color_moving_low && j<color_moving_high)
    { npal[j].red=pal[j].red;
      npal[j].green=pal[j].green;
      npal[j].blue=pal[j].blue;
      link2[j]=j;
    }
    else if (link[j]==j)
    { npal[i].red=pal[j].red;
      npal[i].green=pal[j].green;
      npal[i].blue=pal[j].blue;
      link2[j]=i;

      i++;
      if (i==color_moving_low) i=color_moving_high;
    }
    else link2[j]=link2[link[j]];
  }

  for (i=0; i<128; i++)
  { j=(npal[i+128].red*30+npal[i+128].green*59+npal[i+128].blue*11)/500;
    npal[i].red=j, npal[i].green=j, npal[i].blue=j;

    npal[i+128].red*=0.7;
    npal[i+128].green*=0.7;
    npal[i+128].blue=npal[i+128].blue*0.4+10;
  }

  for (i=0; i<255; i++)
   link2[i]-=128;

  link2[255]=255;

  f=fopen("/TANK/DATA/NIGHT.LNK", "wb");
  fwrite(link2, sizeof(link2), 1, f);
  fclose(f);

  f=fopen("/TANK/DATA/NIGHT.PAL", "wb");
  fwrite(npal, sizeof(npal), 1, f);
  fclose(f);

  printf("\n");
}
