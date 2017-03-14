/* Functions to place products */

#include <edit.h>		    

#define p ao->d.pr

void product_setup_mnu(void)
{
  prodtype_mnu[0].value = &p.type;
  prodprice_mnu[0].value = &p.price;
}

void prodpos_fun(void)
{
  click_left=new_position;
}

void add_prod(long x, long y)
{
  x = smx*b_size+x-16, y = smy*b_size+y-16;

  ao = make_product(x, y, 0);

  active_mnu = product_mnu;
  product_mnu[0].s = 0;

  product_setup_mnu();
}

void createprod_fun(void)
{
  click_left=add_prod;
}				    

void prodprice_fun(void)
{
  updown_mnu = prodprice_mnu;
}

void prodtype_fun(void)
{
  updown_mnu = prodtype_mnu;
}
