/* Everything related with the keyboard/joystick and movement of tanks. */

#include <tank.h>

#define tnk o->d.tk

#define x_min          joy[i].x_min
#define x_mid          joy[i].x_mid
#define x_max          joy[i].x_max
#define y_min          joy[i].y_min
#define y_mid          joy[i].y_mid
#define y_max          joy[i].y_max
#define sensi          joy[i].sensitivity
#define but_pressed(n) (joy[i].but[n]!=0 && (k&joy[i].but[n])==joy[i].but[n])

typedef struct
{ char fire, shield;
} buttonstatus[2];

typedef struct
{ char up, down, left, right;
  char aim_left, aim_right, weapon_left, weapon_right;
} keystatus[2];

typedef struct
{ long x, y;
  char aim, weapon;
} joystatus[2];

extern long gamcor;

keytype keys={{0x0025, 0x0033, 0x0032, 0x0034, 0x002c, 0x002d, 0x002e, 0x001f, 0x0020, 0x001e},
	      {0x0048, 0x0050, 0x004B, 0x004D, 0xE053, 0xE04f, 0xE051, 0xe047, 0xe049, 0xe052}};

joytype joy={{50, {0, 0, 0, 0}},
	     {50, {1, 2, 4, 8}}};

long key_or_joy[2]={0, 0}, rel_or_dir[2]={0, 0}, id_select=1;
extern long grab, grab_no;

static keystatus ks;
static joystatus js;
static buttonstatus bs;

void init_joystick(void)
{ unsigned k, b;
  long i, x, y, w;

  for (i=0; i<2; i++)
   if (key_or_joy[i]==CONTR_JOY)
   { printf("Joystick for player %ld ", i+1);

     if (detect_joystick(i^1))
     { printf("detected.\n\n"
	      "Move joystick in all directions, then center and push button.\n");

       x_min=0xffff, x_max=0, y_min=0xffff, y_max=0;

       do
       { read_joystick(i^1, &x, &y);

	 if (x<x_min) x_min=x;
	 if (x>x_max) x_max=x;
	 if (y<y_min) y_min=y;
	 if (y>y_max) y_max=y;

	 w = tinit(10);
	 twait(w);

	 b = i == 0? (read_joy_buttons() & 0xc) : (read_joy_buttons() & 0x3);

	 k=scan_key();

       } while (b==0 && (k==0 || (k&0x0080)!=0));

       x_mid=x;
       y_mid=y;

       printf("\n");

       if (x_min == x_mid || x_max == x_mid || y_min == y_mid || y_max == y_mid)
	 error("Joystick calibration failed!\n");
     }
     else error("not detected.\n");
   }
}

void reset_control(long n)
{ bs[n].fire=0;
  bs[n].shield=0;

  ks[n].left=0;
  ks[n].right=0;
  ks[n].up=0;
  ks[n].down=0;

  ks[n].aim_left=0;
  ks[n].aim_right=0;
  ks[n].weapon_left=0;
  ks[n].weapon_right=0;

  js[n].x=0;
  js[n].y=0;

  js[n].aim=0;
  js[n].weapon=0;
}

void reset_controls(void)
{ long i;

  for (i=0; i<2; i++)
    reset_control(i);
}

void read_controls(void)
{ objectstruct *o;
  char str[20];
  unsigned k;
  long x, y;
  long i;

  for (i=0; i<2; i++)
  { ks[i].left&=0x01;
    ks[i].right&=0x01;
    ks[i].up&=0x01;
    ks[i].down&=0x01;

    bs[i].fire&=0x01;
    bs[i].shield&=0x01;

    ks[i].aim_left&=0x01;
    ks[i].aim_right&=0x01;
    ks[i].weapon_left&=0x01;
    ks[i].weapon_right&=0x01;

    js[i].aim=0;
    js[i].weapon=0;
  }

  for (i=0; i<2; i++)
   if (key_or_joy[i]==CONTR_JOY)
   { read_joystick(i^1, &x, &y);

     if (x<x_mid)
      js[i].x=(x-x_mid)*100/(x_mid-x_min);
      else js[i].x=(x-x_mid)*100/(x_max-x_mid);

     if (y<y_mid)
      js[i].y=(y-y_mid)*100/(y_mid-y_min);
      else js[i].y=(y-y_mid)*100/(y_max-y_mid);

     k=read_joy_buttons();

     if (but_pressed(2)) js[i].weapon=1;
     if (but_pressed(1)) js[i].aim=1;

     if (but_pressed(3)) bs[i].shield|=0x10;
     if (but_pressed(0) && js[i].weapon==0) bs[i].fire|=0x10;
   }

  for (k=scan_key(); k!=0xffff; k=scan_key())
  { if (game.mode==DUALVC)
    { if (k==RKEY_LALT || k==RKEY_RALT)
       id_select=0;

      if (k==(RKEY_LALT+0x80) || k==(RKEY_RALT+0x80))
       id_select=1;
    }

    if (k==RKEY_ESC)
     game.exit_game=TRUE;

    else if (k==RKEY_PRTSCRN)
     grab=TRUE;

    else if (k==RKEY_F11)
    { gamcor++;
      if (gamcor>3) gamcor=-3;

      sprintf(str, "LEVEL %ld", gamcor);
      create_message(game.tk[0], "GAMMA CORRECTION", str);

      do_gamma_correction();
    }

    else if (k==RKEY_PLUS)
    { if (win_size>0)
      {
	win_size--;
	compute_window_size();
	clear_layers();
	write_panel();
      }
    }

    else if (k==RKEY_MIN)
    { if (win_size<5)
      {
	win_size++;
	compute_window_size();
	clear_layers();
	write_panel();
      }
    }

    else if (k==RKEY_F12)
      show_radar = !show_radar;

    else if (k==RKEY_P)
    { game_pause=1;

      wait_key();

      game_pause=0;

      reset_controls();
    }

    else if (k>=RKEY_F1 && k<=RKEY_F10 && game.mode!=DUAL)
    { i=k-RKEY_F1+1;

      /* search for tank with id: i */

      switch (game.mode)
      { case SINGLE:
	 game.id[0]=i;
	 game.tk[0]=NULL;

	 for (o=obj; o!=NULL; o=o->next)
	  if (o->type==TANK_TYPE && tnk.contr.id==i)
	   game.tk[0]=o;

	 break;

	default:
	 if (game.id[id_select^1]!=i)
	 { game.id[id_select]=i;
	   game.tk[id_select]=NULL;
	
	   for (o=obj; o!=NULL; o=o->next)
	    if (o->type==TANK_TYPE && tnk.contr.id==i)
	    { game.tk[id_select]=o;
	      tnk.contr.num=id_select;
	    }
	  }
	  break;
      }
    }

    else if ((k&0x80)==0)
    { for (i=0; i<2; i++)
      { if (k==keys[i][PLAYER_UP])
	 ks[i].up=0x11;

	else if (k==keys[i][PLAYER_DOWN])
	 ks[i].down=0x11;

	else if (k==keys[i][PLAYER_LEFT])
	 ks[i].left=0x11;

	else if (k==keys[i][PLAYER_RIGHT])
	 ks[i].right=0x11;

        else if (k==keys[i][PLAYER_FIRE])
	 bs[i].fire=0x11;

	else if (k==keys[i][PLAYER_AIM_LEFT])
	 ks[i].aim_left=0x11;

        else if (k==keys[i][PLAYER_AIM_RIGHT])
	 ks[i].aim_right=0x11;

	else if (k==keys[i][PLAYER_WEAPON_LEFT])
	 ks[i].weapon_left=0x11;

        else if (k==keys[i][PLAYER_WEAPON_RIGHT])
	 ks[i].weapon_right=0x11;

	else if (k==keys[i][PLAYER_SHIELD])
	 bs[i].shield=0x11;
      }
    }

    else
    { k&=0xff7f;

      for (i=0; i<2; i++)
      { if (k==keys[i][PLAYER_UP])
	 ks[i].up&=0x10;

	else if (k==keys[i][PLAYER_DOWN])
	 ks[i].down&=0x10;

	else if (k==keys[i][PLAYER_LEFT])
	 ks[i].left&=0x10;

	else if (k==keys[i][PLAYER_RIGHT])
	 ks[i].right&=0x10;

        else if (k==keys[i][PLAYER_FIRE])
	 bs[i].fire&=0x10;

	else if (k==keys[i][PLAYER_AIM_LEFT])
	 ks[i].aim_left&=0x10;

        else if (k==keys[i][PLAYER_AIM_RIGHT])
	 ks[i].aim_right&=0x10;

	else if (k==keys[i][PLAYER_WEAPON_LEFT])
	 ks[i].weapon_left&=0x10;

        else if (k==keys[i][PLAYER_WEAPON_RIGHT])
	 ks[i].weapon_right&=0x10;

	else if (k==keys[i][PLAYER_SHIELD])
	 bs[i].shield&=0x10;
      }
    }
  }
}

void clear_controls(objectstruct *o)
{ tnk.contr.up=0;
  tnk.contr.down=0;
  tnk.contr.left=0;
  tnk.contr.right=0;

  tnk.contr.fire=0;
  tnk.contr.shield=0;
}

long go_left_or_right(long wanted, long actual)
{ if (wanted==actual) return(0);

  return (((actual-wanted)&15)<8? -1:1);
}

void copy_controls_direct(objectstruct *o)
{ long i=tnk.contr.num, x, y;

  tnk.contr.down=0;
  tnk.contr.left=0;
  tnk.contr.right=0;

  tnk.contr.fire=bs[i].fire;
  tnk.contr.shield=bs[i].shield;

  if (ks[i].weapon_left!=0)
   weapon_left(o);

  if (ks[i].weapon_right!=0)
   weapon_right(o);

  if (key_or_joy[i]==CONTR_KEY)
  { y=ks[i].up!=0? -100:(ks[i].down!=0? 100:0);
    x=ks[i].right!=0? 100:(ks[i].left!=0? -100:0);
  }
  else
  { x=js[i].x;
    y=js[i].y;

    if (js[i].weapon!=0)
    { if (x<-sensi)
       weapon_left(o);

      if (x>sensi)
       weapon_right(o);

      return;
    }
  }

  if (ks[i].aim_left!=0 || ks[i].aim_right!=0 || js[i].aim!=0)
  { if (!in_radius(x, y, sensi))
     switch(go_left_or_right(radtoa16(angle(x, y)), tnk.motion.ldir))
     { case -1:
	aim_left(o);
        break;

       case 1:
	aim_right(o);
        break;
     }
  }

  else if (!in_radius(x, y, sensi))
  { switch(go_left_or_right(radtoa16(angle(x, y)), tnk.motion.dir))
    { case -1:
       tnk.contr.left=1;
       break;

      case 1:
       tnk.contr.right=1;
       break;
    }

    tnk.contr.up=1;
  }

  else tnk.contr.up=0;
}

void copy_controls_relative(objectstruct *o)
{ long i=tnk.contr.num;

  tnk.contr.left=0;
  tnk.contr.right=0;
  tnk.contr.up=0;
  tnk.contr.down=0;

  tnk.contr.fire=bs[i].fire;
  tnk.contr.shield=bs[i].shield;

  if (ks[i].aim_left!=0)
   aim_left(o);

  if (ks[i].aim_right!=0)
   aim_right(o);

  if (ks[i].weapon_left!=0)
   weapon_left(o);

  if (ks[i].weapon_right!=0)
   weapon_right(o);

  if (key_or_joy[i]==CONTR_KEY)
  { tnk.contr.up=ks[i].up;
    tnk.contr.down=ks[i].down;

    tnk.contr.left=ks[i].left;
    tnk.contr.right=ks[i].right;
  }
  else
  { if (js[i].y<-sensi) tnk.contr.up=1;
    if (js[i].y>sensi) tnk.contr.down=1;

    if (js[i].weapon!=0)
    { if (js[i].x<-sensi)
       weapon_left(o);

      if (js[i].x>sensi)
       weapon_right(o);
    }

    else if (js[i].aim!=0)
    { if (js[i].x<-sensi)
       aim_left(o);

      if (js[i].x>sensi)
       aim_right(o);
    }

    else
    { if (js[i].x<-sensi) tnk.contr.left=1;
      if (js[i].x>sensi) tnk.contr.right=1;
    }
  }
}
