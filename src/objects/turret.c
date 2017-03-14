/* Controls the various turrets. */

#include <tank.h>
#include <math.h>

#define tnk o->d.tk
#define tur o->d.tu

turretinfo ti[num_turrets]={{150, 5, 200, 3, FALSE, HT_MAIN, {1, 0,-15}},
			    {200, 5, 200, 4, FALSE, HT_ROCKET, {1, 0,-6}},
			    {100, 5, 200, 1, FALSE, HT_MAIN, {1, 0,-14}},
			    {10, 1, 200, 1, FALSE, HT_MAIN, {1, 0,-6}},
			    {150, 5, 200, 1, FALSE, HT_MAIN, {1, 0,-14}},
			    {150, 5, 200, 1, FALSE, HT_MAIN, {1, 0,-9}},
			    {150, 5, 200, 1, FALSE, HT_MAIN, {1, 0,-6}},
			    {150, 5, 200, 1, FALSE, HT_MAIN, {1, 0,-7}},
			    {150, 5, 200, 1, FALSE, HT_MAIN, {2, -2,-6, 2,-6}}};

static objectstruct *closest_tank(char shootat, long x, long y)
{ objectstruct *o, *c=NULL;
  long r=10000, r2;

  for (o=obj; o!=NULL; o=o->next)
  { if (o->type!=TANK_TYPE) continue;

    if (tnk.shield.armor==0 || tnk.invis.count!=0)
     continue;

    switch(shootat)
    { case 1:
       if (!tnk.contr.good)
	continue;
       break;

      case 2:
       if (tnk.contr.good)
	continue;
       break;

      case 3:
       if (tnk.contr.num==0)
	continue;
       break;

      case 4:
       if (tnk.contr.num==1)
	continue;
       break;
    }

    r2=distance(x-o->x, y-o->y);

    if (r2<r) r=r2, c=o;
  }

  return(c);
}

static void write_turret(objectstruct *o)
{ b_add_layer_d(o->x-ssx, o->y-ssy, tur.dir, ti[tur.type].pic);
}

static void mwrite_turret(objectstruct *o, objectstruct *ao)
{
  float a, r = tur.radius;
  int d;

  if (o == ao)
  {
    draw_circle(o->x, o->y, r, 15);

    if (tur.fixed)
      {
	a = a16torad(tur.dir);
	draw_line(o->x, o->y, o->x+r*cos(a-0.2), o->y-r*sin(a-0.2), 15);
	draw_line(o->x, o->y, o->x+r*cos(a+0.2), o->y-r*sin(a+0.2), 15);
      }
    else
      for (d = 0; d < 16; d ++)
      {
	a = a16torad(d);     
	draw_line(o->x, o->y, o->x+r*cos(a), o->y-r*sin(a),
		  ((tur.forbid_dir >> d) & 1) == 0? 15:95);
      }
  }
}

static void control_turret(objectstruct *o)
{ objectstruct *target;
  long dir, x=o->x, y=o->y;

  if (o->owner==NULL && strength[0][0][((x+16)/b_size)+map.sx*((y+16)/b_size)]==0)
  { remove_object(o);
    return;
  }

  target=closest_tank(tur.shootat, x, y);

  if (target!=NULL)
  { dir=radtoa16(angle(target->x-x, target->y-y));

    if (!tur.fixed && tdone(tur.nd) && tur.dir!=dir && in_radius(target->x-x, target->y-y, tur.radius))
    { tur.dir=(tur.dir+go_left_or_right(dir, tur.dir))&15;
      tur.nd=tinit(35);
    }

    if (in_radius(x-target->x, y-target->y, tur.radius)
	&& tdone(tur.wait)
	&& dir==tur.dir
	&& (tur.fixed || ((tur.forbid_dir >> tur.dir) & 1) == 0)
       )
    { switch(ti[tur.type].shootwith)
      { case HT_MAIN:
	 create_bullet(o, tur.power<2? HT_CHAIN:HT_MAIN, x+loc_x(dir, &ti[tur.type].loc[1+2*tur.activegun])+16, y+loc_y(dir, &ti[tur.type].loc[1+2*tur.activegun])+16, dir, tur.power, tur.radius, 0);
         break;

        case HT_ROCKET:
	 create_rocket(o, x+loc_x(dir, &ti[tur.type].loc[1+2*tur.activegun]), y+loc_y(dir, &ti[tur.type].loc[1+2*tur.activegun]), target->x, target->y, tur.power);
         break;
      }

      tur.activegun++;
      if (tur.activegun==ti[tur.type].loc[0]) tur.activegun=0;

      tur.wait=tinit(tur.delay);
    }
  }
}

objectstruct *make_turret(long x, long y, long dir, long h, long type, objectstruct *owner)
{ objectstruct *o;

  o=create_object(TURRET_TYPE, x, y, h, 0);

  o->funct.control=control_turret;
  o->funct.write=write_turret;
  o->funct.mwrite=mwrite_turret;

  o->owner=owner;

  tur.type=type;
  tur.wait=tnow;
  tur.dir=dir*2;
  tur.nd=tnow;
  tur.delay=ti[type].delay;
  tur.radius=ti[type].radius;
  tur.shootat=ti[type].shootat;
  tur.fixed=ti[type].fixed;
  tur.power=ti[type].power;

  return(o);
}

void prepare_turret(objectstruct *o)
{ o->funct.control=control_turret;
  o->funct.write=write_turret;
  o->funct.mwrite=mwrite_turret;
}
