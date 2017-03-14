/* This is the heart of the game. It is divided in game_control() which
   controls all objects and write_screen() which writes every object
   to screen.
*/

#include <tank.h>
#include <dpmi.h>
#include <string.h>

#define tnk o->d.tk

extern long num_objects, grab;

long active_page=0, win_size=0, show_radar = TRUE;
gameinfo game;

void game_control(void)
{ static long np=tnow;

  twait(np);
  np=tinit(2);

  read_controls();
  transportation();

  control_effects();
  control_bullets();
  control_parts();
  check_obj_overlap();
  control_objects();
  palette_effects();
}

#define tn game.tn
#define o game.tk[tn]

void write_screen(void)
{ long i, x, y, start_x, start_y, nw = game.mode==SINGLE? 1:2;
  static long count=0, frames=0;
  static float speed=0;
  char *p;

  for (tn = 0; tn < nw; tn++)
  {
    if (nw == 1)
      start_x = (res_x - buf_x) / 2;
    else
      start_x = (res_x - 2 * buf_x) / 3;
    
    if (tn == 1)
      start_x += start_x + buf_x + 1;

    start_x &= 0xfffffffc;

    start_y = (res_y - buf_y) / 3;

    if (o!=NULL)
    { ssx=(!tdone(tnk.lock.fired)? tnk.lock.x:o->x)-buf_x/2+b_size/2;
      if (ssx<0) ssx=0;
      if (ssx>map.sx*b_size-buf_x) ssx=map.sx*b_size-buf_x;

      ssy=(!tdone(tnk.lock.fired)? tnk.lock.y:o->y)-buf_y/2+b_size/2;
      if (ssy<0) ssy=0;
      if (ssy>map.sy*b_size-buf_y) ssy=map.sy*b_size-buf_y;

      bottom_layer(tn, ssx, ssy);

      write_objects(NULL);

      if (!tdone(tnk.lock.fired))
       tank_write_marker(o);

      if (game.winner==(game.mode==DUAL? (tn^1):2) && !map.night)
       b_convert(scr, gray, buf_x*buf_y);
      
      if (show_radar && !game.supermarket_mayhem)
	write_radar(o);

      write_status(o);
    }
    else
    { for (i=0; i<4; i++)
       for (p=intern[STATIC+i].ptr, x=0; x<b_size*b_size; x++, p++)
	*p=rnd(16);

      for (y=0; y<buf_y; y+=b_size)
       for (x=0; x<buf_x; x+=b_size)
	b_first_layer(x, y, intern[STATIC+rnd(4)].ptr);

      write_status(o);
    }

    if (debug && tn==0)
    { if (timer-count>sec)
      { speed=(float)frames*sec/(timer-count);
	count=timer;
	frames=0;
      } else frames++;

      b_write_text(3, 24, cmap[15], "FRR: %.1f", speed);
      b_write_text(3, 30, cmap[15], "MEM: %ld", _go32_dpmi_remaining_physical_memory());
      b_write_text(3, 36, cmap[15], "OBJ: %ld", num_objects);
    }

    blit(scrbuf, screen, 0, 0, start_x, active_page*res_y+start_y, buf_x, buf_y);
  }

  if (grab)
   grab_frame();

  if (num_pages == 2)
  {
    scroll_screen(0, active_page*res_y);
    active_page^=1;
  }
}
