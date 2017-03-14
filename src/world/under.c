/* Write the underground (everything below the tanks) and the top layer
   (everything above tanks), but only the structures, not objects.
*/

#include <tank.h>
#include <sys/segments.h>
#include <sys/farptr.h>

static long start_x[2], start_y[2], rewrite[2];

static inline long in_window(long sx, long sy, long x, long y)
{ return(x>sx-b_size && x<sx+buf_x && y>sy-b_size && y<sy+buf_y);
}

void clear_layers(void)
{ rewrite[0]=TRUE;
  rewrite[1]=TRUE;
}

void compute_window_size(void)
{
  long nw = game.mode==SINGLE? 1:2;

  buf_x = (res_x - (res_x*win_size)/20)/nw - 1;
  if (map.sx*b_size<buf_x) buf_x=map.sx*b_size;

  buf_y = res_y - (res_y*win_size)/20;
  if (map.sy*b_size<buf_y) buf_y=map.sy*b_size;

  set_window_size(buf_x, buf_y);
}

void set_window_size(int x, int y)
{
  int i;

  buf_x = x, buf_y = y;

  scrbuf->w = buf_x, scrbuf->h = buf_y;

  for (i = 0; i < buf_y; i ++)
    scrbuf->line[i] = scrbuf->dat + i*buf_x;
}

void rewrite_block(long px, long py)
{ long sx, sy, spx, spy, nx, ny, nw = game.mode==SINGLE? 1:2, tn, m;

  for (tn = 0; tn < nw; tn++)
   if (in_window(start_x[tn], start_y[tn], px*b_size, py*b_size))
   { ubuf=buf[tn];

     sx=px*b_size-start_x[tn];
     if (sx<0) spx=-sx, sx=0; else spx=0;

     nx=buf_x-sx;
     if (nx+spx>b_size) nx=b_size-spx;

     sy=py*b_size-start_y[tn];
     if (sy<0) spy=-sy, sy=0; else spy=0;

     ny=buf_y-sy;
     if (ny+spy>b_size) ny=b_size-spy;

     s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

     if ((m=mapm.m[0][px+map.sx*py])!=-1)
      s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

     if ((m=mapm.m[1][px+map.sx*py])!=-1)
      s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
   }
}

void bottom_layer(long tn, long x, long y)
{ long n, sx, sy, spx, spy, nx=0, ny=0, px, py, m;

  ubuf=buf[tn];

  if (abs(start_x[tn]-x)>=buf_x || abs(start_y[tn]-y)>=buf_y || rewrite[tn])
  { for (sx=0, px=x/b_size, spx=x%b_size; sx<buf_x; sx+=nx, px++, spx=0)
    { nx=buf_x-sx;
      if (nx>b_size-spx) nx=b_size-spx;

      for (sy=0, py=y/b_size, spy=y%b_size; sy<buf_y; sy+=ny, py++, spy=0)
      { ny=buf_y-sy;
	if (ny>b_size-spy) ny=b_size-spy;

	s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

	if ((m=mapm.m[0][px+map.sx*py])!=-1)
	 s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

	if ((m=mapm.m[1][px+map.sx*py])!=-1)
	 s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
      }
    }
  }

  else
  { n=start_x[tn]-x;

    if (n<0)
    { if (-n<buf_x) s_move_left(-n);

      for (sx=buf_x+n<=0? 0:buf_x+n, px=(x+sx)/b_size; sx<buf_x; sx+=nx, px++)
      { spx=(x+sx)%b_size;

	nx=buf_x-sx;
	if (nx>b_size-spx) nx=b_size-spx;

	for (sy=0, py=start_y[tn]/b_size; sy<buf_y; sy+=ny, py++)
	{ spy=(start_y[tn]+sy)%b_size;

	  ny=buf_y-sy;
	  if (ny>b_size-spy) ny=b_size-spy;

	  s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

	  if ((m=mapm.m[0][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

	  if ((m=mapm.m[1][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
	}
      }
    }

    else if (n!=0)
    { if (n<buf_x) s_move_right(n);

      for (sx=0, px=x/b_size; sx<n && sx<buf_x; sx+=nx, px++)
      { spx=(x+sx)%b_size;

	nx=n-sx;
	if (nx>b_size-spx) nx=b_size-spx;

	for (sy=0, py=start_y[tn]/b_size; sy<buf_y; sy+=ny, py++)
	{ spy=(start_y[tn]+sy)%b_size;

	  ny=buf_y-sy;
	  if (ny>b_size-spy) ny=b_size-spy;

	  s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

	  if ((m=mapm.m[0][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

	  if ((m=mapm.m[1][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
	}
      }
    }

    n=start_y[tn]-y;

    if (n<0)
    { if (-n<buf_y) s_move_up(-n);

      for (sy=buf_y+n<=0? 0:buf_y+n, py=(y+sy)/b_size; sy<buf_y; sy+=ny, py++)
      { spy=(y+sy)%b_size;

	ny=buf_y-sy;
	if (ny>b_size-spy) ny=b_size-spy;

	for (sx=0, px=x/b_size; sx<buf_x; sx+=nx, px++)
	{ spx=(x+sx)%b_size;

	  nx=buf_x-sx;
	  if (nx>b_size-spx) nx=b_size-spx;

	  s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

	  if ((m=mapm.m[0][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

	  if ((m=mapm.m[1][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
	}
      }
    }

    else if (n!=0)
    { if (n<buf_y) s_move_down(n);

      for (sy=0, py=y/b_size; sy<n; sy+=ny, py++)
      { spy=(y+sy)%b_size;

	ny=n-sy;
	if (ny>b_size-spy) ny=b_size-spy;

	for (sx=0, px=x/b_size; sx<buf_x; sx+=nx, px++)
	{ spx=(x+sx)%b_size;

	  nx=buf_x-sx;
	  if (nx>b_size-spx) nx=b_size-spx;

	  s_write_picture(sx, sy, spx, spy, nx, ny, bkground(px, py));

	  if ((m=mapm.m[0][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[0]);

	  if ((m=mapm.m[1][px+map.sx*py])!=-1)
	   s_add_picture(sx, sy, spx, spy, nx, ny, pic[m].ptr[strength[0][0][px+map.sx*py]==0? 1:0]);
	}
      }
    }
  }

  start_x[tn]=x, start_y[tn]=y, rewrite[tn]=FALSE;

  memcpy(scr, ubuf, buf_x*buf_y);
}

void top_layer(long x, long y)
{ long sx, sy, px, py, m;

  x--, y-=2;

  for (sy=-y%b_size, py=y/b_size; sy<buf_y; sy+=b_size, py++)
   for (sx=-x%b_size, px=x/b_size; sx<buf_x; sx+=b_size, px++)
    if ((m=mapm.m[2][px+map.sx*py])!=-1 && strength[0][0][px+map.sx*py]!=0)
     b_add_shadow(sx, sy, pic[m].ptr[strength[1][0][px+map.sx*py]==0? 1:0]);

  x++, y+=2;

  for (sy=-y%b_size, py=y/b_size; sy<buf_y; sy+=b_size, py++)
   for (sx=-x%b_size, px=x/b_size; sx<buf_x; sx+=b_size, px++)
    if ((m=mapm.m[2][px+map.sx*py])!=-1 && strength[0][0][px+map.sx*py]!=0)
    { b_add_layer(sx, sy, pic[m].ptr[strength[1][0][px+map.sx*py]==0? 1:0]);

      if (pic[m].type==LIGHT_SRC && map.night && strength[1][0][px+map.sx*py]!=0 && game.mode!=MAP)
       b_set_layer(sx, sy, pic[m].ptr[2]);
    }
}

long search_type(long type)
{ long i;

  for (i=0; i<num_pic[(int)map.background]; i++)
   if (pic[i].type==type) return(i);

  return(-1);
}

long get_type(long x, long y, long layer)
{ long m;

  if (x<0 || x>=map.sx || y<0 || y>=map.sy) return(-1);

  if ((m=mapm.m[layer][x+map.sx*y])==-1)
   return(-1);

  return(pic[m].type);
}

void b_add_layer_d(long x, long y, long dir, unsigned char **l)
{ if (dir<=4)
   b_add_layer(x, y, l[dir]);

  else if (dir<=8)
   b_add_layer_x(x, y, l[8-dir]);

  else if (dir<=12)
   b_add_layer_xy(x, y, l[dir-8]);

  else b_add_layer_y(x, y, l[16-dir]);
}

void b_add_layer_l(long x, long y, unsigned char *layer)
{ if (map.night)
   b_add_layer_set_high(x, y, layer);
   else b_add_layer(x, y, layer);
}
