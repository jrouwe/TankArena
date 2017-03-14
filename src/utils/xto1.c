/* Convert many 33x33 images to one */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

void error_msg(char *msg)
{ printf(msg);
  exit(1);
}

char buffer[4*33*4*33], pict[33][33];
long x, y, x2, y2, t, s, nx, ny, sx, sy, par;
FILE *f, *fout;

void main(long argc, char **argv)
{ setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<6)
   error_msg("USE: XTO1 <num_pict_x> <num_pict_y> <target> <source1> <source2> .. <sourceN>\n");

  sscanf(argv[1], "%ld", &nx);
  sscanf(argv[2], "%ld", &ny);

  sx=nx*33, sy=ny*33;

  if (nx<1 || nx>4 || ny<1 || ny>4)
   error_msg("Invalid number.\n");

  strupr(argv[3]);

  printf("%s <- ", argv[3]);

  if ((fout=fopen(argv[3], "wb"))==NULL)
   error_msg("open error.\n");

  chdir("/TANK/PICS");

  par=4+y*nx+x;

  for (y=0; y<sy; y+=33)
   for (x=0; x<sx; x+=33)
   { strupr(argv[par]);
     printf("%s, ", argv[par]);

     if ((f=fopen(argv[par], "rb"))==NULL)
      error_msg("open error.\n");

     fread(pict, 33*33, 1, f);

     for (x2=0; x2<33; x2++)
      for (y2=0; y2<33; y2++)
       buffer[x2+x+(y2+y)*sx]=pict[y2][x2];

     fclose(f);

     par++;
   }

  fwrite(buffer, sx*sy, 1, fout);

  chdir("..");

  fclose(fout);

  printf("done.\n");
}
