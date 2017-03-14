/* Place structures in the map */

#include <edit.h>

void select_struct(long x, long y)
{ x=x/b_size+stx;
  y=y/b_size+sty;

  while (((*st)[x][y].links&UP)==UP) y--;
  while (((*st)[x][y].links&LEFT)==LEFT) x--;

  sel_x=x, sel_y=y;

  struct_mnu[0].s=1;
}

void struct_fun(void)
{ write_left_fun=write_left_with_block;
}

void select_fun(void)
{ write_left_fun=write_struct_left, kb_control_fun=kb_struct_control;
  click_left=select_struct;
}

void recursive_add_struct(long x, long y, long fx, long fy)
{ long i, j, p;

  if (x<0 || x>=map.sx || y<0 || y>=map.sy || fx<0 || fx>=50 || fy<0 || fy>=50)
   return;

  for (i=0; i<3; i++)
   if ((p=(*st)[fx][fy].pic[i])!=-1)
   { mapm.m[i][x+map.sx*y]=p;

     if (i>0)
      for (j=0; j<3; j++)
       strength[i-1][j][x+map.sx*y]=pic[p].str[j];
   }

  i=(*st)[fx][fy].links;

  if ((i&DOWN)==DOWN)
   recursive_add_struct(x, y+1, fx, fy+1);

  if ((i&RIGHT)==RIGHT)
   recursive_add_struct(x+1, y, fx+1, fy);
}

void do_remove_struct(long x, long y)
{ long i, j;

  if (mapm.m[2][x+map.sx*y]!=-1 || mapm.m[1][x+map.sx*y]!=-1)
   for (i=0; i<2; i++)
   { mapm.m[i+1][x+map.sx*y]=-1;

     for (j=0; j<3; j++)
      strength[i][j][x+map.sx*y]=0;
   }

  else mapm.m[0][x+map.sx*y]=-1;
}

void add_struct(long x, long y)
{ x=x/b_size+smx;
  y=y/b_size+smy;

  recursive_add_struct(x, y, sel_x, sel_y);
}

void remove_struct(long x, long y)
{ x=x/b_size+smx;
  y=y/b_size+smy;

  do_remove_struct(x, y);
}

void addremove_fun(void)
{ click_left=add_struct;
  click_right=remove_struct;
}

void set_startpoint(long x, long y)
{ start_x=x/b_size+smx;
  start_y=y/b_size+smy;
}

void smallest_first(long *x, long *y)
{ long t;

  if (*y<*x) t=*x, *x=*y, *y=t;
}

void add_region(long x, long y)
{ long i, j, sx=start_x, sy=start_y;

  x=x/b_size+smx;
  y=y/b_size+smy;

  smallest_first(&sx, &x);
  smallest_first(&sy, &y);

  for (i=sx; i<=x; i++)
   for (j=sy; j<=y; j++)
    recursive_add_struct(i, j, sel_x, sel_y);
}

void remove_region(long x, long y)
{ long i, j, sx=start_x, sy=start_y;

  x=x/b_size+smx;
  y=y/b_size+smy;

  smallest_first(&sx, &x);
  smallest_first(&sy, &y);

  for (i=sx; i<=x; i++)
   for (j=sy; j<=y; j++)
    do_remove_struct(i, j);
}

void addreg_fun(void)
{ click_left=set_startpoint;
  click_right=add_region;
}

void remreg_fun(void)
{ click_left=set_startpoint;
  click_right=remove_region;
}
