/* The object: destroyer. Destroys large are if something happens, nice
   for making missions.
*/

#include <tank.h>

#define d o->d.dest

static void mwrite_destroyer(objectstruct *o, objectstruct *ao)
{
  draw_circle(o->x, o->y, d.r, 95);
  b_write_text(o->x-ssx+4, o->y-ssy+14, 95, "DEST");
}

static void hit_destroyer(objectstruct *o, long power, long type)
{
  remove_object(o);
}		   

static void remove_destroyer(objectstruct *o)
{
  if ((d.what & 1) != 0)
    hit_wall_big(o, o->x+16, o->y+16, d.r, 1, HT_DESTROY, 1000);

  if ((d.what & 2) != 0)
    check_obj_hit_radius(o, o->x+16, o->y+16, d.r, HT_DESTROY, 1000);
}

objectstruct *make_destroyer(long x, long y)
{
  objectstruct *o;

  o = create_object(DESTROYER_TYPE, x, y, 24, 0);

  o->funct.mwrite = mwrite_destroyer;
  o->funct.hit = hit_destroyer;
  o->funct.remove = remove_destroyer;
  o->funct.control = d.immediate? remove_object:0;

  d.r = 30;
  d.immediate = TRUE;
  d.what = 3;

  return(o);
}

void prepare_destroyer(objectstruct *o)
{
  o->funct.mwrite = mwrite_destroyer;
  o->funct.hit = hit_destroyer;
  o->funct.remove = remove_destroyer;
  o->funct.control = d.immediate? remove_object:0;
}

