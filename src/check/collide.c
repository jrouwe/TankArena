/* Checks if objects collide. Many different cases give many different
   collisions.
*/

#include <tank.h>

#define tnk o->d.tk
#define crash_power 2

void check_obj_overlap(void)
{ objectstruct *o, *p;

  for (o=obj; o!=NULL; o=o->next)
   if (o->type==TANK_TYPE)
    for (p=o->next; p!=NULL; p=p->next)
     if (p->type==TANK_TYPE && abs(p->h-o->h)<2 && in_radius(o->x-p->x, o->y-p->y, o->r+p->r))
     { if (tnk.shield.armor>0 && tnk.shield.icount==0)
	hit_obj_for(o, o->x+10+rnd(12), o->y+10+rnd(12), HT_DESTROY, 1000);

       if (tnk.shield.armor>0 && p->d.tk.shield.icount==0)
	hit_obj_for(p, p->x+10+rnd(12), p->y+10+rnd(12), HT_DESTROY, 1000);
     }
}

long check_obj_collide(objectstruct *o, long x, long y)
{ long ret=FALSE, r;
  objectstruct *p, *n;

  for (p=obj; p!=NULL; p=n)
  {
    n = p->next;

    r=p->type==MINE_TYPE && p->d.mi.type==HEAVY_MINE? 50:p->r;

    if (o!=p && p->r!=0 && (abs(p->h-o->h)<2 || p->type==FLAG_TYPE) && in_radius(x-p->x, y-p->y, o->r+r))
     switch(p->type)
     { case MINE_TYPE:
	if (tdone(p->d.mi.active))
	 p->funct.control=blowup_mine;
	break;

       case MAN_TYPE:
	remove_object(p);
	break;

       default:
	switch(o->type)
	{ case TANK_TYPE:
	   switch(p->type)
	   { case TANK_TYPE:
	      if (tdone(tnk.motion.nhit) && tnk.tank->general.crash>4)
	      { hit_obj_for(p, (x+p->x)/2+rnd(4)+14, (y+p->y)/2+rnd(4)+14, HT_TANK_CRASH, crash_power);
		tnk.motion.nhit=tinit(10);
	      }

	      if (tnk.contr.computer && (o->target!=p || !tdone(tnk.computer.wave)))
	      { tnk.computer.crashed=tinit(4*tnk.tank->motion.rotation);
		tnk.computer.crashed2=tinit(200);
	      }

	      tanks_collide(o, p);
	      ret=TRUE;
	      break;

	     case TRAIN_TYPE:
	     case WAGON_TYPE:
	     case B52_TYPE:
	      static_collide(o);
	      ret=TRUE;
	      break;

	     case BONUS_TYPE:
	      if (tnk.tank->type==TANK || tnk.tank->type==CAR)
	       pickup_bonus(o, p);
	      break;

             case FLAG_TYPE:
	      if (p->target==NULL && tnk.shield.armor>0)
	      { create_message(o, "GOT A FLAG!", "");
		p->target=o, tnk.und.flags++;
              }
              break;

	     case PRODUCT_TYPE:
	      if (p->target==NULL && !tnk.contr.computer)
		pickup_product(o, p);
	      break;
	   }
	   break;

	  case B52_TYPE:
	   switch(p->type)
	   { case TANK_TYPE:
	     case B52_TYPE:
	     case TRAIN_TYPE:
	     case WAGON_TYPE:
	      hit_obj_for(o, (x+p->x)/2+rnd(4)+14, (y+p->y)/2+rnd(4)+14, HT_DESTROY, 1000);
	      hit_obj_for(p, (x+p->x)/2+rnd(4)+14, (y+p->y)/2+rnd(4)+14, HT_DESTROY, 1000);
	      ret=TRUE;
	      break;
	   }
	   break;

	  case TRAIN_TYPE:
	  case WAGON_TYPE:
	   switch(p->type)
	   { case TANK_TYPE:
	      hit_obj_for(p, (x+p->x)/2+rnd(4)+14, (y+p->y)/2+rnd(4)+14, HT_DESTROY, 1000);
	      ret=TRUE;
	      break;

	     case TRAIN_TYPE:
	     case WAGON_TYPE:
	      if (o->d.tr.info.front!=p && o->d.tr.info.rear!=p)
	      { remove_object(p);
		ret=TRUE;
	      }
	      break;
	   }
	   break;
	}
     }
  }

  return(ret);
}
