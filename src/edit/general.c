/* Some general functions that can be reused */

#include <edit.h>

void done_fun(void)
{ ao=NULL;
  active_mnu=edit_mnu;
  execute_smart_menu();
}

void new_position(long x, long y)
{ ao->x=smx*b_size+x-16;
  ao->y=smy*b_size+y-16;
  ao->fx=ao->x;
  ao->fy=ao->y;
}

void map_setup_menu(void)
{ prev_wagon=NULL;

  time_mnu[0].s=map.night? 1:0;
  seltype_mnu[0].s=map.map_type;
  bonustype_mnu[0].s=map.random_bonus? 0:1;
  misspubl_mnu[0].s=map.public_pwd? 0:1;
  missshare_mnu[0].s=map.shareware? 0:1;

  smx=0, smy=0, stx=0, sty=0;
}

void time_fun(void)
{ map.night=time_mnu[0].s==1;
}

void seltype_fun(void)
{ map.map_type=seltype_mnu[0].s;
}
