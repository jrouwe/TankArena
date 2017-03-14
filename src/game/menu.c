/* The main menu system. */

#include <tank.h>

void edit_map(void);
void dual_game(void);
void dual_v_computer_game(void);
void single_game(void);

static void select_rounds(void);
static void select_laps(void);
static void enable_losers_tank(void);
static void new_game(void);
static void game_options(void);
static void gamma_correction(void);
static void music_vol(void);
static void sound_vol(void);
static void keys_info(void);

extern long gamcor;

long ctrl[2]={0, 0}, lose=0, rnds=3, race_laps=3;

static int voltab[]={0, 85, 170, 255};

static char *loser[]={"Losers tank disabled",
		      "Losers tank enabled at 1 loss",
		      "Losers tank enabled at 3 losses"},

	    *svol[]={"Sound off",
		     "Sound volume - Low",
		     "Sound volume - Medium",
		     "Sound volume - High"},

	    *mvol[]={"Music off",
		     "Music volume - Low",
		     "Music volume - Medium",
		     "Music volume - High"};

menustruct mm[]={{"MAIN MENU", NULL},
		 {"Start new game", new_game},
		 {"Game options", game_options},
		 {"Map editor", edit_map},
		 {"Arena info", arena_info},
		 {"Default keys", keys_info},
		 {"Exit to DOS", NULL},
		 {NULL, NULL}};

static menustruct ngame[]={{"SELECT GAME MODE", NULL},
			   {"Player VS Player mode", dual_game},
			   {"One player VS Computer mode", single_game},
			   {"Two player VS Computer mode", dual_v_computer_game},
			   {"Leave this menu", NULL},
			   {NULL, NULL}},

		  gameop[]={{"GAME OPTIONS:", NULL},
			    {"Race consists of \xEC laps", select_laps},
			    {"Match consists of \xEC rounds", select_rounds},
			    {"", enable_losers_tank},
			    {"Gamma correction ??", gamma_correction},
			    {"", music_vol},
			    {"", sound_vol},
			    {"Leave this menu", NULL},
			    {NULL, NULL}};

/**************************** WRITE MENU **********************************/

void menu(menustruct *m)
{ long max, x0, y0, y, s=1, l;
  unsigned k;
	      
  clear(screen);

  for (;;)
  {
    x0 = 0, y0 = 180;

    for (max=1; m[max].string!=NULL; max++)
    {
      y0 -= big_height;

      l = big_length(m[max].string);
      if (x0 < l) x0 = l;
    }

    y0 /= 2;

    set_palette(pal);

    write_panel();
    m13_add_bar((280-x0)/2, y0-10, x0+40, 20+max*big_height+11, 5);
    m13_bar((290-x0)/2, y0-5, x0+30, 10+max*big_height+11, 0);
    
    textout_centre(screen, big_font, m[0].string, 160, y0, FNT_HIGH);

    do
    {
      for (y=1; y<max; y++)
	textout_centre(screen, big_font, m[y].string, 160, y0+11+y*big_height, s==y? FNT_HIGH:FNT_NORM);

      k=wait_key();

      switch(k)
      { case RKEY_UP:
	 if (s>1) s--;
	  else s=max-1;
	 break;

	case RKEY_DOWN:
	 if (s<max-1) s++;
	  else s=1;
	 break;

	case RKEY_ENTER:
	 if (m[s].funct==NULL) return;
	 m[s].funct();
	 break;

	case RKEY_ESC:
	 return;
      }
    } while(k!=RKEY_ENTER);
  }
}

/**************************** GAME OPTIONS ********************************/

static void game_options(void)
{ gameop[1].string[17]='0'+race_laps;
  gameop[2].string[18]=rnds=='~'? '\xEC':'0'+rnds;
  gameop[3].string=loser[lose];
  sprintf(gameop[4].string, "Gamma correction %ld", gamcor);
  gameop[5].string=mvol[music_volume/85];
  gameop[6].string=svol[sound_volume/85];

  menu(gameop);
}

static void enable_losers_tank(void)
{ lose=(lose+1)%3;
  gameop[3].string=loser[lose];
}

static void select_rounds(void)
{ if (rnds=='~') rnds=1;
  else
  { rnds+=2;
    if (rnds>9) rnds='~';
  }

  gameop[2].string[18]=rnds=='~'? '\xEC':'0'+rnds;
}

static void select_laps(void)
{ race_laps++;
  if (race_laps>6) race_laps=1;

  gameop[1].string[17]='0'+race_laps;
}

static void gamma_correction(void)
{ gamcor++;
  if (gamcor>3) gamcor=-3;

  sprintf(gameop[4].string, "Gamma correction %ld", gamcor);

  do_gamma_correction();
  set_palette(pal);
}

static void sound_vol(void)
{ sound_volume=voltab[(sound_volume/85+1)&3];
  gameop[6].string=svol[sound_volume/85];
}

static void music_vol(void)
{ music_volume=voltab[(music_volume/85+1)&3];
  gameop[5].string=mvol[music_volume/85];
  set_volume(0, music_volume);
}

/**************************** NEW GAME ************************************/

static void new_game(void)
{ menu(ngame);
}

/**************************** INFO ***************************************/

void arena_info(void)
{ FILE *f;

  f=open_file("DATA/TANK.HLP");
  view_file(f, 0, get_file_size(f));
  fclose(f);
}

void keys_info(void)
{
  FILE *f;

  f = open_file("DATA/KEYS.HLP");
  view_file(f, 0, get_file_size(f));
  fclose(f);
}

