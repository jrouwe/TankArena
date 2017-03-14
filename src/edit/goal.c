/* Functions to edit goals */

#include <edit.h>

/******************** STRUCT GOALS SUB FUNCTIONS *****************************/

void increase_goal10(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.g[x+map.sx*y]+=10;

  if (mapm.g[x+map.sx*y]>100)
   mapm.g[x+map.sx*y]=100;
}

void decrease_goal10(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.g[x+map.sx*y]-=10;

  if (mapm.g[x+map.sx*y]<-100)
   mapm.g[x+map.sx*y]=-100;
}

void goal_fun10(void)
{ click_left=increase_goal10;
  click_right=decrease_goal10;
  write_left_fun=write_left_with_struct_goals;
}

void increase_goal1(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.g[x+map.sx*y]++;

  if (mapm.g[x+map.sx*y]>100)
   mapm.g[x+map.sx*y]=100;
}

void decrease_goal1(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.g[x+map.sx*y]--;

  if (mapm.g[x+map.sx*y]<-100)
   mapm.g[x+map.sx*y]=-100;
}

void goal_fun1(void)
{ click_left=increase_goal1;
  click_right=decrease_goal1;
  write_left_fun=write_left_with_struct_goals;
}

/******************** OBJECT GOALS SUB FUNCTIONS *****************************/

objectstruct *closest_goal(long x, long y)
{ objectstruct *o, *f=NULL;
  long d1=20, d2;

  x+=smx*b_size;
  y+=smy*b_size;

  for (o=obj; o!=NULL; o=o->next)
  { d2=distance(o->x+16-x, o->y+16-y);

    if (d2<d1)
     switch(o->type)
     { case TANK_TYPE:
       case B52_TYPE:
       case TRAIN_TYPE:
       case WAGON_TYPE:
       case MINE_TYPE:
       case LOCK_TYPE:
       case GOAL_TYPE:
	f=o, d1=d2;
	break;
     }
  }
  
  return(f);
}

void increase_ogoal10(long x, long y)
{ if ((ao=closest_goal(x, y))==NULL)
   return;

  ao->gc+=10;

  if (ao->gc>100)
   ao->gc=100;

  ao=NULL;
}

void decrease_ogoal10(long x, long y)
{ if ((ao=closest_goal(x, y))==NULL)
   return;

  ao->gc-=10;

  if (ao->gc<-100)
   ao->gc=-100;

  ao=NULL;
}

void ogoal_fun10(void)
{ click_left=increase_ogoal10;
  click_right=decrease_ogoal10;
  write_left_fun=write_left_with_obj_goals;
}

void increase_ogoal1(long x, long y)
{ if ((ao=closest_goal(x, y))==NULL)
   return;

  ao->gc++;

  if (ao->gc>100)
   ao->gc=100;

  ao=NULL;
}

void decrease_ogoal1(long x, long y)
{ if ((ao=closest_goal(x, y))==NULL)
   return;

  ao->gc--;

  if (ao->gc<-100)
   ao->gc=-100;

  ao=NULL;
}

void ogoal_fun1(void)
{ click_left=increase_ogoal1;
  click_right=decrease_ogoal1;
  write_left_fun=write_left_with_obj_goals;
}
