/* Everything about controlling abombs */

#include <tank.h>

extern long nuke_total, palette_update;

#define ab o->d.ab
#define abomb_power 4

void clear_abombs(void)
{ nuke_total=0;
  palette_update=FALSE;
}

static void write_abomb(objectstruct *o)
{ if (ab.nuke_count==30 && get_type((o->x+16)/b_size, (o->y+16)/b_size, 0)!=WATER)
   b_add_layer(o->x-ssx, o->y-ssy, intern[ABOMB+3-ab.count].ptr);
}

static void control_abomb(objectstruct *o)
{ long l, x, y;

  if (tdone(ab.next))
  { if (ab.count==0)
    { if (ab.nuke_count==30)
       create_sound_absolute(o->x, o->y, SND_NUKE);

      if (ab.nuke_count>20) nuke_total+=6; else nuke_total-=3;
       palette_update=TRUE;

      ab.nuke_count--;
      ab.next=tinit(6);

      x=o->x+16, y=o->y+16, l=(30-ab.nuke_count)*7;

      hit_wall_big(o->owner, x, y, l, 2, HT_ABOMB, abomb_power);
      check_obj_hit_radius(o, x, y, l, HT_ABOMB, abomb_power);
    }
    else
    { ab.count--;
      ab.next=tinit(60);
    }
  }

  if (ab.nuke_count==0)
   remove_object(o);
}

static void hit_abomb(objectstruct *o, long power, long type)
{ if (ab.nuke_count==30)
   remove_object(o);
}

void create_abomb(objectstruct *owner, long x, long y, long count)
{ objectstruct *o;

  o=create_object(ABOMB_TYPE, x, y, 0, 3);

  o->funct.control=control_abomb;
  o->funct.write=write_abomb;
  o->funct.hit=hit_abomb;

  o->owner=owner;

  ab.next=tinit(count==0? 0:60);

  ab.count=count;
  ab.nuke_count=30;
}
