/* Converts 1 image to many 33x33 images */

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void error_msg(char *msg)
{ printf(msg);
  exit(1);
}

char buffer[8*33*8*33], picture[33][33], out[15];
long x, y, x2, y2, t, s, nx, ny;
FILE *f;

void main(long argc, char **argv)
{ setvbuf(stdout, NULL, _IONBF, 0);

  if (argc<4)
   error_msg("USE: 1TOX <name> <num_pic_x> <num_pic_y>\n");

  sscanf(argv[2], "%ld", &nx);
  sscanf(argv[3], "%ld", &ny);

  if (nx<1 || nx>8 || ny<1 || ny>8)
   error_msg("Invalid number.\n");

  nx*=33, ny*=33;

  strupr(argv[1]);

  printf("%s -> ", argv[1]);

  if ((f=fopen(argv[1], "rb"))==NULL)
  { chdir("..");

    error_msg("open error.\n");
  }

  fread(buffer, nx*ny, 1, f);

  fclose(f);

  chdir("/TANK/PICS");

  t=0;

  for (y=0; y<ny; y+=33)
   for (x=0; x<nx; x+=33)
   { for (x2=0; x2<33; x2++)
      for (y2=0; y2<33; y2++)
       picture[y2][x2]=buffer[(y2+y)*nx+x2+x];

     if (nx*ny <= 26)
       sprintf(out, "%s%c", argv[1], 'A'+(char)t);
     else
       sprintf(out, "%s%c%c", argv[1], 'A'+(char)(t/26), 'A'+(char)(t%26));

     printf("%s, ", out);

     if ((f=fopen(out, "wb"))==NULL)
      error_msg("open error.\n");

     fwrite(picture, 33*33, 1, f);

     t++;
   }

  chdir("..");

  printf("done.\n");

}
