/* Functions to place flags */

#include <edit.h>

void flagpos_fun(void)
{
  click_left=new_position;
}
                             
void add_flag(long x, long y)
{
  x = smx*b_size+x-16, y = smy*b_size+y-16;

  ao = make_flag(x, y, start_mnu[0].s-8);

  active_mnu = flag_mnu;
  flag_mnu[0].s = 0;
}

void addflag_fun(void)
{
  click_left=add_flag;
}
