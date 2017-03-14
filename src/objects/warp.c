/* Object: Warp. This object warps tanks/men/b52's and rockets from one
   place in the map to another.
*/

#include <tank.h>

static void mwrite_warp(objectstruct *o, objectstruct *ao)
{ warpstruct *w=&o->d.wa;

  draw_circle(o->x, o->y, 15, 95);
  b_write_text(o->x-ssx+4, o->y-ssy+9, 95, "WARP");
  b_write_text(o->x-ssx+10, o->y-ssy+17, 95, "IN");

  if (w->x!=o->x || w->y!=o->y)
  { draw_line(o->x, o->y, w->x, w->y, 95);
    b_write_text(w->x-ssx+8, w->y-ssy+14, 95, "OUT");
  }
}

static void control_warp(objectstruct *o)
{ warpstruct *w=&o->d.wa;
  objectstruct *p;
  long i;

  for (p=obj; p!=NULL; p=p->next)
   if (in_radius(p->x-o->x, p->y-o->y, 15))
    switch(p->type)
    { case TANK_TYPE:
      case MAN_TYPE:
      case B52_TYPE:
      case ROCKET_TYPE:
       for (i=0; i<3; i++)
       { create_effect(p->x+8-rnd(16), p->y+8-rnd(16), p->h+1, TELEPORT_SPARK+rnd(3));
	 create_effect(w->x+8-rnd(16), w->y+8-rnd(16), p->h+1, TELEPORT_SPARK+rnd(3));
       }

       p->x=w->x, p->fx=w->x;
       p->y=w->y, p->fy=w->y;

       create_sound_absolute(o->x, o->y, SND_TELEPORT);
       create_sound_absolute(w->x, w->y, SND_TELEPORT);

       break;
    }
}

objectstruct *make_warp(long x, long y)
{ objectstruct *o;

  o=create_object(WARP_TYPE, x, y, 24, 0);

  o->funct.control=control_warp;
  o->funct.mwrite=mwrite_warp;

  o->d.wa.x=x;
  o->d.wa.y=y;

  return(o);
}

void prepare_warp(objectstruct *o)
{ o->funct.control=control_warp;
  o->funct.mwrite=mwrite_warp;
}
