/* The object: goal. Goals for single/dual player missions. */

#include <tank.h>

#define g o->d.go

static void mwrite_goal(objectstruct *o, objectstruct *ao)
{ draw_circle(o->x, o->y, g.r, 95);
  b_write_text(o->x-ssx+4, o->y-ssy+14, 95, "GOAL");

  if (ao==o && o->target!=NULL)
   draw_line(o->x, o->y, o->target->x, o->target->y, 95);
}

static void control_goal(objectstruct *o)
{ objectstruct *p;

  switch(g.who)
  { case 0:
     for (p=obj; p!=NULL; p=p->next)
      if (p->type==TANK_TYPE && p->d.tk.contr.good && in_radius(p->x-o->x, p->y-o->y, g.r) &&
	  (!game.supermarket_mayhem || game.mayhem_products == 0))
      {

	remove_object(o);
	return;
      }
     break;

    case 1:
     for (p=obj; p!=NULL; p=p->next)
      if (p->type==TANK_TYPE && !p->d.tk.contr.good && in_radius(p->x-o->x, p->y-o->y, g.r))
      { remove_object(o);
	return;
      }
     break;

    case 2:
     for (p=obj; p!=NULL; p=p->next)
      if ((p->type==TRAIN_TYPE || p->type==WAGON_TYPE) && in_radius(p->x-o->x, p->y-o->y, g.r))
      { remove_object(o);
	return;
      }
     break;

    case 3:
     if (o->target!=NULL && in_radius(o->target->x-o->x, o->target->y-o->y, g.r))
     { remove_object(o);
       return;
     }
    break;
  }
}

objectstruct *make_goal(long x, long y)
{ objectstruct *o;

  o=create_object(GOAL_TYPE, x, y, 24, 0);

  o->funct.control=control_goal;
  o->funct.mwrite=mwrite_goal;

  g.r=30;

  return(o);
}

void prepare_goal(objectstruct *o)
{ o->funct.control=control_goal;
  o->funct.mwrite=mwrite_goal;
}

