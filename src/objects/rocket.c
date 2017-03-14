/* Controls the homing missile */

#include <tank.h>

extern unsigned char *rock[4][5];

#define rck o->d.ro

static long smp[2]={0,11};

static void write_rocket(objectstruct *o)
{ b_add_layer_d(o->x-ssx, o->y-ssy, rck.angle, rock[1]);
}

static void control_rocket(objectstruct *o)
{ long x, y, done;
  float a;

  done=home(o, &rck.target_x, &rck.target_y, &a)<7;
  rck.dist-=add_angle(&o->fx, &o->fy, rck.v, a, &rck.count);
  o->x=pround(o->fx), o->y=pround(o->fy);
  rck.angle=radtoa16(a), x=o->x, y=o->y;

  if (tdone(rck.smoke))
  { create_effect(x+loc_x(rck.angle, smp), y+loc_y(rck.angle, smp), 5, EXHAUST);
    rck.smoke=tinit(10+rnd(20));
  }

  if (rck.v<150) rck.v++;

  if (done || rck.dist<0)
  { if (o->target!=NULL && rck.dist>=0)
     check_obj_hit_radius(o, o->target->x+16, o->target->y+16, 10, HT_ROCKET, rck.power);
     else check_wall_hit(o->owner, rck.target_x+16, rck.target_y+16, 10, HT_ROCKET, rck.power);

    remove_object(o);
  }
}

static void hit_rocket(objectstruct *o, long power, long type)
{ remove_object(o);
}

static void remove_rocket(objectstruct *o)
{ create_effect(o->x, o->y, o->h+1, LEXPLOSION);
}

void create_rocket(objectstruct *owner, long x, long y, long target_x, long target_y, long power)
{ objectstruct *o, *target;

  target=nearest_target(target_x, target_y);

  o=create_object(ROCKET_TYPE, x, y, 6, 3);

  o->funct.control=control_rocket;
  o->funct.write=write_rocket;
  o->funct.hit=hit_rocket;
  o->funct.remove=remove_rocket;

  o->owner=owner;
  o->target=target;

  rck.v=0;
  rck.count=timer;
  rck.target_x=target_x;
  rck.target_y=target_y;
  rck.power=power;
  rck.smoke=tnow;
  rck.dist=3300;
  rck.angle=radtoa16(angle(target_x-x, target_y-y));

  create_message(target, "INCOMING", "MISSILE!");

  create_sound_relative(target, SND_INCOMING);
}
