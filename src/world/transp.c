/* Controls the rotating, rolling things for tanks */

#include <tank.h>
#include <math.h>

static long count, new_game;

void clear_transp(void)
{ new_game=TRUE;
}

void transportation(void)
{ float fx, fy, d, dx, dy, a;
  objectstruct *o, *p;
  long xm, ym;

  if (new_game)
   count=timer, new_game=FALSE;

  d=(float)(timer-count)/sec;
  count=timer;

  for (o=obj; o!=NULL && o->h<2; o=o->next)
   if (o->type!=PLAYER_TYPE)
   { fx=o->fx, fy=o->fy;
     xm=(o->x+16)/b_size, ym=(o->y+16)/b_size;

     switch(get_type(xm, ym, 0))
     { case T_LEFT:
	o->fx-=d*60;
	goto update;

       case T_RIGHT:
	o->fx+=d*60;
	goto update;

       case T_UP:
	o->fy-=d*60;
	goto update;

       case T_DOWN:
	o->fy+=d*60;
	goto update;

       case ROTATE_L:
	xm=xm*b_size, ym=ym*b_size;
	dx=fx-xm, dy=fy-ym, a=2*PI*d;
	o->fx=dx*cos(a)+dy*sin(a)+xm;
	o->fy=dy*cos(a)-dx*sin(a)+ym;
	goto update;

       case ROTATE_R:
	xm=xm*b_size, ym=ym*b_size;
	dx=fx-xm, dy=fy-ym, a=-2*PI*d;
	o->fx=dx*cos(a)+dy*sin(a)+xm;
	o->fy=dy*cos(a)-dx*sin(a)+ym;

       update:;
	o->x=pround(o->fx), o->y=pround(o->fy);

	if (o->type==TANK_TYPE)
	 for (p=obj; p!=NULL; p=p->next)
	  if (p!=o && p->type==TANK_TYPE && abs(p->h-o->h)<4 && in_radius(o->x-p->x, o->y-p->y, o->r+p->r))
	   o->fx=fx, o->fy=fy, o->x=pround(o->fx), o->y=pround(o->fy);
	break;
     }
   }
}
