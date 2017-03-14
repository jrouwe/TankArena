/* When you have not registered you get a nice zeppelin flying over
   your screen.
*/

#include <tank.h>

extern unsigned char *zeppelin[8*4];

#define z	o->d.zep
#define vzep	10

static void write_zeppelin(objectstruct *o)
{ long x=o->x-4*b_size, y=o->y-2*b_size, h=o->h, j, k, l;

  for (l=z.img*4*8, k=y; k<y+4*b_size; k+=b_size)
   for (j=x; j<x+8*b_size; j+=b_size, l++)
   {
     b_add_shadow(j-ssx+h, k-ssy+2*h, zeppelin[l]);
     b_add_layer(j-ssx, k-ssy, zeppelin[l]);
   }
}

static void control_zeppelin(objectstruct *o)
{
  add_vector(&o->fx, &o->fy, z.vx, 0, &z.count);
  
  if (o->x < -4 * b_size)
  {
    o->fx = (map.sx + 4) * b_size;
    o->fy = rnd(map.sy * b_size);
  }

  o->x = pround(o->fx), o->y = pround(o->fy);

  if (tdone(z.nimg))
  {
    z.img ^= 1;
    z.nimg = tinit(100);
  }
}

objectstruct *make_zeppelin(void)
{ objectstruct *o;

  o=create_object(ZEPPELIN_TYPE, (map.sx + 4) * b_size, rnd(map.sy * b_size), 20, 20);

  o->funct.control=control_zeppelin;
  o->funct.write=write_zeppelin;

  z.count = timer;

  z.vx = -vzep;

  z.img = 0;
  z.nimg = tinit(100);

  return(o);
}
