/* Place goals */

#include <edit.h>

void goal_setup_mnu(void)
{ goalrad_mnu[0].value=&ao->d.go.r;
  goalwho_mnu[0].s=ao->d.go.who;
}

void create_goal(long x, long y)
{ ao=make_goal(smx*b_size+x-16, smy*b_size+y-16);

  active_mnu=goalobj_mnu;
  goalobj_mnu[0].s=0;

  goal_setup_mnu();
}

void eddgoal_fun(void)
{ click_left=new_position;
}

void creategoal_fun(void)
{ click_left=create_goal;
}

void goalrad_fun(void)
{ updown_mnu=goalrad_mnu;
}

void goalwho_fun(void)
{ ao->d.go.who=goalwho_mnu[0].s;
  ao->target=NULL;
}

void select_goal(long x, long y)
{ objectstruct *o;

  if ((o=closest_object(x, y))==NULL)
   return;

  ao->target=o;
}

void goalpoint_fun(void)
{ click_left=select_goal;

  ao->d.go.who=3;
}
