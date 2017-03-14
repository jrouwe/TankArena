/* Object: lock. Blows structure/object when another structure/object
   is blown.
*/

#include <tank.h>

#define l o->d.lo

static void mwrite_lock(objectstruct *o, objectstruct *ao)
{
  switch(l.lock_activate)
  {
    case 0:
      draw_circle(o->x, o->y, 15, 95);
      b_write_text(o->x-ssx+8, o->y-ssy+14, 95, "KEY");
      break;
  
    case 1:
      draw_circle(o->x, o->y, 15, 95);
      b_write_text(o->x-ssx+8, o->y-ssy+14, 95, "OBJ");
      break;
  }

  switch(l.lock_target)
  { 
    case 0:
      draw_line(o->x, o->y, l.x, l.y, 95);
      b_write_text(l.x-ssx-7, l.y-ssy+14, 95, "STR LOCK");
      break;

    case 1:
      draw_line(o->x, o->y, l.x, l.y, 95);
      b_write_text(l.x-ssx-7, l.y-ssy+14, 95, "OBJ DEST");
      break;

    case 2:
      draw_line(o->x, o->y, l.x, l.y, 95);
      b_write_text(l.x-ssx-7, l.y-ssy+14, 95, "OBJ REMV");
      break;
  }
}

static void blow_lock(objectstruct *o)
{
  switch(l.lock_target)
  {
    case 0:
      hit_wall_for(NULL, l.x+16, l.y+16, 0, HT_REMOVE, 1000);
      break;

    case 1:
      if (o->target != 0)
	hit_obj_for(o->target, o->target->x+16, o->target->y+16, HT_DESTROY, 1000);
      break;

    case 2:
      if (o->target != 0)
	remove_object(o->target);
      break;
  }

  remove_object(o);
}

static void control_lock(objectstruct *o)
{
  long x, y;

  switch(l.lock_activate)
  {
    case 0:
      x = (o->x + 16) / b_size, y = (o->y + 16) / b_size;

      if (strength[0][0][x + map.sx*y]==0)
	blow_lock(o);
      break;

    case 1:
      if (o->owner == 0 ||
	  (o->owner->type == TANK_TYPE && o->owner->d.tk.shield.armor == 0) ||
	  (o->owner->type == B52_TYPE && o->owner->d.b52.armor == 0))
	blow_lock(o);
      break;
  }
}

objectstruct *make_lock(long x, long y)
{
  objectstruct *o;

  o=create_object(LOCK_TYPE, x, y, 24, 0);

  o->funct.control=control_lock;
  o->funct.mwrite=mwrite_lock;

  l.x=x;
  l.y=y;

  l.lock_activate = 0;
  l.lock_target = 0;

  return(o);
}

void prepare_lock(objectstruct *o)
{
  long x, y;

  o->funct.control=control_lock;
  o->funct.mwrite=mwrite_lock;

  switch(l.lock_activate)
  {
    case 0:
      break;

    case 1:
      o->owner = nearest_object(o, o->x, o->y);
      break;
  }

  switch(l.lock_target)
  {
    case 0:
      x=(l.x+16)/b_size, y=(l.y+16)/b_size;

      if (mapm.m[1][x+map.sx*y]!=-1)
	strength[0][0][x+map.sx*y]=1000;

      if (mapm.m[2][x+map.sx*y]!=-1)
	strength[1][0][x+map.sx*y]=1000;

      break;

    case 1:
    case 2:
      o->target = nearest_object(o, l.x, l.y);
      break;
  }
}
