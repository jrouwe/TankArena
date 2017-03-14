/* The routines writing logo/title/credits */

#include <tank.h>
#include <math.h>
#include <unistd.h>

static char *credits[]={"3.14 SOFWARE PRESENTS:",
			"TANK ARENA",
			"",
			"PROGRAMMING BY:",
			"JORRIT ROUWE",
			"",
			"GRAPHICS BY:",
			"DENNIS MEDEMA",
			"",
			"MUSIC BY:",
			"JANNE LEINONEN",
			NULL};

static char *credits2[]={"THANKS TO:",
			 "",
			 "D.J. DELORIE (DJGPP)",
			 "CHARLES SANDMANN (CWSDPMI)",
			 "SHAWN HARGREAVES (ALLEGRO)",
			 NULL};

void display_credits(char *fn, int x, int y, char *text[])
{
  char ffn[MAX_PATH];
  BITMAP *bmp;
  PALETTE pal;
  int i;

  sprintf(ffn, "DATA/BITMAPS.DAT#%s", fn);
  bmp = load_bmp(ffn, pal);
  if (bmp == 0) error("Unable to find: %s\n", fn);

  set_black();
  blit(bmp, screen, 0, 0, 0, 0, 320, 200);

  for (i=0; text[i]!=NULL; i++);

  y -= (i*7)/2;

  for (i=0; text[i]!=NULL; i++, y+=7)
   m13_write_text(x-strlen(text[i])*3, y, 255, text[i]);
   
  fade_in(pal, 3);
  wait_key();
  fade_out(3);

  destroy_bitmap(bmp);
}

void write_title(void)
{
  int exit = FALSE;

  if (__file_exists("FLI/LOGO.FLI"))
    exit = !show_fli("FLI/LOGO.FLI");

  init_pictures();
  init_sounds();
  fade_out(3);

  if (exit) return;

  show_fli("FLI/TITLE.FLI");
  fade_out(3);

  display_credits("CREDITS.BMP", 70, 40, credits);
  display_credits("CREDITS2.BMP", 160, 100, credits2);
}
