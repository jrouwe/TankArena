/* Makes tanks fire */

#include <tank.h>
#include <math.h>

#define tnk o->d.tk

extern long unlimited_lock, unlimited_weapons;

static long drop[]={0,-20};

static char *desc[] = {"MAIN CANNON",
		       "CHAIN GUN",
		       "FLAME THROWER",
		       "MINES",
		       "ROCKET",
		       "MORTAR",
		       "A-BOMB",
		       "MEN WITH CG",
		       "MEN WITH FL",
		       "SMOKE SCREEN",
		       "INVISIBILITY",
		       "EXTRA SPEED",
		       "LIGHT"};

void weapon_left(objectstruct *o)
{
  long i, nw;

  if (!tdone(tnk.weap.nws) || tnk.lock.active)
   return;

  nw = map.night? num_weapon+1:num_weapon;

  for (i = 0; i < nw; i++)
  {
    tnk.weap.w--;

    if (tnk.weap.w < 0)
      tnk.weap.w = nw - 1;

    if (tnk.weap.nw[tnk.weap.w] != 0 ||
	(tnk.tank->weap.flame && tnk.weap.w == 2) ||
	tnk.weap.w == num_weapon)
      break;
  }

  create_message(o, desc[tnk.weap.w], "SELECTED!");

  tnk.weap.nws=tinit(15);
}

void weapon_right(objectstruct *o)
{
  long i, nw;

  if (!tdone(tnk.weap.nws) || tnk.lock.active)
   return;

  nw = map.night? num_weapon+1:num_weapon;

  for (i = 0; i < nw; i++)
  {
    tnk.weap.w++;

    if (tnk.weap.w >= nw)
      tnk.weap.w = 0;

    if (tnk.weap.nw[tnk.weap.w] != 0 ||
	(tnk.tank->weap.flame && tnk.weap.w == 2) ||
	tnk.weap.w == num_weapon)
      break;
  }

  create_message(o, desc[tnk.weap.w], "SELECTED!");

  tnk.weap.nws=tinit(15);
}

static void tank_fire_locked(objectstruct *o, long tx, long ty, long power)
{ long sx, sy;
  char dir;

  switch(tnk.weap.w)
  { case 4:
     create_sound_relative(o, SND_ROCKET);

     dir=tnk.tank->turret.main_static!=MAIN_TURRET? tnk.motion.dir:tnk.motion.ldir;
     sx=o->x+loc_x(tnk.motion.dir, tnk.tank->turret.loc)+loc_x(dir, tnk.tank->rocket);
     sy=o->y+loc_y(tnk.motion.dir, tnk.tank->turret.loc)+loc_y(dir, tnk.tank->rocket);
     create_rocket(o, sx, sy, tx, ty, power);
     break;

    case 7:
    case 8:
     dir=radtoa16(angle(tx-o->x, ty-o->y));
     sx=o->x+loc_x(dir, drop);
     sy=o->y+loc_y(dir, drop);
     make_man(o, sx, sy, tx, ty, tnk.weap.w==7? HT_CHAIN:HT_FLAME, tnk.weap.power[tnk.weap.w]);
     break;
  }
}

void tank_fire(objectstruct *o, objectstruct *target)
{ tankinfotype *tank=tnk.tank;
  long i, x, y, x2, y2, dir, ldir;

  if (!tdone(tnk.weap.nf) || o->h<1)
   return;

  if (tnk.und.nw==WATER && o->h<2)
   switch(tnk.tank->turret.main_static)
   { case MAIN_STATIC:
     case MAIN_ROTATE:
      switch(tnk.weap.w)
      { case 10:
	case 11:
	 break;

	default:
	 return;
      }
      break;

     case MAIN_TURRET:
      switch(tnk.weap.w)
      { case 0:
	case 2:
	case 4:
	case 5:
	case 10:
	case 11:
	 break;

	default:
	 return;
      }
      break;
   }

  if (tnk.weap.w == num_weapon)
    tnk.weap.nf = tinit(50);
  else if (!tnk.contr.computer)
    {
      if (tnk.weap.w!=2)
	{
	  if (tnk.weap.nw[tnk.weap.w]==0)
	    {
	      create_sound_relative(o, SND_EMPTY);
	      tnk.weap.nf=tinit(50);
	      return;
	    }
	  else
	    if (!unlimited_weapons)
	      if (tnk.weap.w==1)
		tnk.weap.nw[1]-=5;
	      else
		tnk.weap.nw[tnk.weap.w]--;

	}
      else
	if (!unlimited_weapons)
	  if (tnk.motion.fuel>40)
	    tnk.motion.fuel-=40;
	  else
	    return;

      tnk.weap.nf = tinit(tnk.tank->weap.speed[tnk.weap.w]);
    }
  else
    tnk.weap.nf = tinit(tnk.computer.fire_delay);

  x=o->x, y=o->y, dir=tnk.motion.dir;
  ldir=tank->turret.main_static!=MAIN_TURRET? dir:tnk.motion.ldir;

  switch(tnk.weap.w)
  { case 0:
    case 5:
     x2=o->x+loc_x(ldir, &tnk.tank->main_gun[1+2*tnk.weap.activegun])+loc_x(dir, tnk.tank->turret.loc)+16;
     y2=o->y+loc_y(ldir, &tnk.tank->main_gun[1+2*tnk.weap.activegun])+loc_y(dir, tnk.tank->turret.loc)+16;

     switch(tnk.weap.w)
     { case 0:
	create_bullet(o, tank->weap.main, x2, y2, ldir, tnk.weap.power[tnk.weap.w], game.mode==DUAL? 500:250, tank->weap.main==HT_SROCK? 30:0);
	break;

       case 5:
	if (tnk.tank->type!=PLANE)
	{ tank_add_impulse(o, a16torad(ldir)+PI, 50);
	  create_bullet(o, HT_MORTAR, x2, y2, ldir, tnk.weap.power[tnk.weap.w], 0, 0);
	} else create_bullet(o, HT_PLANEBOMB, x2, y2, ldir, tnk.weap.power[tnk.weap.w], 0, 0);

	break;
     }

     tnk.weap.activegun++;

     if (tnk.weap.activegun==tnk.tank->main_gun[0])
      tnk.weap.activegun=0;
     break;

    case 1:
     i=tnk.tank->chain_gun[0]-1;
     if (i<3 && tnk.weap.extragun) i++;

     for (; i>=0; i--)
     { x2=o->x+loc_x(dir, &tnk.tank->chain_gun[1+i*2])+16;
       y2=o->y+loc_y(dir, &tnk.tank->chain_gun[1+i*2])+16;

       create_bullet(o, HT_CHAIN, x2, y2, dir, tnk.weap.power[tnk.weap.w], 5*b_size, 0);
     }

     tnk.weap.chf=1;
     break;

    case 2:
     x2=o->x+loc_x(ldir, &tnk.tank->main_gun[1+2*tnk.weap.activegun])+loc_x(dir, tnk.tank->turret.loc)+16;
     y2=o->y+loc_y(ldir, &tnk.tank->main_gun[1+2*tnk.weap.activegun])+loc_y(dir, tnk.tank->turret.loc)+16;

     create_bullet(o, HT_FLAME, x2, y2, ldir, tnk.weap.power[tnk.weap.w], 0, 0);

     tnk.weap.activegun++;

     if (tnk.weap.activegun==tnk.tank->main_gun[0])
      tnk.weap.activegun=0;
     break;

    case 3:
     create_mine(x, y, LIGHT_MINE, tnk.weap.power[tnk.weap.w]);
     break;

    case 4:
    case 7:
    case 8:
     if (!tnk.contr.computer)
     { tnk.lock.active=TRUE;
       tnk.lock.vx=0;
       tnk.lock.vy=0;
       tnk.lock.fired=tnever;
       tnk.lock.x=o->x;
       tnk.lock.y=o->y;
     }
     else
     { if (o->target==NULL)
	tank_fire_locked(o, tnk.computer.target_x, tnk.computer.target_y, tnk.weap.power[tnk.weap.w]);
	else tank_fire_locked(o, o->target->x, o->target->y, tnk.weap.power[tnk.weap.w]);
     }
     break;

    case 6:
     create_abomb(o, x, y, 3);
     break;

    case 9:
     create_sound_relative(o, SND_SMOKESCR);

     tnk.und.smokes+=100;
     break;

    case 10:
     create_sound_relative(o, SND_INVISIBLE);

     tnk.invis.count=2;
     tnk.invis.ncount=tinit(2000);
     break;

    case 11:
     create_sound_relative(o, SND_SPEEDUP);

     tnk.motion.boost=tinit(1000);
     break;

    case 12:
     create_sound_relative(o, SND_LIGHT);

     tnk.last_min.light_on = !tnk.last_min.light_on;
     break;
  }
}

void tank_marker_control(objectstruct *o)
{ long lx=tnk.lock.x, ly=tnk.lock.y, vlx=tnk.lock.vx, vly=tnk.lock.vy;

  tnk.lock.fired=tinit(50);

  if (tnk.contr.fire==0)
  { tnk.lock.active=FALSE;
    tank_fire_locked(o, lx, ly, tnk.weap.power[tnk.weap.w]);
  }
  else if (tdone(tnk.weap.nm))
  { if (tnk.contr.up!=0)
    { if (vly>=0) vly=-5; else if (vly>-20) vly--;
    } else
    if (tnk.contr.down!=0)
    { if (vly<=0) vly=5; else if (vly<20) vly++;
    } else vly=0;

    if (tnk.contr.left!=0)
    { if (vlx>=0) vlx=-5; else if (vlx>-20) vlx--;
    } else
    if (tnk.contr.right!=0)
    { if (vlx<=0) vlx=5; else if (vlx<20) vlx++;
    } else vlx=0;

    lx+=vlx, ly+=vly;

    if (lx<0) lx=0;
    if (ly<0) ly=0;
    if (lx>(map.sx-1)*b_size) lx=(map.sx-1)*b_size;
    if (ly>(map.sy-1)*b_size) ly=(map.sy-1)*b_size;

    if (game.mode==DUAL || in_radius(lx-o->x, ly-o->y, 450) || unlimited_lock)
     tnk.lock.x=lx, tnk.lock.y=ly;
     else vlx=0, vly=0;

    tnk.lock.vx=vlx, tnk.lock.vy=vly;

    tnk.weap.nm=tinit(3);
  }
}
