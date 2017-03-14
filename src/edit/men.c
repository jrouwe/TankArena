/* Place men in structures */

#include <edit.h>

void incman_fun(long x, long y)
{ long type;

  x=smx+x/b_size, y=smy+y/b_size;

  switch(menstruct_mnu[0].s)
  { case 0:
     type=HT_CHAIN;
     break;

    case 1:
     type=HT_FLAME;
     break;

    default:
     type=HT_SROCK;
     break;
  }

  if ((mapm.man[0][x+map.sx*y]==type || mapm.man[1][x+map.sx*y]==0) && mapm.man[1][x+map.sx*y]<50)
   mapm.man[1][x+map.sx*y]++;

  mapm.man[0][x+map.sx*y]=type;
}

void decman_fun(long x, long y)
{ x=smx+x/b_size, y=smy+y/b_size;

  if (mapm.man[1][x+map.sx*y]>0)
   mapm.man[1][x+map.sx*y]--;
}

void incdecman_fun(void)
{ click_left=incman_fun;
  click_right=decman_fun;
  write_left_fun=write_left_with_men;
}
