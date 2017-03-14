/* Forces a tank to use some kind of weapon */

#include <tank.h>

#define e o->d.enf
#define t p->d.tk

static void mwrite_enforcer(objectstruct *o, objectstruct *ao)
{
  draw_circle(o->x, o->y, e.r, 95);
  b_write_text(o->x-ssx+4, o->y-ssy+14, 95, "ENFO");
}

static void control_enforcer(objectstruct *o)
{
  objectstruct *p;
  long w;

  for (p = obj; p != 0; p = p->next)
    if (p->type == TANK_TYPE && t.contr.computer &&
	in_radius(p->x-o->x, p->y-o->y, e.r) &&
	((t.contr.good && e.enf_good) ||
	 (!t.contr.good && e.enf_evil)))
      {
	w = t.weap.w;

	t.weap.w = e.weapon;

	if (e.fire && tdone(e.done))
	{
	  t.weap.nf = tnow;
	  t.weap.nw[e.weapon] = 1;
	  tank_fire(p, p->target);

	  e.done = tinit(e.delay);
	}

	if (!e.change)
	  t.weap.w = w;
      }
}

objectstruct *make_enforcer(long x, long y)
{
  objectstruct *o;

  o = create_object(ENFORCER_TYPE, x, y, 24, 0);

  o->funct.mwrite = mwrite_enforcer;
  o->funct.control = control_enforcer;

  e.r = 30;
  e.enf_good = FALSE;
  e.enf_evil = TRUE;
  e.weapon = 6;
  e.fire = TRUE;
  e.change = FALSE;
  e.delay = 500;
  e.done = tnow;

  return(o);
}

void prepare_enforcer(objectstruct *o)
{
  o->funct.mwrite = mwrite_enforcer;
  o->funct.control = control_enforcer;
}

