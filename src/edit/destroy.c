/* Place destroyer objects */

#include <edit.h>

#define d ao->d.dest

void destroyer_setup_mnu(void)
{
  destprop_mnu[0].value = &d.immediate;
  destprop_mnu[1].value = &d.r;

  destwhat_mnu[0].s = d.what - 1;
}

void create_destroyer(long x, long y)
{
  ao = make_destroyer(smx*b_size+x-16, smy*b_size+y-16);

  active_mnu = destroy_mnu;
  destroy_mnu[0].s = 0;

  destroyer_setup_mnu();
}

void destpos_fun(void)
{
  click_left = new_position;
}

void createdest_fun(void)
{
  click_left = create_destroyer;
}

void destprop_fun(void)
{
  updown_mnu = destprop_mnu;
}

void destwhat_fun(void)
{
  d.what = destwhat_mnu[0].s + 1;
}
