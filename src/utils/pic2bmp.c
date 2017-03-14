/* Convert our picture format back into BMP */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error(char *msg)
{ printf(msg);
  exit(1);
}

void main(int argc, char **argv)
{ unsigned char *b;
  int i, j, sizex, sizey, padd;
  FILE *out, *in;

  setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<6)
   error("use: PIC2BMP <picture> <palette> <size_x> <size_y> <outpicture>\n");

  if (sscanf(argv[3], "%d", &sizex)==0)
   error("Invalid x resolution.\n");

  if (sscanf(argv[4], "%d", &sizey)==0)
   error("Invalid y resolution.\n");

  if ((out=fopen(argv[5], "wb"))==NULL)
   error("Unable to open output file.\n");

  b=malloc(1000000);

  memset(b, 0, 54);				// write header
  b[0]='B';                                     // ID = 'BM'
  b[1]='M';
  b[10]=54;					// start of image data = 4*256+54
  b[11]=4;
  b[14]=40;					// size of header
  b[18]=sizex&0xff;				// size x
  b[19]=sizex>>8;
  b[22]=sizey&0xff;				// size y
  b[23]=sizey>>8;
  b[26]=1;					// 1 color plane
  b[28]=8;					// 8 bits per pixel
  b[47]=0;					// number of colors used = 256
  b[51]=0;

  fwrite(b, 54, 1, out);

  if ((in=fopen(argv[2], "rb"))==NULL)          // open palette
   error("Palette file does not exist.\n");
  fread(b, 3*256, 1, in);
  fclose(in);
  
  for (i=0; i<256; i++) 			// convert palette
  { for (j=2; j>=0; j--)
     fputc(b[i*3+j]<<2, out);

    fputc(0, out);
  }

  if ((in=fopen(argv[1], "rb"))==NULL)
   error("Picture file does not exist.\n");
  fread(b, 1000000, 1, in);
  fclose(in);

  padd=(4-(sizex&3))&3;

  for (i=sizey-1; i>=0; i--)
  { for (j=0; j<sizex; j++)
     fputc(b[i*sizex+j], out);

    for (j=padd; j>0; j--)
     fputc(0, out);
  }
  
  fclose(out);
}
