/* Place locks */

#include <edit.h>

#define l ao->d.lo

void lock_setup_mnu(void)
{
  lcksrc_mnu[0].s=l.lock_activate;
  lcktrg_mnu[0].s=l.lock_target;
}

void create_lock(long x, long y)
{
  ao=make_lock(smx*b_size+x-16, smy*b_size+y-16);

  active_mnu=lock_mnu;
  lock_mnu[0].s = 0;

  lock_setup_mnu();
}

void lock_position(long x, long y)
{
  l.x=smx*b_size+x-16;
  l.y=smy*b_size+y-16;
}

void eddlock_fun(void)
{
  click_left=lock_position;
  click_right=new_position;
}

void createlock_fun(void)
{
  click_left=create_lock;
}

void lcksrc_fun(void)
{
  l.lock_activate = lcksrc_mnu[0].s;
}

void lcktrg_fun(void)
{
  l.lock_target = lcktrg_mnu[0].s;
}
