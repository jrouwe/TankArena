/* Text mode based setup screen. I didn't feel like making this in graphics
   mode because there is so much user interaction required.
*/

#include <tank.h>
#include <string.h>
#include <conio.h>

#define UNK "UNKNOWN"

static void quit(int i);
static void select_controls(int i);
static void select_graph_driver(int i);
static void do_select_graph(int i);
static void configure_joy(int i);
static void configure_keys(int i);

static char *nkt[]={"NONE","ESC","1","2","3","4","5","6","7","8",
		    "9","0","-","=","BACKSP","TAB","Q","W","E","R",
		    "T","Y","U","I","O","P","[","]","ENTER","LCTRL",
		    "A","S","D","F","G","H","J","K","L",";",
		    "'","`","LSHIFT","\\","Z","X","C","V","B","N",
		    "M",",",".","/","RSHIFT","* (KEYPAD)","LALT","SPACE","CAPS","F1",
		    "F2","F3","F4","F5","F6","F7","F8","F9","F10","NUM",
		    "SCROLL","7 (KEYPAD)","8 (KEYPAD)","9 (KEYPAD)","- (KEYPAD)","4 (KEYPAD)","5 (KEYPAD)","6 (KEYPAD)","+ (KEYPAD)","1 (KEYPAD)",
		    "2 (KEYPAD)","3 (KEYPAD)","0 (KEYPAD)",". (KEYPAD)",UNK,UNK,UNK,"F11","F12",UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK},

	    *ekt[]={UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,"ENTER (KEYPAD)","RCTRL",
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,"PRTSCR",UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,"/ (KEYPAD)",UNK,"PRTSCR","RALT",UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,"HOME","UP","PGUP",UNK,"LEFT",UNK,"RIGHT",UNK,"END",
		    "DOWN","PGDN","INS","DEL",UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    "MACRO",UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK,
		    UNK,UNK,UNK,UNK,UNK,UNK,UNK,UNK},

	    *joyt[]={"----", "1---", "-2--", "12--", "--3-", "1-3-", "-23-", "123-",
		     "---4", "1--4", "-2-4", "12-4", "--34", "1-34", "-234", "1234"},

	    *ckey[]={"UP", "DOWN", "LEFT", "RIGHT", "FIRE", "AIM LEFT",
                     "AIM RIGHT", "WEAPON LEFT", "WEAPON RIGHT", "SHIELD"},

	    *jbut[]={"FIRE", "AIM", "WEAPON", "SHIELD"},

	    *cont[]={"CONTROL", "MODE"},

	    *korj[]={"KEYBOARD", "JOYSTICK"},

	    *rord[]={"RELATIVE", "DIRECT  "};

static textmenustruct
		  mm[]={{"Select Video Mode", select_graph_driver},
			{"Select Controls", select_controls},
			{"Configurate Joystick", configure_joy},
			{"Configurate Keys", configure_keys},
			{"Quit Setup", quit},
			{NULL, NULL}},

		  gm[]={{"320x200 Standard VGA (fastest)", do_select_graph},
			{"320x200 Tweaked mode VGA", do_select_graph},
			{"360x270 Tweaked mode VGA", do_select_graph},
			{"640x480 VESA Super VGA (Pentium)", do_select_graph},
			{NULL, NULL}};

static long select_res_x[] =	 { 320,     320,       360,	  640,		 };
static long select_res_y[] =	 { 200,     200,       270,	  480,		 };
static long select_num_pages[] = { 1,	    2,	       2,	  2,		 };
static long select_graph_drv[] = { GFX_VGA, GFX_MODEX, GFX_MODEX, GFX_AUTODETECT };

static void shaded_window(long x, long y, long nx, long ny, long color)
{ m3_write_shade(x+1, y+1, nx, ny);
  m3_write_window(x, y, nx, ny, color);
}

static void m3_message(char *text)
{ long x;

  x=(75-strlen(text))/2;

  shaded_window(x, 10, strlen(text)+4, 3, 0x2f);
  m3_write_text(x+2, 11, text);
}

static long text_menu(textmenustruct *mnu)
{ long i, x, y, nx=0, ny;

  for (ny=0; mnu[ny].string!=NULL; ny++)
   if (strlen(mnu[ny].string)>nx)
    nx=strlen(mnu[ny].string);

  nx+=14, ny+=4, x=(79-nx)/2, y=(24-ny)/2;

  gotoxy(-1, -1);

  m3_write_background();

  shaded_window(x, y, nx, ny, 0x4f);

  for (i=0, y+=2; mnu[i].string!=NULL; i++, y++)
   m3_write_text((79-strlen(mnu[i].string))/2, y, mnu[i].string);

  ny-=4, nx-=2, x++, y=y-ny, i=0;

  m3_write_bar(x, y, nx, 1, 0x40);

  for (;;)
   switch(wait_key())
   { case RKEY_UP:
      if (i>0)
      { m3_write_bar(x, y+i, nx, 1, 0x40);
	i--;
	m3_write_bar(x, y+i, nx, 1, 0x40);
      }
      break;

     case RKEY_DOWN:
      if (i<ny-1)
      { m3_write_bar(x, y+i, nx, 1, 0x40);
	i++;
	m3_write_bar(x, y+i, nx, 1, 0x40);
      }
      break;

     case RKEY_ENTER:
      if (mnu[i].funct!=NULL)
       mnu[i].funct(i);
      return(FALSE);

     case RKEY_ESC:
      return(TRUE);
   }
}

void setup_hardware(void)
{
  m3_init();

  for (;;)
    if (text_menu(mm))
      quit(0);
}

/**************************** GRAPHICS ***************************************/

static void select_graph_driver(int i)
{
  text_menu(gm);
}

static void do_select_graph(int i)
{
  res_x = select_res_x[i];
  res_y = select_res_y[i];
  num_pages = select_num_pages[i];
  graph_drv = select_graph_drv[i];
}

/**************************** SELECT CONTROLS ********************************/

static void select_controls(int i)
{ long x=0, y=0, x2, y2;

  m3_write_background();

  shaded_window(4, 4, 30, 7, 0x4f);
  m3_write_text(15, 5, "PLAYER 1");
  m3_write_bar(16, 7, 16, 2, 0x40);

  shaded_window(44, 4, 30, 7, 0x4f);
  m3_write_text(55, 5, "PLAYER 2");
  m3_write_bar(56, 7, 16, 2, 0x40);

  shaded_window(1, 15, 78, 9, 0x1f);
  m3_write_text(3, 16, "Press ENTER to toggle controller or mode. Press ESC when done. Please note");
  m3_write_text(3, 17, "that if you have only one joystick, you can't select it for player 1.");
  m3_write_text(3, 19, "RELATIVE mode: Rotate the tank by pressing LEFT/RIGHT, drive by pressing");
  m3_write_text(3, 20, "               UP/DOWN. (Best for keyboard.)");
  m3_write_text(3, 21, "DIRECT mode:   The tank will drive in the direction you press. (For");
  m3_write_text(3, 22, "               joystick and people who are unfamiliar with computers.)");

  for (;;)
  { gotoxy(x*40+18, y+8);

    for (x2=0; x2<2; x2++)
    { for (y2=0; y2<2; y2++)
       m3_write_text(x2*40+6, y2+7, cont[y2]);

      m3_write_text(x2*40+17, 7, korj[key_or_joy[x2]]);
      m3_write_text(x2*40+17, 8, rord[rel_or_dir[x2]]);
    }

    switch(wait_key())
    { case RKEY_LEFT:
       x=0;
       break;

      case RKEY_RIGHT:
       x=1;
       break;

      case RKEY_UP:
       if (y>0) y--;
       break;

      case RKEY_DOWN:
       if (y<1) y++;
       break;

      case RKEY_ENTER:
       if (y==0) key_or_joy[x]^=1;
	else rel_or_dir[x]^=1;
       break;

      case RKEY_ESC:
       return;
    }
  }
}

/**************************** SELECT KEYS ************************************/

static char *key_name(unsigned k)
{ return((k&0xff00)!=0? ekt[k&0x7f]:nkt[k&0x7f]);
}

static void configure_keys(int i)
{ long x=0, y=0, x2, y2;
  unsigned k;

  m3_write_background();

  shaded_window(4, 2, 34, 15, 0x4f);
  m3_write_text(17, 3, "PLAYER 1");
  m3_write_bar(20, 5, 16, 10, 0x40);

  shaded_window(42, 2, 34, 15, 0x4f);
  m3_write_text(55, 3, "PLAYER 2");
  m3_write_bar(58, 5, 16, 10, 0x40);

  shaded_window(1, 19, 78, 5, 0x1f);
  m3_write_text(3, 20, "Press ENTER to select, then press button. Press DEL to delete a button.");
  m3_write_text(3, 21, "Press ESC when done. Due to the keyboard architecture, many settings");
  m3_write_text(3, 22, "won't work very well. You'll have to experiment or leave it this way.");

  for (;;)
  { gotoxy(x*38+22, y+6);

    for (x2=0; x2<2; x2++)
     for (y2=0; y2<10; y2++)
     { m3_write_text(x2*38+6, y2+5, ckey[y2]);
       m3_write_text(x2*38+21, y2+5, "%-16s", key_name(keys[x2][y2]));
     }

    switch(wait_key())
    { case RKEY_LEFT:
       x=0;
       break;

      case RKEY_RIGHT:
       x=1;
       break;

      case RKEY_UP:
       if (y>0) y--;
       break;

      case RKEY_DOWN:
       if (y<9) y++;
       break;

      case RKEY_DEL:
       keys[x][y]=0;
       break;

      case RKEY_ENTER:
       m3_write_bar(x*38+20, y+5, 16, 1, 0x7f);

       k=wait_key();

       if (k==RKEY_ESC) goto dont_define;

       for (x2=0; x2<2; x2++)
	for (y2=0; y2<10; y2++)
	 if (keys[x2][y2]==k)
	  keys[x2][y2]=0;

       keys[x][y]=k;

       dont_define:;

       m3_write_bar(x*38+20, y+5, 16, 1, 0x7f);

       break;

      case RKEY_ESC:
       return;
    }
  }
}

/**************************** SELECT JOYSTICK ********************************/

static void configure_joy(int i)
{ long x=0, y=0, x2, y2, w;
  unsigned k, b;

  m3_write_background();

  shaded_window(4, 4, 30, 11, 0x4f);
  m3_write_text(15, 5, "PLAYER 1");
  m3_write_bar(20, 7, 12, 4, 0x40);
  m3_write_bar(20, 12, 12, 1, 0x40);

  shaded_window(44, 4, 30, 11, 0x4f);
  m3_write_text(55, 5, "PLAYER 2");
  m3_write_bar(60, 7, 12, 4, 0x40);
  m3_write_bar(60, 12, 12, 1, 0x40);

  shaded_window(1, 18, 78, 6, 0x1f);
  m3_write_text(3, 19, "Press ENTER to select, then press joystick button(s). Press DEL to delete");
  m3_write_text(3, 20, "a button. Press ESC when done. If you have 2 buttons on your joystick,");
  m3_write_text(3, 21, "you could define WEAPON as button 1 + 2, but you could also use the");
  m3_write_text(3, 22, "keyboard to switch weapons.");

  for (;;)
  { gotoxy(x*40+22, y+8);

    for (x2=0; x2<2; x2++)
    { for (y2=0; y2<4; y2++)
      { m3_write_text(x2*40+6, y2+7, jbut[y2]);
	m3_write_text(x2*40+21, y2+7, joyt[joy[x2].but[y2]]);
      }

      m3_write_text(x2*40+6, 12, "SENSITIVITY");
      m3_write_text(x2*40+21, 12, "%-12ld", 100-joy[x2].sensitivity);
    }

    switch(wait_key())
    { case RKEY_LEFT:
       x=0;
       break;

      case RKEY_RIGHT:
       x=1;
       break;

      case RKEY_UP:
       if (y==5) y=3;
       else if (y>0) y--;
       break;

      case RKEY_DOWN:
       if (y<3) y++;
       else if (y==3) y=5;
       break;

      case RKEY_DEL:
       joy[x].but[y]=0;
       break;

      case RKEY_ENTER:
       m3_write_bar(x*40+20, 7+y, 12, 1, 0x7f);

       if (y!=5)
       { b=0;

	 while (b==0 || read_joy_buttons()!=0)
	 { b|=read_joy_buttons();

	   w=tinit(10);
	   twait(w);

	   if (scan_key()==RKEY_ESC)
	    goto dont_define;
	 }

	 for (x2=0; x2<2; x2++)
	  for (y2=0; y2<4; y2++)
	   if (joy[x2].but[y2]==b)
	    joy[x2].but[y2]=0;

	 joy[x].but[y]=b;
       }
       else do
       { k=wait_key();

	 switch(k)
	 { case RKEY_LEFT:
            if (joy[x].sensitivity<100)
             joy[x].sensitivity+=5;
            break;

	   case RKEY_RIGHT:
	    if (joy[x].sensitivity>0)
	     joy[x].sensitivity-=5;
	    break;
	 }

	 m3_write_text(x*40+21, 12, "%-12ld", 100-joy[x].sensitivity);

       } while (k!=RKEY_ESC && k!=RKEY_ENTER);

       dont_define:;

       m3_write_bar(x*40+20, 7+y, 12, 1, 0x7f);

       break;

      case RKEY_ESC:
       return;
    }
  }
}

/**************************** EXIT *******************************************/

static void quit(int i)
{
  m3_message("Save settings? (Y/N)");

  for (;;)
   switch(scan_key())
   { case RKEY_Y:
      m3_clear_screen();
      gotoxy(1, 1);

      save_settings();

      error("Setup completed.\n\n");
      break;

     case RKEY_N:
      m3_clear_screen();
      gotoxy(1, 1);

      error("Setup cancelled.\n\n");
      break;
   }
}
