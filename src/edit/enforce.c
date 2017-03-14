/* Place enforcers */

#include <edit.h>

#define e ao->d.enf

void enforcer_setup_mnu(void)
{
  enfprop_mnu[0].value = &e.enf_good;
  enfprop_mnu[1].value = &e.enf_evil;
  enfprop_mnu[2].value = &e.change;
  enfprop_mnu[3].value = &e.fire;
  enfprop_mnu[4].value = &e.r;
  enfprop_mnu[5].value = &e.delay;

  enfweap_mnu[0].s = e.weapon;
}

void create_enforcer(long x, long y)
{
  ao = make_enforcer(smx*b_size+x-16, smy*b_size+y-16);

  active_mnu = enforce_mnu;
  enforce_mnu[0].s = 0;

  enforcer_setup_mnu();
}

void enfpos_fun(void)
{
  click_left = new_position;
}

void createenf_fun(void)
{
  click_left = create_enforcer;
}

void enfprop_fun(void)
{
  updown_mnu = enfprop_mnu;
}

void enfweap_fun(void)
{
  e.weapon = enfweap_mnu[0].s;
}
