/* Select map from list. Is called after you press ESC in a
   prompt "LOAD MAP".
*/

#include <tank.h>
#include <dir.h>
#include <unistd.h>
#include <string.h>

typedef struct
{ char name[13];
} filestruct;

#define list_length	20

long select_from_list(char *result)
{ long done=FALSE, fn=0, start=0, sel=0, k, s;
  struct ffblk ffblk;
  filestruct *fs;
  maptype mp;

  static int compare_files(const void *e1, const void *e2)
  { return(stricmp(((filestruct *)e1)->name, ((filestruct *)e2)->name));
  }

  static void rewrite_menu(void)
  { long i, y;

    for (i=start, y=33+big_height; i<start+list_length && i<fn; i++, y+=6)
    { s=m13_overwrite_text(125, y, 15, "%c %s", i==sel? '>':' ', fs[i].name);
      m13_bar(125+s, y, 220-125-s, 5, 0);
    }
  }

  write_panel();
  m13_add_bar(90, 20, 140, 160, 5);
  m13_bar(95, 25, 130, 150, 0);

  fs=allocate(max_files*sizeof(filestruct));

  done=findfirst("MAPS/*.MAP", &ffblk, 0);

  while(!done && fn<max_files && scan_key()!=RKEY_ESC)
  { if (game.mode==DUAL)
      fclose(read_header(ffblk.ff_name, "",  "rb", &mp));

    if (game.mode==MAP || mp.map_type==MAPT_DONTCARE || mp.map_type==MAPT_DUAL)
    { strcpy(fs[fn].name, ffblk.ff_name);
      *strchr(fs[fn].name, '.')=0;
      fn++;
    }

    done=findnext(&ffblk);
  }

  if (fn == 0)
  {
    message_key("NO MAPS FOUND");
    return(FALSE);
  }

  qsort(fs, fn, sizeof(filestruct), &compare_files);

  textout_centre(screen, big_font, "SELECT MAP", 160, 29, FNT_HIGH);

  rewrite_menu();

  do
  { k=wait_key();

    switch(k)
    { case RKEY_UP:
       sel--;
       break;

      case RKEY_DOWN:
       sel++;
       break;

      case RKEY_PGUP:
       sel-=list_length;
       break;

      case RKEY_PGDN:
       sel+=list_length;
       break;
    }

    if (sel<0) sel=0;
    if (sel>=fn) sel=fn-1;
    if (start>sel) start=sel;
    if (start<sel-(list_length-1)) start=sel-(list_length-1);

    rewrite_menu();

  } while(k!=RKEY_ESC && k!=RKEY_ENTER);

  if (k==RKEY_ENTER)
   strcpy(result, fs[sel].name);

  free(fs);

  clear(screen);

  return(k==RKEY_ENTER);
}
