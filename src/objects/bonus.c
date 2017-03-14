/* Controls bonusses. */

#include <tank.h>

#define tnk o->d.tk

static void write_bonus(objectstruct *o)
{ b_add_layer(o->x+12-ssx, o->y+12-ssy, icons[0][o->d.bo.type]);
}

void pickup_bonus(objectstruct *o, objectstruct *b)
{ char *m;

  switch(b->d.bo.type)
  { case 0:
     m="10 GRENADES";
     tnk.weap.nw[0]+=10;
     break;

    case 1:
     if (!tnk.weap.extragun)
     { m="EXTRA GUN";
       tnk.weap.extragun=TRUE;
     }
     else
     { m="500 ROUNDS";
       tnk.weap.nw[1]+=500;
     }
     break;

    case 2:
     m="FUEL";
     tnk.motion.fuel=tnk.tank->props.max_fuel;
     break;

    case 3:
     m="5 MINES";
     tnk.weap.nw[3]+=5;
     break;

    case 4:
     m="3 ROCKETS";
     tnk.weap.nw[4]+=3;
     break;

    case 5:
     m="A MORTAR SHELL";
     tnk.weap.nw[5]++;
     break;

    case 6:
     m="NUCLEAR BOMB";
     tnk.weap.nw[6]++;
     break;

    case 7:
     m="5 GUNNERS";
     tnk.weap.nw[7]+=5;
     break;

    case 8:
     m="5 FLAMERS";
     tnk.weap.nw[8]+=5;
     break;

    case 9:
     m="SMOKE SCREEN";
     tnk.weap.nw[9]++;
     break;

    case 10:
     m="A CLOAKING DEVICE";
     tnk.weap.nw[10]++;
     break;

    case 11:
     m="SPEEDUP";
     tnk.weap.nw[11]++;
     break;

    case 12:
     m="INVUNERABILITY";
     tnk.shield.invun=tinit(2000);
     tnk.shield.icount=15;
     break;

    case 13:
     m="ARMOR!";
     tnk.shield.armor+=10;
     if (tnk.tank->props.armor<tnk.shield.armor)
      tnk.shield.armor=tnk.tank->props.armor;
     break;

    default:
     m="JUNK!";
     break;
  }

  create_sound_absolute(o->x, o->y, SND_PICKUP);

  create_message(o, "PICKED UP", m);

  remove_object(b);
}

static void hit_bonus(objectstruct *o, long power, long type)
{ create_effect(o->x, o->y, 2, EXPLOSION);

  remove_object(o);
}

static void make_bonus(long x, long y, long type)
{ objectstruct *o;

  if (type>=0)
  { o=create_object(BONUS_TYPE, x*b_size-4+rnd(8), y*b_size-4+rnd(8), 0, 2);

    o->funct.write=write_bonus;
    o->funct.hit=hit_bonus;

    o->d.bo.type=type;
  }
}

void create_all_bonusses(void)
{ long x, y;

  if (!map.random_bonus)
   for (x=0; x<map.sx; x++)
    for (y=0; y<map.sy; y++)
     if (strength[0][0][x+map.sx*y]==0)
      make_bonus(x, y, mapm.b[x+map.sx*y]);
}

void create_bonus(long x, long y)
{ long type;

  if (map.random_bonus)
  { type=rnd(10000);

    if (type<300) type=0;
    else if (type<600) type=1;
    else if (type<700) type=2;
    else if (type<800) type=3;
    else if (type<850) type=4;
    else if (type<900) type=5;
    else if (type<950) type=6;
    else if (type<1000) type=7;
    else if (type<1050) type=8;
    else if (type<1100) type=9;
    else if (type<1150) type=10;
    else if (type<1200) type=11;
    else if (type<1225) type=12;
    else if (type<1250) type=13;
    else type=-1;
  }

  else type=mapm.b[x+map.sx*y];

  make_bonus(x, y, type);
}
