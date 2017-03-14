/* Object: mine. Controls the two different types of mines. */

#include <tank.h>

#define m o->d.mi

static void write_mine(objectstruct *o)
{ if (get_type((o->x+16)/b_size, (o->y+16)/b_size, 0)!=WATER && o->d.mi.type==1)
   b_add_layer(o->x-ssx, o->y-ssy, intern[MINE+o->d.mi.type].ptr);
}

void blowup_mine(objectstruct *o)
{ long x=o->x+16, y=o->y+16, i;

  switch(o->d.mi.type)
  { case LIGHT_MINE:
     check_obj_hit_radius(o, x, y, 30, HT_MINE, m.power);
     check_wall_hit(NULL, x, y, 2, HT_MINE, m.power);
     break;

    case HEAVY_MINE:
     for (i=0; i<16; i+=2)
      create_bullet(o, HT_SROCK, x, y, i, 10, 100, 0);
     break;
  }

  remove_object(o);
}

static void mwrite_mine(objectstruct *o, objectstruct *ao)
{ b_add_layer(o->x-ssx, o->y-ssy, intern[MINE+o->d.mi.type].ptr);
}

static void hit_mine(objectstruct *o, long power, long type)
{ o->funct.control=blowup_mine;
}

static void remove_mine(objectstruct *o)
{ create_effect(o->x, o->y, 2, LEXPLOSION);
}

void create_mine(long x, long y, long type, long power)
{ objectstruct *o;

  o=create_object(MINE_TYPE, x, y, 0, 3);

  o->funct.write=game.mode==MAP? NULL:write_mine;
  o->funct.hit=hit_mine;
  o->funct.remove=remove_mine;
  o->funct.mwrite=mwrite_mine;

  m.type=type;
  m.power=power;
  m.active=tinit(100);
}

void prepare_mine(objectstruct *o)
{ o->funct.write=game.mode==MAP? NULL:write_mine;
  o->funct.hit=hit_mine;
  o->funct.remove=remove_mine;
  o->funct.mwrite=mwrite_mine;

  m.active=tnow;
}
