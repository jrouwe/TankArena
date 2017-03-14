/* Mission codes */

#include <edit.h>

void thiscode_fun(void)
{ read_string("THIS MISSION: ", map.this_mission_code, 21);

  misscode_mnu[0].s=-1;
}

void nextcode_fun(void)
{ read_string("NEXT MISSION: ", map.next_mission_code, 21);

  misscode_mnu[0].s=-1;
}

void misspubl_fun(void)
{
  map.public_pwd = misspubl_mnu[0].s == 0;
}

void missshare_fun(void)
{
  map.shareware = missshare_mnu[0].s == 0;
}
