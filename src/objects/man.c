/* Controls those nice little pixels that are supposed to be men. */

#include <tank.h>

#define m o->d.ma

extern unsigned char *man[10];

static long mgun[]={1,-3};

static void write_man(objectstruct *o)
{ b_add_layer_d(o->x-ssx, o->y-ssy, o->d.ma.dir, &man[o->d.ma.select]);
}

static void mwrite_man(objectstruct *o, objectstruct *ao)
{ draw_circle(o->x, o->y, 100, 31);
}

static void control_man(objectstruct *o)
{ long x, y, d;
  float a;

  if (home(o, &m.target_x, &m.target_y, &a)>20)
  { add_angle(&o->fx, &o->fy, 5, a, &m.count);
    o->x=pround(o->fx), o->y=pround(o->fy);

    if (tdone(m.ns))
    { m.select^=5;
      m.ns=tinit(10+rnd(10));
    }
  } else m.count=timer;

  d=radtoa16(a), m.dir=d, x=o->x+16, y=o->y+16;

  if (m.bullets==0 ||
      check_wall_hit_point(o, x, y, 1, HT_CHAIN, 10) ||
      get_type(x/b_size, y/b_size, 0)==WATER ||
      in_pit(o->x, o->y))
  { remove_object(o);
    return;
  }

  if (tdone(m.fire))
  { m.fire=tinit(40+rnd(40));
    create_bullet(o, m.type, x+loc_x(d,mgun), y+loc_y(d,mgun), d, m.power, 5*b_size, 0);
    m.bullets--;
  }
}

static void hit_man(objectstruct *o, long power, long type)
{ remove_object(o);
}

static void remove_man(objectstruct *o)
{ create_effect(o->x, o->y, 0, BLOOD);
  create_sound_absolute(o->x, o->y, SND_SCREAM);
}

objectstruct *make_man(objectstruct *owner, long x, long y, long target_x, long target_y, long type, long power)
{ objectstruct *o, *target;

  target=nearest_target(target_x, target_y);

  o=create_object(MAN_TYPE, x, y, 0, 5);

  o->funct.control=control_man;
  o->funct.write=write_man;
  o->funct.mwrite=mwrite_man;
  o->funct.hit=hit_man;
  o->funct.remove=remove_man;

  o->owner=owner;
  o->target=target;

  m.count=timer;
  m.type=type;
  m.target_x=target_x;
  m.target_y=target_y;
  m.bullets=75;
  m.fire=tnow;
  m.dir=0;
  m.select=0;
  m.ns=tnow;
  m.power=power;

  return(o);
}
