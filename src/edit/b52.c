/* Edit b52's */

#include <edit.h>

void b52_new_direction(long x, long y)
{ ao->d.b52.dir=(ao->d.b52.dir+1)&3;
}

void b52pos_fun(void)
{ click_left=new_position;
  click_right=b52_new_direction;
}

void add_b52(long x, long y)
{ ao=make_b52(smx*b_size+x-16, smy*b_size+y-16, 0);

  active_mnu=b52_mnu;
  b52_mnu[0].s=0;
}

void addb52_fun(void)
{ click_left=add_b52;
}
