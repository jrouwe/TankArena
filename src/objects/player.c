/* Object: player. This object creates the tanks/racecars in dual
   player mode. It also detects when somebody wins.
*/

#include <tank.h>

extern unsigned char *pstart[6][4];
extern long pl_tanks[2], lose, race_laps;

#define ply o->d.pl
#define tnk q->d.tk

static void mwrite_player(objectstruct *o, objectstruct *ao)
{ b_add_shadow(o->x-ssx+o->h, o->y-ssy+o->h*2, pstart[ply.type][ply.dir]);
  b_add_layer(o->x-ssx, o->y-ssy, pstart[ply.type][ply.dir]);
}

static void control_race(objectstruct *o)
{ objectstruct *q=o->target;
  long i, c=ply.type&1;
  char str[30];

  if (q==NULL || tdone(tnk.motion.dead))
  { if (ply.last_chk==-1) q=make_tank(o, 6, o->x, o->y, ply.dir*4, c);
     else q=make_tank(o, 6, ply.chk[ply.last_chk].x, ply.chk[ply.last_chk].y, ply.chk[ply.last_chk].dir, c);

    o->target=q;

    if (o->d.pl.weap.active)
     for (i=0; i<num_weapon; i++)
      q->d.tk.weap.nw[i]=o->d.pl.weap.nw[i];

    control_tank(o->target);

    game.tk[tnk.contr.num]=o->target;
  }
  else
  { for (i=0; i<ply.num_chk; i++)
     if (!ply.chk[i].passed && in_radius(q->x-ply.chk[i].x, q->y-ply.chk[i].y, 70))
     { ply.last_chk=i, ply.chk[i].passed=TRUE;
       create_sound_relative(q, SND_BEEP);
     }

    if (tnk.und.nw==START)
    { for (i=0; i<ply.num_chk && ply.chk[i].passed; i++);

      if (i==ply.num_chk)
      { for (i=0; i<ply.num_chk; i++)
	 ply.chk[i].passed=FALSE;

	ply.last_chk=-1;
	ply.laps--;

	if (ply.laps==0 && game.winner==-1)
	{ game.winner=c;
	  game.end_round=tinit(300);
	}
	else if (ply.laps>0)
	{ sprintf(str, "%ld MORE LAPS", ply.laps);
	  create_message(q, ply.laps>1? str:"FINAL LAP!", "");
	}
      }
    }
  }
}

static void control_player(objectstruct *o)
{ objectstruct *q=o->target;
  long x, y, c=ply.type&1, i;

  if (q==NULL || tnk.shield.armor==0)
  { if (ply.lives>1 && (q==NULL || tdone(tnk.motion.dead)))
    { if (!game.flag_hunt) ply.lives--;

      q=make_tank(o, pl_tanks[c], o->x, o->y, ply.dir*4, c);

      o->target=q;

      if (o->d.pl.weap.active)
       for (i=0; i<num_weapon; i++)
	q->d.tk.weap.nw[i]=o->d.pl.weap.nw[i];

      control_tank(o->target);

      game.tk[tnk.contr.num]=o->target;
    }

    else if (ply.lives==1 && game.winner==-1)
    { if (!game.flag_hunt) ply.lives--;

      game.winner=c^1;
      game.end_round=tinit(300);
    }
  }
  else
  { x=(o->x+16)/b_size, y=(o->y+16)/b_size;

    if (!game.flag_hunt && mapm.m[1][x+map.sx*y]!=-1 && strength[0][0][x+map.sx*y]==0)
    { ply.lives=1;
      tnk.und.in_base=FALSE;
    }
    else tnk.und.in_base=tnk.shield.armor!=0 && in_radius(o->x-q->x, o->y-q->y, 7);
  }
}

objectstruct *make_player(long x, long y, long type, long dir)
{ objectstruct *o;

  o=create_object(PLAYER_TYPE, x, y, (type==2 || type==3)? 7:1, 0);

  o->funct.control=control_player;
  o->funct.mwrite=mwrite_player;

  ply.type=type;
  ply.dir=dir;
  ply.lives=4;
  ply.last_chk=-1;

  return(o);
}

static void prepare_track(objectstruct *o)
{ long x, y, i;

  for (i=0, y=0; y<map.sy && i<25; y++)
   for (x=0; x<map.sx && i<25; x++)
    if (mapm.m[0][x+map.sx*y]!=-1)
     switch(pic[mapm.m[0][x+map.sx*y]].type-(CHECK_P1_UP+(ply.type&1)*4))
     { case 0:
	ply.chk[i].dir=0;
	goto set_point;

       case 1:
	ply.chk[i].dir=4;
	goto set_point;

       case 2:
	ply.chk[i].dir=8;
	goto set_point;

       case 3:
	ply.chk[i].dir=12;

       set_point:;
	ply.chk[i].x=x*b_size;
	ply.chk[i].y=y*b_size;
	ply.chk[i].passed=FALSE;
	i++;

       break;
     }

  ply.laps=race_laps;
  ply.num_chk=i;
}

void prepare_player(objectstruct *o)
{ long c=ply.type&1;

  o->funct.mwrite=mwrite_player;

  switch(game.mode)
  { case DUAL:
     switch(ply.type)
     { case 0:
       case 1:
	if (tank[pl_tanks[c]].type!=TANK)
	  o->funct.control=remove_object;
	else
	  {
	    o->funct.control=control_player;
	    control_player(o);
	  }
	break;

       case 2:
       case 3:
	if (tank[pl_tanks[c]].type!=CHOPPER)
	  o->funct.control=remove_object;
	else
	  {
	    o->funct.control=control_player;
	    control_player(o);
	  }
	break;

       case 4:
       case 5:
	if (tank[pl_tanks[c]].type!=PLANE)
	  o->funct.control=remove_object;
	else
	  {
	    o->funct.control=control_player;
	    control_player(o);
	  }
	break;

       case 6:
       case 7:
	prepare_track(o);

	o->funct.control=control_race;
	control_race(o);
	break;
     }
     break;

    case SINGLE:
    case DUALVC:
     o->funct.control = remove_object;
     break;

    case MAP:
     break;
  }
}
