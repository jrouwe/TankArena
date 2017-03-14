/* Function to edit a previously created object */

#include <edit.h>

objectstruct *closest_object(long x, long y)
{ objectstruct *o, *f=NULL;
  long d1=20, d2;
                          
  x+=smx*b_size;
  y+=smy*b_size;

  for (o=obj; o!=NULL; o=o->next)
  { d2=distance(o->x+16-x, o->y+16-y);

    if (d2<d1)
     switch(o->type)
     { case TURRET_TYPE:
        if (o->owner!=NULL) break;

       case TANK_TYPE:
       case B52_TYPE:
       case TRAIN_TYPE:
       case WAGON_TYPE:
       case WARP_TYPE:
       case LOCK_TYPE:
       case PLAYER_TYPE:
       case FLAG_TYPE:
       case GOAL_TYPE:
       case DESTROYER_TYPE:
       case ENFORCER_TYPE:
       case PRODUCT_TYPE:
        f=o, d1=d2;
        break;
     }
  }

  return(f);
}

void do_edit(void)
{ if (ao!=NULL)
   switch(ao->type)
   { case TANK_TYPE:
      active_mnu=tank_mnu;
      tank_setup_mnu();
      break;

     case B52_TYPE:
      active_mnu=b52_mnu;
      break;

     case TURRET_TYPE:
      active_mnu=tur_mnu;
      tur_setup_mnu();
      break;

     case TRAIN_TYPE:
     case WAGON_TYPE:
      active_mnu=train_mnu;
      prev_wagon=ao;
      train_setup_mnu();
      break;

     case WARP_TYPE:
      active_mnu=warp_mnu;
      break;

     case LOCK_TYPE:
      active_mnu=lock_mnu;
      lock_setup_mnu();
      break;

     case PLAYER_TYPE:
      active_mnu=player_mnu;
      player_setup_mnu();
      break;

     case FLAG_TYPE:
      active_mnu=flag_mnu;
      break;

     case GOAL_TYPE:
      active_mnu=goalobj_mnu;
      goal_setup_mnu();
      break;

     case DESTROYER_TYPE:
      active_mnu=destroy_mnu;
      destroyer_setup_mnu();
      break;

     case ENFORCER_TYPE:
      active_mnu=enforce_mnu;
      enforcer_setup_mnu();
      break;

     case PRODUCT_TYPE:
      active_mnu=product_mnu;
      product_setup_mnu();
      break;
   }

  active_mnu[0].s=0;
}

void edit_object(long x, long y)
{ if ((ao=closest_object(x, y))==NULL)
   return;

  do_edit();
}

void copy_object(long x, long y)
{ objectstruct *o;

  if ((ao=closest_object(x, y))==NULL)
   return;

  o=allocate(sizeof(objectstruct));
  memcpy(o, ao, sizeof(objectstruct));

  add_object_to_list(o);

  ao=o;

  do_edit();
}

void delete_object(long x, long y)
{
  x+=smx*b_size-16;
  y+=smy*b_size-16;

  ao = nearest_object(0, x, y);

  if (ao != 0)
    remove_object(ao);

  ao = 0;
}

void editobj_fun(void)
{ click_left=edit_object;
}

void copyobj_fun(void)
{ click_left=copy_object;
}

void remove_fun(void)
{ click_left=delete_object;
}
