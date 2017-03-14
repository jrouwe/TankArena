/* Make trains. This function could be made a little more fool proof, but
   then again, users should pay attention to what they're doing.
*/

#include <edit.h>

void train_setup_mnu(void)
{
  if (ao->d.tr.info.turret==NULL)
    train_mnu[1].attr = DISABLED;
  else
    train_mnu[1].attr = ENABLED;
}

void tredtur_fun(void)
{ if (ao->d.tr.info.turret!=NULL)
  { ao=ao->d.tr.info.turret;

    active_mnu=tur_mnu;
    tur_mnu[0].s=0;

    tur_setup_mnu();

  } else train_mnu[0].s=0;
}

void tr_new_pos(long x, long y)
{ prev_wagon->d.tr.info.x=smx+x/b_size;
  prev_wagon->d.tr.info.y=smy+y/b_size;
  train_change(prev_wagon);
}

void tr_new_dir(long x, long y)
{ ao->d.tr.motion.dir=(ao->d.tr.motion.dir+4)&15;
  train_change(ao);
}

void trpos_fun(void)
{ click_left=tr_new_pos;
  click_right=tr_new_dir;
}

void add_loco(long x, long y)
{ x=smx+x/b_size, y=smy+y/b_size;

  if (addtrain_mnu[0].s!=0)
   ao=make_train(x, y, addwagon_mnu[0].s+1, prev_wagon);
   else ao=make_train(x, y, 0, NULL);

  prev_wagon=ao;

  active_mnu=train_mnu;
  train_mnu[0].s=0;

  train_setup_mnu();
}

void addloco_fun(void)
{ click_left=add_loco;
}

