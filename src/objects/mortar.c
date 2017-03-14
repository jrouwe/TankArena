/* The mortar explosion */

#include <tank.h>

#define m o->d.mo

static void control_mortar(objectstruct *o)
{ if (tdone(m.nexpl))
  { check_obj_hit_radius(o, o->x, o->y, m.expl, HT_MORTAR, 5);
    hit_wall_big(o->owner, o->x, o->y, m.expl, 2, HT_MORTAR, 5);

    m.expl+=5;

    if (m.expl<=60)
     m.nexpl=tinit(3);
     else remove_object(o);
  }
}

void create_mortar(objectstruct *owner, long x, long y)
{ objectstruct *o;

  create_parts(PT_MORTAR, x, y);
  create_sound_absolute(x, y, SND_NUKE);

  o=create_object(MORTAR_TYPE, x, y, 0, 0);

  o->funct.control=control_mortar;
  o->owner=owner;

  m.expl=5;
  m.nexpl=tnow;
}
