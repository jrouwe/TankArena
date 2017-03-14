/* Place warps */

#include <edit.h>

void create_warp(long x, long y)
{ ao=make_warp(smx*b_size+x-16, smy*b_size+y-16);

  active_mnu=warp_mnu;
  warp_mnu[0].s=0;
}

void warp_entry(long x, long y)
{ ao->x=smx*b_size+x-16;
  ao->y=smy*b_size+y-16;
}

void warp_exit(long x, long y)
{ ao->d.wa.x=smx*b_size+x-16;
  ao->d.wa.y=smy*b_size+y-16;
}

void warp_fun(void)
{ click_left=warp_exit;
  click_right=warp_entry;
}

void createwarp_fun(void)
{ click_left=create_warp;
}
