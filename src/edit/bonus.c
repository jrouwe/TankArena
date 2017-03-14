/* Part of the map editor to place bonusses */

#include <edit.h>

void add_bonus(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.b[x+map.sx*y]=bonus_mnu[0].s;
}

void remove_bonus(long x, long y)
{ x=smx+x/b_size;
  y=smy+y/b_size;

  mapm.b[x+map.sx*y]=-1;
}

void addbonus_fun(void)
{ click_left=add_bonus;
  click_right=remove_bonus;
  write_left_fun=write_left_with_bonus;
}

void ranbonus_fun(void)
{ map.random_bonus=bonustype_mnu[0].s==0;
}
