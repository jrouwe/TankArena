/* Convert BMP to our format */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error(char *msg)
{ printf(msg);
  exit(1);
}

void main(int argc, char **argv)
{ unsigned char *buffer, name[40], *tmp;
  int i, j, start, size_x, size_y, cols_used;
  FILE *f;

  setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<2)
   error("use: BMP2PIC <picture[.bmp]>\n");

  buffer=malloc(1000000);

  tmp=strchr(argv[1], '.');
  if (tmp!=NULL) *tmp=0;

  strcpy(name, argv[1]);
  strcat(name, ".BMP");

  if ((f=fopen(name, "rb"))==NULL)
   error("File does not exist.\n");

  fread(buffer, 1000000, 1, f);
  fclose(f);
  
  if (buffer[0]!='B' && buffer[1]!='M')
   error("Invalid header.\n");

  start=buffer[10]+256*buffer[11];
  size_x=buffer[18]+256*buffer[19];
  size_y=buffer[22]+256*buffer[23];
  cols_used=buffer[47]==0? 256:buffer[47];
  
  if (buffer[30]!=0)
   error("Packed BMP's unsupported.\n");
  
  printf("Image is %dx%d, starts at %d and uses %d colors.\n", size_x, size_y, start, cols_used);

  strcpy(name, argv[1]);
  strcat(name, ".PAL");

  f=fopen(name, "wb");

  for (i=0; i<cols_used; i++)
   for (j=2; j>=0; j--)
    fputc(buffer[54+i*4+j]>>2, f);

  for (; i<256; i++)
   for (j=2; j>=0; j--)
    fputc(0, f);

  fclose(f);
 
  f=fopen(argv[1], "wb");

  for (i=size_y-1; i>=0; i--)
   for (j=0; j<size_x; j++)
    fputc(buffer[start+i*size_x+j], f);

  fclose(f);
}
