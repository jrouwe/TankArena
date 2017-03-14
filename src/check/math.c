/* Some functions to make calculation in the game easier. This has mainly
   to do with the way angles are used in the game. There are angles in
   radians, and angles of the type char which runs from 0..15. 0 is straight
   up and this gives some incompatibilities here and there.
*/

#include <tank.h>
#include <math.h>

long radtoa[]={4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4};

float atorad[]={0.5*PI, 0.375*PI, 0.25*PI, 0.125*PI,
		0, 1.875*PI, 1.75*PI, 1.625*PI,
		1.5*PI, 1.375*PI, 1.25*PI, 1.125*PI,
		PI, 0.875*PI, 0.75*PI, 0.625*PI};

float add_angle(float *x, float *y, long v, float a, long *count)
{ float d=(float)v*(timer-*count)/sec, dx, dy;

  dx=cos(a)*d;
  *x+=dx;

  dy=sin(a)*d;
  *y-=dy;

  *count=timer;

  return(sqrt(dx*dx+dy*dy));
}

float add_vector(float *x, float *y, float vx, float vy, long *count)
{ float d=(float)(timer-*count)/sec, dx, dy;

  dx=d*vx;
  *x+=dx;

  dy=d*vy;
  *y+=dy;

  *count=timer;

  return(sqrt(dx*dx+dy*dy));
}

long home(objectstruct *o, long *target_x, long *target_y, float *a)
{ objectstruct *target=o->target;
  long x=o->x, y=o->y;

  if (target!=NULL) *target_x=target->x, *target_y=target->y;

  *a=angle(*target_x-x, *target_y-y);

  return(distance(x-*target_x, y-*target_y));
}

float angle(long dx, long dy)
{ float a;

  if (dx==0)
   if (dy<=0) return(0.5*PI);
   else return(1.5*PI);

  else if (dx>0) a=-atan((float)dy/(float)dx);

  else a=PI-atan((float)dy/(float)dx);

  return(a<0? 2*PI+a:a);
}

float in2pi(float a)
{ while (a<0 || a>2*PI)
  { if (a<0) a+=2*PI;
    else a-=2*PI;
  }

  return(a);
}

long radtoa16(float a)
{ return(radtoa[pround(a/(2*PI)*16)]);
}

float a16torad(long a)
{ return(atorad[a]);
}

long loc_x(long dir, long *loc)
{ float a=0.5*PI-atorad[dir];

  return(round(cos(a)*loc[0]-sin(a)*loc[1]));
}

long loc_y(long dir, long *loc)
{ float a=0.5*PI-atorad[dir];

  return(round(sin(a)*loc[0]+cos(a)*loc[1]));
}
