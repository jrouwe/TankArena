/* General routines to write/control the tank. */

#include <tank.h>
#include <math.h>

#define tnk o->d.tk

extern long invunerable;

static void write_tank(objectstruct *o)
{ long x=o->x-ssx, y=o->y-ssy, h=o->h, x2=x+h, y2=y+2*h, i;
  long dir=tnk.motion.dir+tnk.motion.select;
  long ldir=tnk.motion.ldir+32;

  if ((tdone(tnk.contr.disable) && h==1) || h<1)
  { if (tnk.und.nw!=WATER && tnk.und.nw!=LAVA_PUT)
     b_add_layer(x, y, tnk.tank->pic[49-h]);
  }

  else if ((tnk.invis.count==0 || o==game.tk[game.tn]) && tnk.invis.flash==1)
  { if (h>1)
    { b_add_shadow(x2, y2, tnk.tank->pic[dir]);

      if (tnk.tank->type==CHOPPER)
       b_add_shadow(x2, y2, tnk.tank->pic[ldir]);
    }

    if (h>1 || (tnk.und.nw!=WATER && tnk.und.nw!=LAVA_PUT))
     b_add_layer(x, y, tnk.tank->pic[dir]);

    if (((tnk.shield.power!=0 && tnk.contr.shield!=0) || (tnk.shield.icount&1)==1) && game.mode!=MAP)
     b_add_layer(x, y, intern[ARMOR].ptr);

    if (tnk.weap.chf==1)
    { i=tnk.tank->chain_gun[0]-1;
      if (i<3 && tnk.weap.extragun) i++;

      for (; i>=0; i--)
       b_add_layer(x+loc_x(tnk.motion.dir, &tnk.tank->chain_gun[1+i*2]), y+loc_y(tnk.motion.dir, &tnk.tank->chain_gun[1+i*2]), eff[SFIRE].ptr);
    }

    if (h>0)
     b_add_layer(x+loc_x(tnk.motion.dir, tnk.tank->turret.loc), y+loc_y(tnk.motion.dir, tnk.tank->turret.loc), tnk.tank->pic[ldir]);
  }
}

static void mwrite_tank(objectstruct *o, objectstruct *ao)
{ long r, i, j;
  float a;

  if (o==ao && tnk.contr.computer)
  { draw_circle(o->x, o->y, tnk.computer.activation_rad, 15);

    r=tnk.computer.attack_at;
    a=a16torad(tnk.motion.dir);
    draw_circle(o->x, o->y, r, 95);
    draw_line(o->x, o->y, o->x+(float)r*cos(a-0.2), o->y-(float)r*sin(a-0.2), 95);
    draw_line(o->x, o->y, o->x+(float)r*cos(a+0.2), o->y-(float)r*sin(a+0.2), 95);

    if (tnk.way.type!=WAY_INACTIVE)
    { for (i=0; i<10; i++)
       if (tnk.way.point[i].x!=-1)
       { b_write_text(tnk.way.point[i].x+13-ssx, tnk.way.point[i].y+13-ssy, 15, "%d", i);

         for (j=i+1; j<10; j++)
	  if (tnk.way.point[j].x!=-1)
	  { draw_line(tnk.way.point[i].x, tnk.way.point[i].y, tnk.way.point[j].x, tnk.way.point[j].y, 15);
            break;
          }
       }

      if (tnk.way.type==WAY_CIRCULAR)
       for (i=0; i<10; i++)
	if (tnk.way.point[i].x!=-1)
         for (j=9; j>i; j--)
	  if (tnk.way.point[j].x!=-1)
	  { draw_line(tnk.way.point[i].x, tnk.way.point[i].y, tnk.way.point[j].x, tnk.way.point[j].y, 15);
            goto end_loop;
          }

      end_loop:;
    }
  }
}

static void light_tank(objectstruct *o)
{ long i, x=o->x, y=o->y, d1, d2, w=tnk.weap.w;
  long *t=tnk.tank->light;

  if (tnk.invis.count != 0 || !tnk.last_min.light_on)
    return;

  if (tnk.tank->turret.main_static==MAIN_TURRET)
  { x+=loc_x(tnk.motion.dir, tnk.tank->turret.loc);
    y+=loc_y(tnk.motion.dir, tnk.tank->turret.loc);
    d1=tnk.motion.ldir;

  } else d1=tnk.motion.dir;

  d2=(w==0 || w==2 || w==5)? d1:tnk.motion.dir;

  for (i=0; i<t[0]; i++)
   write_bundle(t[3*i+1], x+loc_x(d1, &t[3*i+2]), y+loc_y(d1, &t[3*i+2]), d2);
}

static void hit_tank(objectstruct *o, long power, long type)
{ long sx=(o->x+16)/b_size, sy=(o->y+16)/b_size, other_tanks=FALSE;
  objectstruct *p;
  char str[20];
  int i;

  if (tnk.shield.armor==0 || (invunerable && tnk.contr.good && type!=HT_DESTROY))
   return;

  if (type==HT_ABOMB && get_type(sx, sy, 1)==SHELTER)
   return;

  if (type!=HT_ROCKET && type!=HT_DESTROY && tnk.contr.shield!=0 && tnk.shield.power!=0)
   return;

  if (type!=HT_DESTROY && tnk.shield.icount!=0 && o->h>=1)
   return;

  if (game.mode!=DUAL && tnk.contr.id>=0)
   for (i=0; i<2; i++)
    if (game.tk[i]!=NULL && game.tk[i]!=o)
    { sprintf(str, "TANK %ld HAS", tnk.contr.id);
      create_message(game.tk[i], str, "BEEN HIT!");
    }

  tnk.shield.armor-=power;

  if (tnk.shield.armor<=0)
  { tnk.shield.armor=0;

    tnk.contr.disable=tinit(100);
    tnk.motion.dead=tinit(300);

    /* check if game ends by losing this tank */

    for (p=obj; p!=NULL; p=p->next)
     if (p->type==TANK_TYPE)
     { if (p->d.tk.contr.num!=tnk.contr.num)
	p->d.tk.weap.reload=TRUE;

       if (!p->d.tk.contr.computer && p->d.tk.shield.armor>0)
	other_tanks=TRUE;
     }

    modify_goal_counter(other_tanks? o->gc:-100);
    o->gc=0;

    /* do nuke for atomic tank */

    if (tnk.tank == &tank[21])
      create_abomb(o, o->x, o->y, 0);
  }
}

objectstruct *make_tank(objectstruct *owner, long n, long x, long y, long dir, long control)
{ objectstruct *o;
  long i;

  o=create_object(TANK_TYPE, x, y, tank[n].type==PLANE? 1:tank[n].props.h, tank[n].props.r);

  o->funct.control=control_tank;
  o->funct.write=write_tank;
  o->funct.hit=hit_tank;
  o->funct.light=light_tank;
  o->funct.mwrite=mwrite_tank;

  o->owner=owner;

  tnk.tank_num=n;
  tnk.tank=&tank[n];

  tnk.motion.dead=tnever;
  tnk.motion.vx=0;
  tnk.motion.vy=0;
  tnk.motion.vz=0;
  tnk.motion.dir=dir;
  tnk.motion.ldir=dir;
  tnk.motion.acount=timer;
  tnk.motion.ndir=tnow;
  tnk.motion.nldir=tnow;
  tnk.motion.fuel=tank[n].props.max_fuel;
  tnk.motion.nfuel=tnow;
  tnk.motion.overlap=b_size*b_size;
  tnk.motion.boost=tnow;
  tnk.motion.select=0;
  tnk.motion.ns=0;
  tnk.motion.rotate=tnow;
  tnk.motion.nhit=tnow;
  tnk.motion.smoke=tnow;
  tnk.motion.nh=tnow;

  tnk.computer.attack_at=tank[n].type==CAR? 0:150;
  tnk.computer.attack_tanks=tank[n].type==CAR? 0:100;
  tnk.computer.attack_goals=60;
  tnk.computer.fire_delay=tank[n].weap.speed[tank[n].weap.initial_weap];
  tnk.computer.wave_delay=tank[n].computer.wave_delay;
  tnk.computer.activation_rad=250;
  tnk.computer.activator=0;
  tnk.computer.stay_active=tank[n].type==PLANE;
  tnk.computer.active=FALSE;
  tnk.computer.crashed=tnow;
  tnk.computer.crashed2=tnow;
  tnk.computer.wave=tnow;
  tnk.computer.target_x=-1;
  tnk.computer.target_y=-1;
  tnk.computer.new_target=tnow;
  tnk.computer.last_dir_active=tnow;
  tnk.computer.last_dir=0;

  tnk.way.type=WAY_INACTIVE;
  tnk.way.current=0;
  for (i=0; i<10; i++)
   tnk.way.point[i].x=-1;

  tnk.contr.id=control==-1? -1:1;
  tnk.contr.num=control;
  tnk.contr.computer=control==-1;
  tnk.contr.good=control!=-1 || tank[n].type==CAR;
  tnk.contr.disable=tnever;

  tnk.shield.power=3000;
  tnk.shield.armor=tank[n].props.armor;
  tnk.shield.ruin=tnow;
  tnk.shield.icount=control==-1? 0:15;
  tnk.shield.pcount=timer;
  tnk.shield.invun=tinit(200);

  tnk.invis.count=0;
  tnk.invis.ncount=tnow;
  tnk.invis.flash=1;
  tnk.invis.nflash=tnow;

  tnk.weap.activegun=0;
  tnk.weap.extragun=0;
  tnk.weap.nf=tnow;
  tnk.weap.w=tank[n].weap.initial_weap;
  tnk.weap.nws=tnow;
  tnk.weap.chf=0;
  tnk.weap.reload=FALSE;
  tnk.weap.nm=tnow;

  tnk.lock.active=FALSE;
  tnk.lock.fired=tnow;

  for (i=0; i<num_weapon; i++)
  { tnk.weap.nw[i]=tnk.tank->weap.num[i];
    tnk.weap.power[i]=tnk.tank->weap.power[i];
  }

  tnk.und.old=NOTHING;
  tnk.und.nw=NOTHING;
  tnk.und.neffect=tnow;
  tnk.und.smokes=0;
  tnk.und.nsmoke=tnow;
  tnk.und.in_base=FALSE;
  tnk.und.flags=0;

  tnk.msg.count=0;

  tnk.last_min.lava_hit=tnow;
  tnk.last_min.light_on=TRUE;

  return(o);
}

void prepare_tank(objectstruct *o)
{ long i;

  if (game.mode==DUAL && !tnk.contr.computer)
  { remove_object(o);
    return;
  }

  o->funct.control=control_tank;
  o->funct.write=write_tank;
  o->funct.hit=hit_tank;
  o->funct.light=light_tank;
  o->funct.mwrite=mwrite_tank;

  tnk.tank=&tank[tnk.tank_num];
  tnk.motion.acount=timer;

  tnk.last_min.lava_hit=tnow;				// last minute
  tnk.last_min.light_on=TRUE;

  tnk.shield.pcount=timer;				// setup shields
  tnk.shield.invun=tinit(200);
  if (game.supermarket_mayhem) tnk.shield.icount=0;
  
  for (i=0; i<10 && tnk.way.point[i].x==-1; i++);	// search first waypoint
  if (i==10) tnk.way.type=WAY_INACTIVE;
   else tnk.way.current=i;

  tnk.weap.w=tnk.tank->weap.initial_weap;		// first active weapon
  if (tnk.weap.w!=2 && tnk.weap.nw[tnk.weap.w]==0)
  { for (i=0; i<num_weapon && tnk.weap.nw[i]==0; i++);
    tnk.weap.w=i==num_weapon? (tnk.tank->weap.flame? 2:0):i;
  }

  switch (game.mode)
  { case DUAL:
     tnk.contr.num=-1;
     break;

    case SINGLE:
     if (game.id[0]==tnk.contr.id) game.tk[0]=o;
     tnk.contr.num=1;
     break;

    case DUALVC:
     for (i=0; i<2; i++)
      if (game.id[i]==tnk.contr.id)
      { game.tk[i]=o;
	tnk.contr.num=i;
	break;
      }
     break;

    case MAP:
     break;
  }
}
