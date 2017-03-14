/* Remembering/selecting of previous passwords */

#include <tank.h>
#include <dir.h>
#include <unistd.h>
#include <string.h>

typedef struct
{ char name[21];
} passstruct;

static passstruct *ps;
static long pn;

#define list_length 20

void read_passwords(void)
{
  FILE *f;

  pn = 0;

  ps = allocate(max_files*sizeof(passstruct));

  f = fopen("PASSWD.DAT", "rb");

  if (f != 0)
  {
    pn = fread(ps, sizeof(passstruct), max_files, f);
    fclose(f);
  }
}

long select_password_from_list(char *pw)
{
  long start=0, sel=0, k, s;

  static void rewrite_menu(void)
  { long i, y;

    for (i = start, y = 33+big_height; i < start+list_length && i < pn; i++, y += 6)
    {
      s = m13_overwrite_text(90, y, 15, "%c %s", i==sel? '>':' ', ps[i].name);
      m13_bar(90 + s, y, 220 - 90 - s, 5, 0);
    }
  }

  write_panel();
  m13_add_bar(50, 20, 220, 160, 5);
  m13_bar(55, 25, 210, 150, 0);
		   
  read_passwords();
		   
  if (pn==0)
  {
    message_key("NO PASSWORDS KNOWN");
    return(FALSE);
  }

  textout_centre(screen, big_font, "KNOWN PASSWORDS", 160, 29, FNT_HIGH);

  rewrite_menu();

  do
  {
    k = wait_key();

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
    if (sel>=pn) sel=pn-1;
    if (start>sel) start=sel;
    if (start<sel-(list_length-1)) start=sel-(list_length-1);

    rewrite_menu();

  } while (k != RKEY_ESC && k != RKEY_ENTER);

  if (k == RKEY_ENTER)
   strcpy(pw, ps[sel].name);

  free(ps);

  clear(screen);

  return(k == RKEY_ENTER);
}

void add_password_to_list(char *passwd)
{
  FILE *f;
  int i;

  if (*passwd == 0)
    return;

  read_passwords();

  for (i = 0; i < pn; i ++)
    if (strcmp(ps[i].name, passwd) == 0)
      {
	free(ps);
	return;
      }

  strcpy(ps[i].name, passwd);

  f = fopen("PASSWD.DAT", "wb");

  if (f != 0)
  {
    fwrite(ps, sizeof(passstruct) * (pn + 1), 1, f);
    fclose(f);
  }

  free(ps);
}
