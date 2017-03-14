/* More functions to control a tank */

#include <tank.h>
#include <math.h>

#define WRECK_LIFE_TIME 3000

#define tnk o->d.tk

extern long invunerable;

static long sscreen[]={-12,11, 12,11};

static void hit_wreck(objectstruct *o, long power, long type)
{ long i;

  if (game.winner==(game.mode!=DUAL? 0:-1))
  { for (i=0; i<10; i++)
     create_effect(o->x-8+rnd(16), o->y-8+rnd(16), 1, EXPLOSION);

    create_sound_absolute(o->x, o->y, SND_EXPLODE);

    for (i=0; i<2; i++)
     if (game.tk[i]==o)
      game.tk[i]=NULL;

    remove_object(o);
  }
}

static void control_wreck(objectstruct *o)
{ if (tdone(tnk.shield.ruin))
  { create_effect(o->x-4+rnd(8), o->y-4+rnd(8), 2, ROOK);
    tnk.shield.ruin=tinit(500+rnd(300));
  }

  if (tdone(tnk.last_min.destroy_wreck))
    hit_wreck(o, 0, 0);
}

static void new_velocity(objectstruct *o)
{ long x=(o->x+16)/b_size, y=(o->y+16)/b_size, lowest, highest, c;
  float a, vx, vy, vz, dv, dt;
  objectstruct *p;

  dt=(float)(timer-tnk.motion.acount)/sec;
  tnk.motion.acount=timer;

  a=a16torad(tnk.motion.dir);

  vx=tnk.motion.vx*cos(a)-tnk.motion.vy*sin(a);
  vy=tnk.motion.vx*sin(a)+tnk.motion.vy*cos(a);
  vz=tnk.motion.vz;

  if ((tnk.contr.up!=0 || tnk.contr.down!=0) &&
      tnk.motion.fuel>0 && !tnk.lock.active &&
      ((tnk.tank->type!=TANK && tnk.tank->type!=CAR) || o->h==1))
  { dv=dt*tnk.tank->motion.acc/tnk.tank->motion.mass;
    
    if (!tdone(tnk.motion.boost)) dv*=1.5;

    if (check(x, y) && o->h<=1)
    { if (mapm.m[0][x+map.sx*y]!=-1)
       dv*=pic[mapm.m[0][x+map.sx*y]].speed;
      if (mapm.m[1][x+map.sx*y]!=-1 && strength[0][0][x+map.sx*y]==0)
       dv*=pic[mapm.m[1][x+map.sx*y]].speed;
    }

    vx+=tnk.contr.up!=0 || tnk.tank->type==PLANE? dv:-dv;
  }
  else if (tnk.tank->type==PLANE)
  { if (o->h>1 && vz<0 && vx>0)
    { vx-=20*vz*dt;
      if (vx<0) vx=0;
    }
  }

  dv=tnk.tank->motion.ff_v*fabs(vx);
  if (tnk.tank->type==CHOPPER || o->h==1)
   dv+=tnk.tank->motion.ff_0;
  dv*=dt/tnk.tank->motion.mass;
  vx=vx>=0? (vx-dv<0? 0:vx-dv):(vx+dv>0? 0:vx+dv);

  if ((tnk.tank->type==PLANE || tnk.tank->type==CHOPPER) || o->h==1)
   dv=tnk.tank->motion.fs_v*fabs(vy)+tnk.tank->motion.fs_0;
   else dv=tnk.tank->motion.ff_v*fabs(vy);
  dv*=dt/tnk.tank->motion.mass;
  vy=vy>=0? (vy-dv<0? 0:vy-dv):(vy+dv>0? 0:vy+dv);
  
  tnk.motion.vx=vx*cos(-a)-vy*sin(-a);
  tnk.motion.vy=vx*sin(-a)+vy*cos(-a);
  tnk.motion.v=sqrt(sqr(tnk.motion.vx)+sqr(tnk.motion.vy));
  
  lowest=in_pit(o->x, o->y)? -4:1;
  highest=tnk.tank->props.h>1? tnk.tank->props.h:24;

  o->fh+=vz*dt;
  if (o->fh<lowest) o->fh=lowest, vz=0;
  if (o->fh>highest) o->fh=highest, vz=0;

  if (round(o->fh)!=o->h)
  { o->h=round(o->fh);
    resort_object(o);
  }

  if (o->h>lowest)
  { if (tnk.tank->type==PLANE)
     vz-=dt*10;
     else vz-=dt*100;
  }

  if (tnk.shield.armor>0 && tnk.motion.fuel>0)
   switch (tnk.tank->type)
   { case PLANE:
      if (tnk.motion.v>5)
      { vz+=tnk.tank->motion.lift*vx*dt;

	/* planes can only land on runways */

	if (o->h<=1 && tnk.und.nw!=RUNWAY)
	 hit_obj_for(o, o->x+16, o->y+16, HT_DESTROY, 1000);
      }
      break;

     case CHOPPER:
      vz=0;
      break;

     default:
      break;
   }

  tnk.motion.vz=vz;

  if (o->h==-4 && tnk.shield.armor>0)
   hit_obj_for(o, o->x+16, o->y+16, HT_DESTROY, 1000);

  if (tnk.shield.armor==0)
  { o->funct.light=NULL;

    if (o->h==lowest && tdone(tnk.contr.disable))
    { o->funct.control=control_wreck, o->funct.hit=hit_wreck;

      /* Wreck will be destroyed after a while */

      tnk.last_min.destroy_wreck = tinit(WRECK_LIFE_TIME);

      /* Search for other tanks */

      c=game.mode==SINGLE? 0:tnk.contr.num;

      if (game.mode!=DUAL && !tnk.contr.computer && tnk.contr.id==game.id[c])
      {
	/* search for tank with same id */

	for (p=obj; p!=NULL; p=p->next)
	 if (p->type==TANK_TYPE && !p->d.tk.contr.computer &&
	     p->d.tk.shield.armor>0 && p->d.tk.contr.id==game.id[c])
	 { game.tk[c]=p;
	   return;
	 }

	 /* search for tanks with other id's */

	 reset_control(tnk.contr.num);

	 for (p=obj; p!=NULL; p=p->next)
	  if (p->type==TANK_TYPE && !p->d.tk.contr.computer &&
	      p->d.tk.shield.armor>0 && p->d.tk.contr.id!=game.id[c^1])
	  { game.id[c]=p->d.tk.contr.id, game.tk[c]=p;
	    p->d.tk.contr.num=tnk.contr.num;
	  }
      }
    }
    else create_effect(o->x-8+rnd(16), o->y-8+rnd(16), 8, EXPLOSION);
  }
}

void tanks_collide(objectstruct *o1, objectstruct *o2)
{ tankstruct *t1=&o1->d.tk, *t2=&o2->d.tk;
  float vx=t1->motion.vx-t2->motion.vx;
  float vy=t1->motion.vy-t2->motion.vy;
  float loss=t1->tank->motion.crash;
  float mass=t1->tank->motion.mass/t2->tank->motion.mass;

  if (!tdone(t2->motion.dead))
  { t1->motion.vx-=vx*(1-loss)/mass;
    t1->motion.vy-=vy*(1-loss)/mass;
    t2->motion.vx+=vx*loss*mass;
    t2->motion.vy+=vy*loss*mass;

    t1->motion.v=sqrt(sqr(t1->motion.vx)+sqr(t1->motion.vy));
    t2->motion.v=sqrt(sqr(t2->motion.vx)+sqr(t2->motion.vy));
  }
  else
  { t1->motion.vx=0;
    t1->motion.vy=0;
    t1->motion.v=0;
  }
}

void tank_add_impulse(objectstruct *o, float a, long power)
{ float impulse;

  impulse=(float)2000*power/tnk.tank->motion.mass;
  tnk.motion.vx+=cos(a)*impulse;
  tnk.motion.vy-=sin(a)*impulse;
  tnk.motion.v=sqrt(sqr(tnk.motion.vx)+sqr(tnk.motion.vy));
}

void static_collide(objectstruct *o)
{ tnk.motion.vx*=-tnk.tank->motion.crash;
  tnk.motion.vy*=-tnk.tank->motion.crash;
  tnk.motion.v*=tnk.tank->motion.crash;

  if (tnk.motion.v>10)
   create_sound_relative(o, SND_CRASH);
}

static void new_position(objectstruct *o)
{ long bx, by, ov;
  float x, y, f, vx, a;

  x=o->fx, y=o->fy;

  f=add_vector(&x, &y, tnk.motion.vx, tnk.motion.vy, &tnk.motion.vcount);

  if (!tnk.contr.computer && !invunerable)
   tnk.motion.fuel-=f;

  tnk.motion.ns-=f;

  if (tnk.motion.ns<0 || o->h>1)
  { tnk.motion.select^=16;
    tnk.motion.ns=tnk.tank->motion.change;
  }

  if (tnk.motion.fuel<=0)
  { tnk.motion.fuel=0;

    if (o->h>1)
     hit_obj_for(o, o->x+16, o->y+16, HT_DESTROY, 1000);
  }

  if (x<-15) x+=map.sx*b_size;
  if (x>map.sx*b_size-15) x-=map.sx*b_size;
  if (y<-15) y+=map.sy*b_size;
  if (y>map.sy*b_size-15) y-=map.sy*b_size;

  if (o->h<1 && !in_pit(pround(x), pround(y)))
  { static_collide(o);
    return;
  }

  /* if number of overlapping pixels is greater than the previous number
     then this is an invalid position */

  ov=overlap(o, tnk.tank->pic[tnk.motion.dir], pround(x), pround(y), &bx, &by);

  if (ov>tnk.motion.overlap)
  { static_collide(o);

    if (tnk.tank->type==PLANE)
     hit_obj_for(o, o->x+16, o->y+16, HT_DESTROY, 1000);

    if ((tnk.computer.target_x != -1 || tnk.computer.target_y != -1)
	&& (bx != (tnk.computer.target_x+16)/b_size || by != (tnk.computer.target_y+16)/b_size))
    { tnk.computer.crashed=tinit(4*tnk.tank->motion.rotation);
      tnk.computer.crashed2=tinit(200);
    }

    if (tnk.tank->general.crash>0 && tdone(tnk.motion.nhit) && strength[0][1][bx+map.sx*by]!=0 && o->h<2)
    { hit_wall_for(o, bx*b_size, by*b_size, 0, HT_CRASH, abs(tnk.motion.v)/25+tnk.tank->general.crash);
      create_effect((bx*b_size+o->x)/2+rnd(8)-4, (by*b_size+o->y)/2+rnd(8)-4, 3, RUIN);
      tnk.motion.nhit=tinit(10);
    }

    return;

  } else tnk.motion.overlap=ov;

  if (check_obj_collide(o, pround(x), pround(y)))
   return;

  if (o->h==1 && (tnk.tank->type==TANK || tnk.tank->type==CAR))
  { a=a16torad(tnk.motion.dir);
    vx=tnk.motion.vx*cos(a)-tnk.motion.vy*sin(a);

    if (on_big_ramp(o->x, o->y, pround(x), pround(y)))
     tnk.motion.vz+=fabs(vx)/5;

    if (on_small_ramp(o->x, o->y, pround(x), pround(y)))
     tnk.motion.vz+=fabs(vx)/10;
  }

  o->fx=x, o->fy=y, o->x=pround(x), o->y=pround(y);
}

static void left_right(objectstruct *o)
{ char dir=tnk.contr.left!=0? -1:1;
  long bx, by;

  if ((tnk.contr.left!=0 || tnk.contr.right!=0) && tdone(tnk.motion.ndir))
  { tnk.motion.dir=(tnk.motion.dir+dir)&15;
    tnk.motion.overlap=overlap(o, tnk.tank->pic[tnk.motion.dir], o->x, o->y, &bx, &by);
    tnk.motion.ndir=tinit(tnk.tank->motion.rotation);
  }
}

void aim_left(objectstruct *o)
{ if (tnk.tank->turret.main_static==MAIN_TURRET && tdone(tnk.motion.nldir))
  { tnk.motion.ldir=(tnk.motion.ldir-1)&15;
    tnk.motion.nldir=tinit(tnk.tank->motion.rotation);
  }
}

void aim_right(objectstruct *o)
{ if (tnk.tank->turret.main_static==MAIN_TURRET && tdone(tnk.motion.nldir))
  { tnk.motion.ldir=(tnk.motion.ldir+1)&15;
    tnk.motion.nldir=tinit(tnk.tank->motion.rotation);
  }
}

static void msg_control(objectstruct *o)
{ if (tnk.msg.count!=0 && tdone(tnk.msg.end))
  { tnk.msg.count--;
    tnk.msg.end=tinit(10);
  }
}

static void get_underground(objectstruct *o)
{ long x=(o->x+16)/b_size, y=(o->y+16)/b_size;

  tnk.und.old=tnk.und.nw;

  tnk.und.nw=get_type(x, y, 0);

  if (tnk.und.nw==-1)
   tnk.und.nw=get_type(x, y, 1);
}

static void underground_effects(objectstruct *o)
{ long i, x=(o->x+16)/b_size, y=(o->y+16)/b_size;

  if (o->h<2 && ((tnk.und.old==WATER && tnk.und.nw!=WATER) || (tnk.und.nw==WATER && tnk.und.old!=WATER)))
  { for (i=0; i<5; i++)
     create_effect(o->x+rnd(16)-8, o->y+rnd(16)-8, 2, SPLASH);

    create_sound_relative(o, SND_SPLASH);
  }

  if (o->h<2 && ((tnk.und.old==LAVA_PUT && tnk.und.nw!=LAVA_PUT) || (tnk.und.nw==LAVA_PUT && tnk.und.old!=LAVA_PUT)))
  { for (i=0; i<5; i++)
     create_effect(o->x+rnd(16)-8, o->y+rnd(16)-8, 2, LAVA_EFFECT);
    
    //create_sound_relative(o, SND_LAVA);
  }

  if (o->h<2 && tnk.und.nw==LAVA_PUT && tdone(tnk.last_min.lava_hit))
  { hit_obj_for(o, o->x+12+rnd(8), o->y+12+rnd(8), HT_LAVA, 2);
    tnk.last_min.lava_hit=tinit(10);
  }

  else if ((o->h>1 || tnk.motion.v!=0) && tdone(tnk.und.neffect))
  switch(tnk.und.nw)
  { case WATER:
     create_effect(o->x+rnd(16)-8, o->y+rnd(16)-8, 2, SPLASH);
     tnk.und.neffect=tinit(30+rnd(30));
     break;

    case OIL:
     if (strength[0][0][x+map.sx*y]==0)
      create_effect(o->x+rnd(8)-4, o->y+rnd(8)-4, 0, OIL_SPLASH);
     tnk.und.neffect=tinit(30+rnd(20));
     break;

    case NOTHING:
     if (map.background==0)
     { create_effect(o->x+rnd(16)-8, o->y+rnd(16)-8, 0, DUST);
       tnk.und.neffect=tinit(10+rnd(10));
     }
     break;
  }
}

static void invisible_effects(objectstruct *o)
{ if (tnk.invis.count!=0)
  { if (tdone(tnk.invis.ncount))
    { tnk.invis.count--;
      tnk.invis.ncount=tinit(300);
    }

    if (tdone(tnk.invis.nflash))
    { tnk.invis.flash^=1;
      tnk.invis.nflash=tinit(tnk.invis.count==2? (tnk.invis.flash==1? 100:10):5);
    }
  } else tnk.invis.flash=1;
}

void shield_effects(objectstruct *o)
{ if (tnk.shield.icount!=0 && tdone(tnk.shield.invun))
  { tnk.shield.icount--;
    tnk.shield.invun=tinit(5);
  }

  if (tnk.contr.shield!=0)
  { tnk.shield.power-=(timer-tnk.shield.pcount)*60;
    if (tnk.shield.power<0) tnk.shield.power=0;
  }
  else
  { tnk.shield.power+=(timer-tnk.shield.pcount)*10;
    if (tnk.shield.power>6000) tnk.shield.power=6000;
  }

  tnk.shield.pcount=timer;
}

static void tank_effects(objectstruct *o)
{ long i;

  if (tdone(tnk.motion.rotate) && tnk.tank->turret.main_static==MAIN_ROTATE)
  { tnk.motion.ldir=(tnk.motion.ldir+1)&15;
    tnk.motion.rotate=tinit(tnk.tank->turret.auto_rotation);
  }

  if (tdone(tnk.motion.smoke) && tnk.und.nw!=WATER && tnk.motion.v!=0 && tnk.tank->general.smoke)
  { i=2*rnd(2);
    create_effect(o->x+loc_x(tnk.motion.dir, &tnk.tank->smoke[i]), o->y+loc_y(tnk.motion.dir, &tnk.tank->smoke[i]), o->h+1, SMOKE);
    tnk.motion.smoke=tinit((tnk.shield.armor<=tnk.tank->props.armor/3? 5:30)+rnd(10));
  }

  if (tnk.und.smokes>0 && tdone(tnk.und.nsmoke))
  { i=2*rnd(2);
    create_effect(o->x+loc_x(tnk.motion.dir, &sscreen[i]), o->y+loc_y(tnk.motion.dir, &sscreen[i]), 6, SMOKE_SCREEN);
    tnk.und.nsmoke=tinit(10+rnd(10));
    tnk.und.smokes--;
  }

  tnk.weap.chf=0;
}

static void tank_refill(objectstruct *o)
{ long i;

  if (tnk.und.in_base && tnk.weap.reload && tnk.shield.armor>0)
  { if (o->owner->d.pl.weap.active)
    { for (i=0; i<num_weapon; i++)
       if (tnk.weap.nw[i]<o->owner->d.pl.weap.nw[i])
	tnk.weap.nw[i]=o->owner->d.pl.weap.nw[i];
    }
    else
    { for (i=0; i<num_weapon; i++)
       if (tnk.weap.nw[i]<tnk.tank->weap.num[i])
	tnk.weap.nw[i]=tnk.tank->weap.num[i];
    }

    tnk.motion.fuel=tnk.tank->props.max_fuel;
    tnk.shield.armor=tnk.tank->props.armor;

    tnk.weap.reload=FALSE;
  }
}

void control_tank(objectstruct *o)
{ msg_control(o);
  new_position(o);
  new_velocity(o);
  get_underground(o);
  underground_effects(o);
  invisible_effects(o);
  shield_effects(o);
  tank_effects(o);
  tank_refill(o);

  if (tnk.contr.computer)
  { control_computer_tank(o);
    left_right(o);
  }
  else if (game.mode==DUAL ||
	   game.id[0]==tnk.contr.id ||
	   (game.mode==DUALVC && game.id[1]==tnk.contr.id))
  { if (tnk.lock.active)
    { copy_controls_relative(o);
      tank_marker_control(o);
    }
    else
    { if (rel_or_dir[tnk.contr.num]==0)
       copy_controls_relative(o);
       else copy_controls_direct(o);

      left_right(o);

      if (tnk.contr.fire!=0)
       tank_fire(o, NULL);
    }
  }
  else clear_controls(o);
}
