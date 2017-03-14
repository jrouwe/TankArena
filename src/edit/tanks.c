/* Functions to edit tanks in the map */

#include <edit.h>

void tank_setup_mnu(void)
{ long i;

  if (tnk.contr.computer)
   contr_mnu[0].s=tnk.contr.good? 1:2;
  else contr_mnu[0].s=0;

  for (i=0; i<11; i++)
   weapon_mnu[i].value=&tnk.weap.nw[i>1? i+1:i];

  id_mnu[0].value=&tnk.contr.id;
 
  activate_mnu[0].value=&tnk.computer.active;
  activate_mnu[1].value=&tnk.computer.stay_active;
  activate_mnu[2].value=&tnk.computer.activator;
  activate_mnu[3].value=&tnk.computer.activation_rad;

  target_mnu[0].value=&tnk.computer.attack_tanks;
  target_mnu[1].value=&tnk.computer.attack_goals;

  fire_mnu[0].value=&tnk.weap.power[tnk.tank->weap.initial_weap];
  fire_mnu[1].value=&tnk.computer.fire_delay;
  fire_mnu[2].value=&tnk.computer.attack_at;
  fire_mnu[3].value=&tnk.computer.wave_delay;

  waypoint_mnu[0].s=tnk.way.type;
}

void deact_way_fun(void)
{ tnk.way.type=WAY_INACTIVE;
}

void act_way_fun(void)
{ tnk.way.type=WAY_ACTIVE;
}

void circ_way_fun(void)
{ tnk.way.type=WAY_CIRCULAR;
}

void edit_waypoint(long x, long y)
{ x+=smx*b_size-16;
  y+=smy*b_size-16;

  tnk.way.point[eddway_mnu[0].s].x=x;
  tnk.way.point[eddway_mnu[0].s].y=y;
}

void remove_waypoint(long x, long y)
{ tnk.way.point[eddway_mnu[0].s].x=-1;
}

void eddway_fun(void)
{ click_left=edit_waypoint;
  click_right=remove_waypoint;
}

void activate_fun(void)
{ updown_mnu=activate_mnu;
}

void target_fun(void)
{ updown_mnu=target_mnu;
}

void fire_fun(void)
{ updown_mnu=fire_mnu;
}

void find_free_id(void)
{ objectstruct *o;
  long id;

  for (id=1; id<10; id++)
  { for (o=obj; o!=NULL; o=o->next)
     if (o!=ao && o->d.tk.contr.id==id)
      break;

    if (o==NULL)
     break;
  }

  tnk.contr.id=id;
}

void player_fun(void)
{ tankstruct *t=&tnk;

  if (t->contr.computer)
   find_free_id();

  t->contr.good=TRUE;
  t->contr.computer=FALSE;
  tnk.contr.num=1;

  updown_mnu=id_mnu;
}

void good_fun(void)
{ tnk.contr.good=TRUE;
  tnk.contr.computer=TRUE;
  tnk.contr.id=-1;
  tnk.contr.num=-1;
}

void evil_fun(void)
{ tnk.contr.good=FALSE;
  tnk.contr.computer=TRUE;
  tnk.contr.id=-1;
  tnk.contr.num=-1;
}

void tkweapon_fun(void)
{ updown_mnu=weapon_mnu;
}

void clweapon_fun(void)
{ long i;

  for (i=0; i<num_weapon; i++)
   tnk.weap.nw[i]=0;

  tkweapon_mnu[0].s=0;
  updown_mnu=weapon_mnu;
}

void tk_new_direction(long x, long y)
{ tnk.motion.dir=(tnk.motion.dir+1)&15;
  tnk.motion.ldir=(tnk.motion.ldir+1)&15;
}

void tkpos_fun(void)
{ click_left=new_position;
  click_right=tk_new_direction;
}

void add_usertank(long x, long y)
{ x+=smx*b_size-16;
  y+=smy*b_size-16;

  ao=make_tank(NULL, usertank_mnu[0].s, x, y, 0, 1);
  find_free_id();

  active_mnu=tank_mnu;
  tank_mnu[0].s=0;

  tank_setup_mnu();
}

void usertank_fun(void)
{ click_left=add_usertank;
}

void add_comptank(long x, long y)
{ x+=smx*b_size-16;
  y+=smy*b_size-16;

  ao=make_tank(NULL, 8+comptank_mnu[0].s, x, y, 0, -1);

  active_mnu=tank_mnu;
  tank_mnu[0].s=0;

  tank_setup_mnu();
}

void comptank_fun(void)
{ click_left=add_comptank;
}

void add_supermarkettank(long x, long y)
{ x+=smx*b_size-16;
  y+=smy*b_size-16;

  ao=make_tank(NULL, 22+supermarkt_mnu[0].s, x, y, 0, 1);
  find_free_id();

  active_mnu=tank_mnu;
  tank_mnu[0].s=0;

  tank_setup_mnu();
}

void supermarkt_fun(void)
{ click_left=add_supermarkettank;
}
