/* The game radar. Writes red dots for enemies, white dots for friends,
   yellow dots for missiles and a black dot for the player itself.
*/

#include <tank.h>
#include <string.h>
#include <math.h>

#define tnk q->d.tk

#define range	    15

#define BLACK	    0
#define RED	    95
#define WHITE	    15
#define YELLOW	    64

void write_radar(objectstruct *q)
{	    
  long size = res_x / 6, r = size / 2;
  long bs = r / range + 1;
  long start_x = buf_x - bs - r;
  long start_y = buf_y - bs - r;
  objectstruct *o;
  long x, y, col;

  for (o = obj; o != NULL; o = o->next)
    if (in_radius(o->x - q->x, o->y - q->y, range * b_size))
    {
      x = start_x + ((o->x - q->x) * r) / (range * b_size);
      y = start_y + ((o->y - q->y) * r) / (range * b_size);
      col = -1;

      switch(o->type)
      {
	case TANK_TYPE:
	  if (o->d.tk.shield.armor == 0 || o->d.tk.invis.count != 0)
	    break;

	  if (game.mode == DUAL)
	    {
	      if (o == q)
		col = BLACK;
	      else if (tnk.contr.num == o->d.tk.contr.num)
		col = WHITE;
	      else
		col = RED;
	    }
	  else
	    {
	      if (o == q)
		col = BLACK;
	      else if (o->d.tk.contr.computer && !o->d.tk.contr.good)
		col = RED;
	      else
		col = WHITE;
	    }

	  break;

	case B52_TYPE:
	  if (o->d.b52.armor != 0)
	    col = RED;

	  break;

	case TRAIN_TYPE:
	case WAGON_TYPE:
	  if (o->d.tr.gen.armor != 0)
	    col = RED;

	  break;
		     
	case TURRET_TYPE:

	  col = RED;

	  switch(o->d.tu.shootat)
	  { case 1:
	      if (!tnk.contr.good)
		col = WHITE;
	      break;

	    case 2:
	      if (tnk.contr.good)
		col = WHITE;
	      break;

	    case 3:
	      if (tnk.contr.num==0)
		col = WHITE;
	      break;

	    case 4:
	      if (tnk.contr.num==1)
		col = WHITE;
	      break;
	  }
	  break;

	case ROCKET_TYPE:

	  col = YELLOW;

	  break;
      }

      if (col != -1)
	b_bar(x, y, bs, bs, cmap[col]);
    }
}
