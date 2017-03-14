/* Place turrets */

#include <edit.h>

#define tur ao->d.tu

void tur_setup_mnu(void)
{ shootat_mnu[0].s=tur.shootat;
  turprops_mnu[0].value=&tur.power;
  turprops_mnu[1].value=&tur.radius;
  turprops_mnu[2].value=&tur.delay;
  turprops_mnu[3].value=&tur.fixed;
}

void tur_new_direction(long x, long y)
{ tur.dir=(tur.dir+1)&15;
}

void turpos_fun(void)
{ click_left=new_position;
  click_right=tur_new_direction;
}

void shootat_fun(void)
{ tur.shootat=shootat_mnu[0].s;
}

void turprops_fun(void)
{ updown_mnu=turprops_mnu;
}

void add_turret(long x, long y)
{ ao=make_turret(smx*b_size+x-16, smy*b_size+y-16, 0, 4, addtur_mnu[0].s, NULL);

  active_mnu=tur_mnu;
  tur_mnu[0].s=0;

  tur_setup_mnu();
}

void addtur_fun(void)
{ click_left=add_turret;
}

void tur_forbid_toggle(long x, long y)
{
  char dir = radtoa16(angle(smx*b_size+x-16-ao->x, smy*b_size+y-16-ao->y));

  tur.forbid_dir ^= (1 << dir);
}

void turforbid_fun(void)
{
  click_left = tur_forbid_toggle;
}
