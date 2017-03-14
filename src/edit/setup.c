/* Edit the structure menu. This is located under F12 in the map editor
   and is not supposed to be used by other people than me.
*/

#include <edit.h>

void struct_place(long x, long y)
{ x=stx+(x*6)/size_x;
  y=sty+(y*6)/size_y;

  switch (show_mnu[0].s)
  { case 0:
     (*st)[x][y].pic[0]=s_pic;
     break;

    case 1:
     (*st)[x][y].pic[1]=s_pic;
     break;

    case 2:
     (*st)[x][y].pic[2]=s_pic;
     break;
  }
}

void struct_remove(long x, long y)
{ x=stx+(x*6)/size_x;
  y=sty+(y*6)/size_y;

  switch (show_mnu[0].s)
  { case 0:
     (*st)[x][y].pic[0]=-1;
     break;

    case 1:
     (*st)[x][y].pic[1]=-1;
     break;

    case 2:
     (*st)[x][y].pic[2]=-1;
     break;

    case 3:
     (*st)[x][y].pic[0]=-1;
     (*st)[x][y].pic[1]=-1;
     (*st)[x][y].pic[2]=-1;
     break;
  }
}

void show_fun(void)
{ write_left_fun=write_struct_left, kb_control_fun=kb_struct_control;
  click_left=struct_place, click_right=struct_remove;
}

void struct_link(long x, long y)
{ long x2=x%b_size, y2=y%b_size;

  x=x/b_size+stx;
  y=y/b_size+sty;

  if (x2>25 && x<49)
  { (*st)[x][y].links|=RIGHT;
    (*st)[x+1][y].links|=LEFT;
  }

  else if (x2<5 && x>0)
  { (*st)[x][y].links|=LEFT;
    (*st)[x-1][y].links|=RIGHT;
  }

  if (y2>25 && y<49)
  { (*st)[x][y].links|=DOWN;
    (*st)[x][y+1].links|=UP;
  }

  else if (y2<5 && y>0)
  { (*st)[x][y].links|=UP;
    (*st)[x][y-1].links|=DOWN;
  }
}

void struct_unlink(long x, long y)
{ long i;

  x=x/b_size+stx;
  y=y/b_size+sty;

  i=(*st)[x][y].links;

  if ((i&LEFT)==LEFT)
   (*st)[x-1][y].links&=LEFT+UP+DOWN;

  if ((i&RIGHT)==RIGHT)
   (*st)[x+1][y].links&=RIGHT+UP+DOWN;

  if ((i&UP)==UP)
   (*st)[x][y-1].links&=LEFT+RIGHT+UP;

  if ((i&DOWN)==DOWN)
   (*st)[x][y+1].links&=LEFT+RIGHT+DOWN;

  (*st)[x][y].links=NONE;
}

void link_fun(void)
{ write_left_fun=write_struct_left, kb_control_fun=kb_struct_control;
  click_left=struct_link, click_right=struct_unlink;
}

void savestr_fun(void)
{ write_left_fun=write_struct_left;
  editstr_mnu[0].s=0;

  save_structs();
}
