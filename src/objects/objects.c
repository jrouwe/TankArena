/* General functions for object processing in a linked list. All objects
   have functions which are called to write/control/destroy them.
*/

#include <tank.h>
#include <string.h>

extern long no_mission_fail;

objectstruct *obj=NULL;
long num_objects;

static objectstruct *next = NULL;

void add_object_to_list(objectstruct *o)
{ objectstruct *p;

  if (obj==NULL || o->h<=obj->h)
  { o->prev=NULL;
    o->next=obj;

    if (obj!=NULL)
     obj->prev=o;

    obj=o;

    return;
  }

  for (p=obj; p!=NULL; p=p->next)
   if (o->h<=p->h)
   { o->prev=p->prev;
     o->next=p;
     p->prev->next=o;
     p->prev=o;
     return;
   }

  for (p=obj; p->next!=NULL; p=p->next);

  o->prev=p;
  o->next=NULL;
  p->next=o;
}

void remove_object_from_list(objectstruct *o)
{ if (o->prev!=NULL)
  { o->prev->next=o->next;

    if (o->next!=NULL)
     o->next->prev=o->prev;
  }
  else
  { obj=o->next;

    if (obj!=NULL)
     obj->prev=NULL;
  }

  /* When looping all objects, avoid looking at already removed object */

  if (o == next)
    next = o->next;
}

void resort_object(objectstruct *o)
{ remove_object_from_list(o);
  add_object_to_list(o);
}

void clear_objects(void)
{ objectstruct *o;

  while(obj!=NULL)
  { o=obj, obj=obj->next;

    o->id=0L;

    free(o);
  }
}

objectstruct *create_object(long type, long x, long y, long h, long r)
{ objectstruct *o;

  o=allocate(sizeof(objectstruct));
  memset(o, 0, sizeof(objectstruct));

  o->id=OBJ_MAGIC;

  o->type=type;

  o->x=x, o->fx=x;
  o->y=y, o->fy=y;
  o->h=h, o->fh=h;
  o->r=r;

  add_object_to_list(o);

  return(o);
}

void remove_object(objectstruct *o)
{ objectstruct *p;

  if (o==NULL)
  { if (debug) error("remove_object: removing NULL object\n");
    return;
  }

  if (obj==NULL)
  { if (debug) error("remove_object: no objects in memory\n");
    return;
  }

  if (o->id!=OBJ_MAGIC)
  { if (debug) error("remove_object: invalid magic number, object type: %d\n", o->type);
    return;
  }

  if (o->funct.remove!=NULL && game.running)
   o->funct.remove(o);

  for (p=obj; p!=NULL; p=p->next)
  { if (p->owner==o) p->owner=NULL;
    if (p->target==o) p->target=NULL;
  }

  modify_goal_counter(o->gc);

  o->id=0L;

  remove_object_from_list(o);
  free(o);
}

void control_objects(void)
{ objectstruct *o;

  for (o=obj; o!=NULL; o=next)
  {
    next = o->next;

    if (o->funct.control!=NULL)
    {
      if (o->id!=OBJ_MAGIC)
      {
	if (debug) error("control_object: invalid magic number, object type: %d\n", o->type);
	return;
      }

      o->funct.control(o);
    }
  }
}

void write_objects(objectstruct *ao)
{ objectstruct *o, *l;
  effectstruct *e;
  long h;

  o=obj, l=obj, e=effect, num_objects=0;

  for (h=-4; h<25; h++)
  { if (h==4)
    { top_layer(ssx, ssy);
      write_parts();
      write_bullets();
    }

    for (; e!=NULL && e->h==h; e=e->next)
     b_add_layer(e->x-ssx, e->y-ssy, eff[e->pn].ptr);

    for (; o!=NULL && o->h==h; o=o->next, num_objects++)
    { if (o->funct.write!=NULL)
       o->funct.write(o);

      if (o->funct.mwrite!=NULL && game.mode==MAP)
       o->funct.mwrite(o, ao);
    }

    if (map.night && game.mode!=MAP)
     for (; l!=NULL && (l->h>4? l->h:4)==h; l=l->next)
      if (l->funct.light!=NULL)
       l->funct.light(l);
  }
}

objectstruct *nearest_target(long x, long y)
{
  objectstruct *o, *n = 0;
  long d, r = 15*15;

  for (o=obj; o!=NULL; o=o->next)
    if (o->r > 0)
    {
      d = distance2(o->x-x, o->y-y);

      if (d < r)
	r = d, n = o;
    }

  return(n);
}

objectstruct *nearest_object(objectstruct *p, long x, long y)
{ 
  objectstruct *o, *n = 0;
  long d, r = 15*15;

  for (o=obj; o!=NULL; o=o->next)
    if (o != p)
    {
      d = distance2(o->x-x, o->y-y);

      if (d < r)
	r = d, n = o;
    }

  return(n);
}

void modify_goal_counter(long percentage)
{ if (game.mode==DUAL || game.winner!=0)
   return;

  if (percentage>0)
   game.gc_good+=percentage;
   else if (!no_mission_fail) game.gc_bad-=percentage;

  if (game.gc_good>=100)
  { game.gc_good=100;
    game.winner=1;
    game.end_round=tinit(300);
  }

  else if (game.gc_bad>=100)
  { game.gc_bad=100;
    game.winner=2;
    game.end_round=tinit(300);
  }
}
