/* Sets up the game in SELECT.MAP and shows all 8 user tanks. Waits for
   user to select a tank.
*/

#include <tank.h>
#include <dir.h>
#include <unistd.h>
#include <string.h>

extern long lose;

long pl_tanks[2]={0, 0};
  
long select_tanks(char *fn)
{
  long allowed[8], exit = 0, res = TRUE, i, j, k, np = tnow, done;
  maptype mp;

  static void rewrite(void)
  { int x, y, x2, y2, tn, c;
    objectstruct *o;

    for (x=0, x2=16; x<4; x++, x2+=76)
     for (y=0, y2=26; y<2; y++, y2+=86)
     { tn=x+4*y;

       for (o=obj; o!=NULL; o=o->next)
	if (o->type==TANK_TYPE && o->d.tk.tank_num==tn)
	 break;

       if (o!=NULL)
       { ssx=o->x-13;
	 if (ssx<0) ssx=0;
	 if (ssx>map.sx*b_size-buf_x) ssx=map.sx*b_size-buf_x;

	 ssy=o->y-13;
	 if (ssy<0) ssy=0;
	 if (ssy>map.sy*b_size-buf_y) ssy=map.sy*b_size-buf_y;

	 bottom_layer(0, ssx, ssy);

	 write_objects(NULL);

	 if (!allowed[tn])
	  b_convert(scr, gray, 60*60);

	 if (pl_tanks[0]==tn && pl_tanks[1]==tn) c=64;
	 else if (pl_tanks[0]==tn) c=143;
	 else if (pl_tanks[1]==tn) c=127;
	 else c=0;

	 if (c!=0)
	 { b_bar(0, 0, 60, 2, c);
	   b_bar(0, 58, 60, 2, c);
	   b_bar(0, 0, 2, 60, c);
	   b_bar(58, 0, 2, 60, c);
	 }

	 blit(scrbuf, screen, 0, 0, x2, y2, 60, 60);
       }

       if (pl_tanks[0]==tn && (exit&1)!=0)
	m13_add_image(x2-14, y2-11, 33, 33, intern[TANKICO].ptr);

       if (pl_tanks[1]==tn && (exit&2)!=0)
	m13_add_image(x2+30, y2-11, 33, 33, intern[TANKICO].ptr);
     }
  }

  static void write_names(void)
  { int x, y, x2, y2, tn;

    m13_write_text(100, 5, 15, "SELECT YOUR TANKS:");

    for (x=0, x2=46; x<4; x++, x2+=76)
     for (y=0, y2=91; y<2; y++, y2+=86)
     { tn=x+y*4;
       m13_write_text(x2-strlen(tank[tn].general.name)*3, y2, 15, tank[tn].general.name);
     }
  }	       

  static void menu_control(void)
  { for (k=scan_key(); k!=0xffff; k=scan_key())
     for (i=0; i<2; i++)
     { if ((exit&(1<<i))==0)
       { if (k==keys[i][PLAYER_UP])
	  pl_tanks[i]-=4;

	 else if (k==keys[i][PLAYER_DOWN])
	  pl_tanks[i]+=4;

	 else if (k==keys[i][PLAYER_LEFT])
	  pl_tanks[i]--;

	 else if (k==keys[i][PLAYER_RIGHT])
	  pl_tanks[i]++;

	 else if (k==keys[i][PLAYER_FIRE] && allowed[pl_tanks[i]] &&
		  (pl_tanks[i]!=7 || game.wins[i]<game.wins[i^1]))
	  exit|=1<<i;
       }

       if (k==RKEY_ENTER && allowed[pl_tanks[0]] && allowed[pl_tanks[1]] &&
	   (pl_tanks[0]!=7 || game.wins[0]<game.wins[1]) &&
	   (pl_tanks[1]!=7 || game.wins[1]<game.wins[0]))
	exit=3;

       else if (k==RKEY_ESC)
	res=FALSE, exit=3;

       if (pl_tanks[i]<0) pl_tanks[i]=0;
       if (pl_tanks[i]>7) pl_tanks[i]=7;
     }
  }

  fclose(read_header(fn, ".MAP", "rb", &mp));

  for (i=0; i<6; i++)
   switch(tank[i].type)
   { case TANK:
      allowed[i]=orred(MAPC_TANK, mp.map_contents);
      break;

     case CHOPPER:
      allowed[i]=orred(MAPC_CHOP, mp.map_contents);
      break;

     case PLANE:
      allowed[i]=orred(MAPC_PLANE, mp.map_contents);
      break;

     default:
      allowed[i]=FALSE;
      break;
   }

  allowed[6]=orred(MAPC_RACE, mp.map_contents);
  allowed[7]=orred(MAPC_TANK, mp.map_contents) && lose>0 && abs(game.wins[0]-game.wins[1])>=lose*2-1;

  for (i=0; i<2; i++)			// auto select available tank
   if (pl_tanks[i]>7 || !allowed[pl_tanks[i]])
    for (j=0; j<7; j++)
     if (allowed[j])
     { pl_tanks[i]=j;
       break;
     }

  game.winner=0;			// don't kill tanks entirely

  load_map("SELECT", TRUE);

  set_black();				// write framework
  write_panel();
  write_names();

  set_window_size(60, 60);		// write first screen
  rewrite();

  do
  { if (exit!=3)
     start_fade_in(&pal);
     else start_fade_out();

    do
    { twait(np);
      np=tinit(2);
    
      menu_control();
      transportation();
      control_effects();
      control_bullets();
      control_parts();
      check_obj_overlap();
      control_objects();
      palette_effects();
      rewrite();

      done=!do_fade_step();

    } while (exit!=3);
  } while (!done);

  clear(screen);

  return(res);
}

