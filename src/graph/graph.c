/* Initialisation of graphic mode and some general drawing routines for use
   in the map editor.
*/

#include <tank.h>
#include <math.h>

void init_graph(void)
{
  if (set_gfx_mode(graph_drv, res_x, res_y, res_x, num_pages * res_y) != 0)
    error("Unable to set selected graphics mode.\n");

  set_clip(screen, 0, 0, res_x, num_pages * res_y);

  set_palette(pal);
}

void draw_line(long x1, long y1, long x2, long y2, long color)
{
  float dx=x2-x1, dy=y2-y1, x, y;
  long d;

  d=distance(dx, dy)/5;

  if (d<1) return;

  dx/=d, dy/=d;
  x=x1-ssx+16, y=y1-ssy+16;

  for (; d>=0; d--, x+=dx, y+=dy)
   b_pixel((long)x, (long)y, color);
}

void draw_circle(long x, long y, long r, long color)
{
  float t;

  if (r>0)
  { x-=ssx-16, y-=ssy-16;

    for (t=0; t<2*PI; t+=(float)5/r)
     b_pixel((long)(x+r*cos(t)), (long)(y-r*sin(t)), color);
  }
}
