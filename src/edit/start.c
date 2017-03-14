/* Player start point */

#include <edit.h>

void player_setup_mnu(void)
{ long i;

  modweap_mnu[0].s=ao->d.pl.weap.active? 0:1;

  for (i=0; i<11; i++)
   weapon_mnu[i].value=&ao->d.pl.weap.nw[i>1? i+1:i];
}

void pl_new_direction(long x, long y)
{ ao->d.pl.dir=(ao->d.pl.dir+1)&3;
}

void startpos_fun(void)
{ click_left=new_position;
  click_right=pl_new_direction;
}

void create_start(long x, long y)
{ x=smx*b_size+x-16, y=smy*b_size+y-16;

  for (ao=obj; ao!=NULL; ao=ao->next)
   if (ao->type==PLAYER_TYPE && ao->d.pl.type==start_mnu[0].s)
    break;

  if (ao==NULL) ao=make_player(x, y, start_mnu[0].s, 0);
   else ao->x=x, ao->y=y;

  active_mnu=player_mnu;
  player_mnu[0].s=0;

  player_setup_mnu();
}

void start_fun(void)
{ click_left=create_start;
}

void plnoweap_fun(void)
{ ao->d.pl.weap.active=FALSE;
}

void plweap_fun(void)
{ ao->d.pl.weap.active=TRUE;
  updown_mnu=weapon_mnu;
}
