/* Main function of the game. Calls all initialisation routines and
   then starts the selection menu.
*/

#include <tank.h>
#include <string.h>
#include <float.h>
#include <sys/farptr.h>
#include <go32.h>

/* We're only using 8 bit color from Allegro */

DECLARE_COLOR_DEPTH_LIST(COLOR_DEPTH_8);

extern menustruct mm[];

long missing=FALSE, unused=FALSE, debug=FALSE, lock_maps=FALSE;
long get_info=FALSE, invunerable=FALSE, unlimited_lock=FALSE;
long setup_game=FALSE, no_mission_fail=FALSE, unlimited_weapons=FALSE;
long unlock_maps=FALSE;
int shareware=FALSE;

static void test_keyboard(void)
{ unsigned k;

  printf("Testing keyboard: ");

  do
  { k = wait_key();
    printf("%.4x ", k);
  } while (k != RKEY_ESC);

  printf("\n\nTesting complete.\n");
  exit(0);
}

void main(long argc, char **argv, char **env)
{ 
  setvbuf(stdout, NULL, _IONBF, 0);

  printf("Tank Arena - Copyrights (C) 1996-1998 3.14 Software (Compiled: " __DATE__ ").\n\n");

  /* Start keyboard driver */

  if (!init_keyboard())
    error("Unable to install keyboard driver.\n");

  atexit(deinit_keyboard);
  
  /* scan parameters */

  for (; argc>1; argc--)
  {
    if (stricmp(argv[argc-1], "-SETUP")==0)
     setup_game=TRUE;

    else if (stricmp(argv[argc-1], "-SETSOUND")==0)
      setsound();

    else if (stricmp(argv[argc-1], "-KEYTEST")==0)
     test_keyboard();

    else if (stricmp(argv[argc-1], "-DEBUG")==0)
     debug=TRUE;

    else if (stricmp(argv[argc-1], "-INFO")==0)
     get_info=TRUE;
     
    else if (!shareware && stricmp(argv[argc-1], "-MISSING")==0)
     missing=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-UNUSED")==0)
     unused=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-AUTOLOCK")==0)
     lock_maps=TRUE;
    
    else if (!shareware && stricmp(argv[argc-1], "-GOD")==0)
     invunerable=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-NOLOCKLIMIT")==0)
     unlimited_lock=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-NOFAIL")==0)
     no_mission_fail=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-LOADSOFWEAPONS")==0)
     unlimited_weapons=TRUE;

    else if (!shareware && stricmp(argv[argc-1], "-CODES")==0)
     list_codes();

    else if (!shareware && stricmp(argv[argc-1], "-UNLOCK")==0)
     unlock_maps=TRUE;
    
    else printf("Unrecognised option: %s\n\n", argv[argc-1]);
  }

  /* Allegro init */

  allegro_init();	  

  /* Set default configuration file */

  set_config_file("tank.cfg");

  /* Install game timer */
  
  init_game_timer();

  /* initialize random generator */

  srandom(_farpeekl(_dos_ds, 0x46c));

  /* reset FPU */

  _fpreset();
  _control87(EM_INVALID|EM_DENORMAL|EM_ZERODIVIDE|EM_OVERFLOW|EM_UNDERFLOW|EM_INEXACT|PC_24|RC_CHOP, MCW_EM|MCW_PC|MCW_RC);

  /* Read settings from tank.cfg */
  
  read_settings();

  /* Start setup screen? */

  if (setup_game)
    setup_hardware();

  /* Init hardware */

  init_various_graph();

  if (!get_info)
  {
    init_joystick();
    init_night_mission();
    init_screen_buffer();
    init_sound_card();
  }

  do_gamma_correction();

  /* initialize graphics, we're using the init call from allegro, so we
     can use both my own functions and the allegro functions
   */

  fade_out(3);

  set_gfx_mode(GFX_VGA, 320, 200, 0, 0);

  /* Get info? */

  if (get_info)
  {
    arena_info();

    error("Overview of currently supported command line options:\n\n"
	  "-SETUP          Enter the setup menu.\n"
	  "-SETSOUND       Enter the sound setup menu.\n"
	  "-INFO           Displays the help file.\n"
	  "-DEBUG          Displays memory consumption and frame rate.\n"
	  "                Might give extra errors during game play.\n\n");
  }

  /* Start game */

  init_player("DATA/TUNE.MID");
  write_title();
  
  do
  {
    menu(mm);

    message("EXIT TO DOS? (Y/N)");

  } while (wait_yesno()==RKEY_N);

  save_settings();

  fade_out(3);
}
