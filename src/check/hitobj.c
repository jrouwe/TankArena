/* Checks if an object is hit. Various objects are hit in various ways
   and each object type is handled separately.
*/

#include <tank.h>

#define tnk o->d.tk

static long determine_hit(objectstruct *o, long type, objectstruct *owner)
{ switch(o->type)
  { case TANK_TYPE:
     return(o->h>0 || type==HT_ROCKET);

    case ABOMB_TYPE:
     return(type!=HT_ABOMB);

    case MAN_TYPE:
     return(owner==NULL || owner->type!=MAN_TYPE || owner->owner!=o->owner);

    case B52_TYPE:
     return(type==HT_ROCKET || o->h<8);

    case TRAIN_TYPE:
    case WAGON_TYPE:
     return(owner==NULL || owner->type!=TURRET_TYPE);

    case FLAG_TYPE:
     return(FALSE);

    default:
     return(TRUE);
  }
}

long check_obj_hit_point(objectstruct *owner, long x, long y, long type, long power)
{ objectstruct *o;
  long x2, y2;

  for (o=obj; o!=NULL; o=o->next)
  { if (o==owner || o->r==0)
     continue;

    if (o->type==TANK_TYPE)
    { x2=x-o->x, y2=y-o->y;

      if (x2>=0 && x2<b_size && y2>=0 && y2<b_size &&
	  *(tnk.tank->pic[tnk.motion.dir]+x2+y2*b_size)!=0xff)
      { hit_obj_for(o, x, y, type, power);
	return(TRUE);
      }
    }

    else if (in_radius(o->x+16-x, o->y+16-y, o->r) &&
	     determine_hit(o, type, owner))
     hit_obj_for(o, x, y, type, power);
  }

  return(FALSE);
}

long check_obj_hit_radius(objectstruct *owner, long x, long y, long r, long type, long power)
{ objectstruct *o;
  long h=FALSE;

  for (o=obj; o!=NULL; o=o->next)
   if (o!=owner && o->r!=0 && in_radius(o->x+16-x, o->y+16-y, r>o->r? r:o->r) &&
       determine_hit(o, type, owner))
   { hit_obj_for(o, x, y, type, power);
     h=TRUE;
   }

  return(h);
}

static long in(long v, long v1, long v2)
{ if (v1<=v2) return(v1<=v && v<=v2);
  else return(v2<=v && v<=v1);
}

long line_obj_hit_point(objectstruct *owner, long x1, long y1, long *x2, long *y2, long dir, long type, long power)
{ long rx, ry, x3, y3, x4, y4, d, c;
  objectstruct *o;
  float a, t;
  unsigned char *p;

  rx=*x2-x1, ry=*y2-y1;
  d=abs(abs(rx)>abs(ry)? rx:ry);
  rx=sign(rx), ry=sign(ry);

  a=(float)(*y2-y1)/(*x2-x1+1E-8);

  for (o=obj; o!=NULL; o=o->next)
  { if (o==owner || o->r==0) continue;

    if (o->type==TANK_TYPE)
    { if (o->h<1 && type!=HT_ROCKET) continue;

      x3=x1-o->x, y3=y1-o->y;
      x4=x1, y4=y1;

      p=tnk.tank->pic[tnk.motion.dir];

      for (c=d; c>=0; c--)
      { x3+=rx, y3+=ry, x4+=rx, y4+=ry;

	if (x3>=0 && x3<b_size && y3>=0 && y3<b_size && *(p+x3+y3*b_size)!=0xff)
	{ hit_obj_for(o, x4, y4, type, power);
	  tank_add_impulse(o, a16torad(dir), power);

	  *x2=x4, *y2=y4;

	  return(TRUE);
	}
      }
    }
    else
    { x3=o->x+16, y3=o->y+16;

      t=(float)(x3-x1+a*(y3-y1))/(1+sqr(a));

      if (!in(x1+t, x1, *x2) || !in(y1+a*t, y1, *y2) ||
	  !in_radius(x1+t-x3, y1+a*t-y3, o->r))
       continue;

      *x2=x1+t, *y2=y1+a*t;

      if (determine_hit(o, type, owner))
      { hit_obj_for(o, *x2, *y2, type, power);
	return(TRUE);
      }
    }
  }

  return(FALSE);
}

long line_obj_hit_radius(objectstruct *owner, long x1, long y1, long *x2, long *y2, long r, long type, long power)
{ objectstruct *o;
  long h=FALSE, x3, y3;
  float a, t;

  a=(float)(*y2-y1)/(*x2-x1+1E-8);

  for (o=obj; o!=NULL; o=o->next)
  { if (o==owner || o->r==0)
     continue;

    x3=o->x+16, y3=o->y+16;

    t=(float)(x3-x1+a*(y3-y1))/(1+sqr(a));

    if (!in(x1+t, x1, *x2) || !in(y1+a*t, y1, *y2) ||
	!in_radius(x1+t-x3, y1+a*t-y3, r>o->r? r:o->r))
     continue;

    *x2=x1+t, *y2=y1+a*t;

    if (determine_hit(o, type, owner))
    { hit_obj_for(o, *x2, *y2, type, power);
      h=TRUE;
    }
  }

  return(h);
}

void hit_obj_for(objectstruct *o, long x, long y, long type, long power)
{ if (type!=HT_ABOMB && o->type!=MINE_TYPE)
   create_effect(x-16, y-16, 5, hit[type].explosion);

  switch(type)
  { case HT_CHAIN:
     if (rnd(20)==0)
      create_sound_absolute(o->x, o->y, SND_RICOCHETS+rnd(6));
     break;

    case HT_TANK_CRASH:
    case HT_ABOMB:
    case HT_CRASH:
    case HT_FLAME:
    case HT_MORTAR:
    case HT_LAVA:
     break;

    default:
     create_sound_absolute(o->x, o->y, SND_EXPLODE+rnd(2));
     break;
  }

  if (o->funct.hit!=NULL)
   o->funct.hit(o, power, type);
}
