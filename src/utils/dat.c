/* Convert many pictures into two files PICTURES.DAT and PICTURES.IDX */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dir.h>
#include <io.h>

#pragma pack(1)

typedef struct
{ char name[12];
  long addr;
} indextype;

#pragma pack()

#define picsize     1092
#define b_size	    33

#define TRUE	    (0==0)
#define FALSE	    (1==0)

char *f_idx="/TANK/DATA/PICTURES.IDX", *f_dat="/TANK/DATA/PICTURES.DAT";
char buffer[picsize], *mask="*.*";
long size, files, done, replace_all, i;
FILE *file, *idx, *dat;
struct ffblk ffblk;
indextype *x;

void error_msg(char *msg)
{ printf(msg);
  exit(1);
}

int compare(const void *x1, const void *x2)
{ return(strncmp(((indextype *)x1)->name, ((indextype *)x2)->name, 12));
}

void main(void)
{ setvbuf(stdout, NULL, _IONBF, 0);
	
  if ((x=malloc(1000000L))==NULL)
   error_msg("Out of memory.\n");

  printf("Storing ");

  if ((idx=fopen(f_idx, "rb"))!=NULL)
  { fseek(idx, 0L, SEEK_END);
    size=ftell(idx), files=size/sizeof(indextype);
    rewind(idx);

    fread(x, size, 1, idx);
    fclose(idx);

    dat=fopen(f_dat, "rb+");

    replace_all=FALSE;
  }
  else
  { files=0;

    dat=fopen(f_dat, "wb");

    replace_all=TRUE;
  }

  chdir("/TANK/PICS");

  done=findfirst(mask, &ffblk, 0);

  while (!done)
  { if (replace_all || (ffblk.ff_attrib&FA_ARCH)!=0)
    { printf("%s, ", ffblk.ff_name);

      file=fopen(ffblk.ff_name, "rb");
      fread(buffer, b_size*b_size, 1, file);
      fclose(file);

      _chmod(ffblk.ff_name, 1, 0);

      for (i=0; i<files; i++)
       if (strnicmp(ffblk.ff_name, x[i].name, sizeof(indextype))==0)
	break;

      if (i==files)
      { x[i].addr=i*picsize;
        memset(x[i].name, 0, 12);
        strncpy(x[i].name, ffblk.ff_name, 12);

	files++;
      }

      fseek(dat, x[i].addr, SEEK_SET);
      fwrite(buffer, picsize, 1, dat);
    }

    done=findnext(&ffblk);
  }

  fclose(dat);

  chdir("..");

  qsort(x, files, sizeof(indextype), &compare);

  idx=fopen(f_idx, "wb");
  fwrite(x, files*sizeof(indextype), 1, idx);
  fclose(idx);

  printf("done.\n");
}
