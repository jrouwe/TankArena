/* View the whole map */

#include <edit.h>
#include <string.h>

#define size_x	    200
#define size_y      200

void view_map(void)
{
  long x, y, k, b, bsx, bsy, block, nblock, size;
  objectstruct *o;

  show_marker(-1, -1);

  set_window_size(buf_x, buf_y);

  bsx = size_x/map.sx+1;
  bsy = size_y/map.sy+1;

  if (bsx<bsy)
    block=bsx, nblock=map.sx, size=size_x;
  else
    block=bsy, nblock=map.sy, size=size_y;

  memset(scr, 0, size_x*size_y);

  for (y=0; y<map.sy; y++)
    for (x=0; x<map.sx; x++)
    {
      if (strength[0][0][x+map.sx*y]!=0 || strength[1][0][x+map.sx*y]!=0)
       b_bar((x*size)/nblock, (y*size)/nblock, block, block, cmap[0x18]);
      else if (mapm.m[0][x+map.sx*y]!=-1)
       b_bar((x*size)/nblock, (y*size)/nblock, block, block, cmap[0x14]);
    }

  for (o=obj; o!=NULL; o=o->next)
  {
    x=(((o->x+16)/b_size)*size)/nblock;
    y=(((o->y+16)/b_size)*size)/nblock;

    switch(o->type)
    {
      case TANK_TYPE:
	if (o->d.tk.shield.armor==0)
	  break;

	if (o->d.tk.contr.computer)
	  goto write_red_dot;
      
	  b_write_text(x, y, cmap[15], "%ld", o->d.tk.contr.id);

	break;

      case B52_TYPE:
	if (o->d.b52.armor!=0)
	  goto write_red_dot;

	break;

      case TRAIN_TYPE:
      case WAGON_TYPE:
	if (o->d.tr.gen.armor!=0)
	  goto write_red_dot;

	break;
		     
      case TURRET_TYPE:

	write_red_dot:;
	  b_bar(x, y, block, block, cmap[0x58]);

	break;
    }
  }

  blit(scrbuf, screen, 0, 0, 0, 0, size_x, size_y);

  do
  { k=scan_key();
    b=get_mouse(&x, &y);
    show_marker(x, y);

  } while ((k&0x80)!=0 && b==0);

  show_marker(-1, -1);

  if (b==1 && x<size_x)
  {
    if (map.sx>map.sy)
      x=(x*map.sx)/size_x, y=(y*map.sx)/size_y;
    else
      x=(x*map.sy)/size_x, y=(y*map.sy)/size_y;

     x-=2, y-=2;

     if (x<0) x=0;
     if (x>max_x) x=max_x;

     if (y<0) y=0;
     if (y>max_y) y=max_y;

     smx=x, smy=y;
  }

  while (get_mouse(&x, &y)!=0);
}
