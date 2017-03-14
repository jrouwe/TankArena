/* Place mines */

#include <edit.h>

void add_mine(long x, long y)
{ create_mine(smx*b_size+x-16, smy*b_size+y-16, LIGHT_MINE, 7);
}

void add_rmine(long x, long y)
{ create_mine(smx*b_size+x-16, smy*b_size+y-16, HEAVY_MINE, 7);
}

void addmine_fun(void)
{ click_left=add_mine;
}

void addrmine_fun(void)
{ click_left=add_rmine;
}
