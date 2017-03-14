/* The products for supermarket mayhem. */

#include <tank.h>

#define p o->d.pr

int numprod[num_products];

static char *prodname[num_products]={
    "WASHING POWDER",
    "CHOCOLATE BAR",
    "APPLE",
    "WINE",
    "M&M'S",
    "TOILET PAPER",
    "ONION",
    "CIGARETTES",
    "COOKIES",
    "BEER",
    "COLA",
    "SAUSAGE",
    "BAMI",
    "CAN",
    "SPAGHETTI"
};

static void write_product(objectstruct *o)
{
  if (o->target==NULL)
    b_add_layer(o->x-ssx, o->y-ssy, intern[PRODUCTS+p.type].ptr);

  else o->x=o->target->x, o->y=o->target->y;
}

void pickup_product(objectstruct *t, objectstruct *o)
{
  game.mayhem_cash -= p.price;

  if (game.mayhem_cash < 0)
  {
    game.mayhem_cash = 0;
    modify_goal_counter(-100);
  }

  if (game.mayhem_products > 0)
    game.mayhem_products--;

  numprod[p.type]++;

  create_message(t, "PICKED UP", prodname[p.type]);

  remove_object(o);
}

objectstruct *make_product(long x, long y, long type)
{ objectstruct *o;

  o=create_object(PRODUCT_TYPE, x, y, 0, 4);

  o->funct.write = write_product;

  p.type = type;
  p.price = 1;

  return(o);
}

void prepare_product(objectstruct *o)
{
  o->funct.write = write_product;

  game.supermarket_mayhem = TRUE;
  game.mayhem_cash = max_mayhem_cash;
  game.mayhem_products = max_mayhem_products;
}
