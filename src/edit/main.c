/* Main routines of the map editor. Uses a nice interface like 3dstudio
   which works quite well.
*/

#include <edit.h>

/*************************** LOCAL VARIABLES *********************************/

void (*click_left)(long x, long y);
void (*click_right)(long x, long y);
void (*kb_control_fun)(void);
void (*write_left_fun)(void);

objectstruct *ao, *prev_wagon;
long exit_edit, stx, sty, smx, smy, s_pic, sel_x, sel_y, start_x=0, start_y=0;
char edit_map_name[9]="DEFAULT";

smartmenu *active_mnu;
updownmenu *updown_mnu;
structtype *st;

/*************************** MOUSE CONTROL ***********************************/

void write_marker(long x, long y)
{ m13_xpixel(x-1, y, 15);
  m13_xpixel(x+1, y, 15);
  m13_xpixel(x, y-1, 15);
  m13_xpixel(x, y+1, 15);
  m13_xpixel(x-2, y, 15);
  m13_xpixel(x+2, y, 15);
  m13_xpixel(x, y-2, 15);
  m13_xpixel(x, y+2, 15);
  m13_xpixel(x-3, y, 15);
  m13_xpixel(x+3, y, 15);
  m13_xpixel(x, y-3, 15);
  m13_xpixel(x, y+3, 15);
}

void show_marker(long x, long y)
{ static long lx=-1, ly=-1;

  if (x==lx && y==ly)
   return;

  if (lx!=-1)
   write_marker(lx, ly);

  if (x!=-1)
   write_marker(x, y);

  lx=x, ly=y;
}

void show_location(long x, long y)
{ static int erase=TRUE;

  if (x<199 && y<199)
  { m13_overwrite_text(277, 194, 15, "%3d,%3d", smx+x/b_size, smy+y/b_size);
    erase=TRUE;
  }
  else if (erase)
  { m13_bar(277, 194, 42, 6, 0);
    erase=FALSE;
  }
}

/*************************** LEFT SCREEN CONTROL *****************************/

void copy_buffer_to_left(void)
{
  blit(scrbuf, screen, 0, 0, 0, 0, buf_x, buf_y);
  m13_bar(0, 198, 200, 2, 0);
  m13_bar(198, 0, 2, 200, 0);
}

void write_left(void)
{ long x, y, x2, y2, i;

  set_window_size(6*b_size, 6*b_size);

  for (x=0, x2=smx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=smy; y<buf_y; y+=b_size, y2++)
   { b_first_layer(x, y, bkground(x2, y2));

     if ((i=mapm.m[0][x2+map.sx*y2])!=-1)
      b_add_layer(x, y, pic[i].ptr[0]);

     if ((i=mapm.m[1][x2+map.sx*y2])!=-1)
      b_add_layer(x, y, pic[i].ptr[0]);
   }

  ssx=smx*b_size, ssy=smy*b_size;

  write_objects(ao);
}

void write_left_bare(void)
{ write_left();
  copy_buffer_to_left();
}

void write_left_with_block(void)
{ long x, y, x2, y2;

  write_left();

  for (x=0, x2=smx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=smy; y<buf_y; y+=b_size, y2++)
    b_add_layer(x, y, intern[BLOCK+2].ptr);

  copy_buffer_to_left();
}

void write_left_with_men(void)
{ long x, y, x2, y2;
  char str[10];

  write_left();

  for (x=0, x2=smx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=smy; y<buf_y; y+=b_size, y2++)
    if (mapm.man[1][x2+map.sx*y2]!=0)
    { sprintf(str, "%d", mapm.man[1][x2+map.sx*y2]);

      switch(mapm.man[0][x2+map.sx*y2])
      { case HT_CHAIN:
	 b_add_layer(x+12-strlen(str)*3, y+10, icons[0][1]);
	 break;

	case HT_FLAME:
	 b_add_layer(x+12-strlen(str)*3, y+10, icons[0][2]);
	 break;

	case HT_SROCK:
	 b_add_layer(x+12-strlen(str)*3, y+10, icons[0][4]);
	 break;
      }

      b_write_text(x+22-strlen(str)*3, y+13, 15, str);
    }

  copy_buffer_to_left();
}

void write_left_with_struct_goals(void)
{ long i, x, y, x2, y2, gcg=0, gcb=0;
  char str[10];

  write_left();

  for (i=0; i<map.sx*map.sy; i++)
   if (mapm.g[i]>0) gcg+=mapm.g[i]; else gcb-=mapm.g[i];

  for (x=0, x2=smx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=smy; y<buf_y; y+=b_size, y2++)
   { sprintf(str, "%d", mapm.g[x2+map.sx*y2]);
     b_write_text(x+17-strlen(str)*3, y+13, 15, str);
   }

  b_write_text(0, 0, 15, "GOOD: %ld, EVIL: %ld", gcg, gcb);

  copy_buffer_to_left();
}

void write_left_with_obj_goals(void)
{ long gcg=0, gcb=0;
  objectstruct *o;
  char str[10];

  write_left();

  for (o=obj; o!=NULL; o=o->next)
   switch(o->type)
   { case TANK_TYPE:
     case B52_TYPE:
     case TRAIN_TYPE:
     case WAGON_TYPE:
     case MINE_TYPE:
     case LOCK_TYPE:
     case GOAL_TYPE:
      if (o->gc>0) gcg+=o->gc; else gcb-=o->gc;

      sprintf(str, "%ld", o->gc);
      b_write_text(o->x-ssx+17-strlen(str)*3, o->y-ssy+13, 15, str);
      break;
   }

  b_write_text(0, 0, 15, "GOOD: %ld, EVIL: %ld", gcg, gcb);

  copy_buffer_to_left();
}

void write_left_with_bonus(void)
{ long i, x, y, x2, y2;

  write_left();

  for (x=0, x2=smx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=smy; y<buf_y; y+=b_size, y2++)
    if ((i=mapm.b[x2+map.sx*y2])!=-1)
     b_add_layer(x+11, y+11, icons[0][i]);

  copy_buffer_to_left();
}
			
void write_struct_left(void)
{ long x, y, x2, y2, i, s;

  buf_x=6*b_size, buf_y=6*b_size;

  for (x=0, x2=stx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=sty; y<buf_y; y+=b_size, y2++)
    b_first_layer(x, y, bkground(x, y));

  s=show_mnu[0].s;

  switch(s)
  { case 2:
     for (x=0, x2=stx; x<buf_x; x+=b_size, x2++)
      for (y=0, y2=sty; y<buf_y; y+=b_size, y2++)
       if ((i=(*st)[x2][y2].pic[2])!=-1)
	b_add_shadow(x+1, y+2, pic[i].ptr[0]);

    case 0:
    case 1:
     m13_add_image(320-b_size, 200-b_size, b_size, b_size, pic[s_pic].ptr[0]);

     for (x=0, x2=stx; x<buf_x; x+=b_size, x2++)
      for (y=0, y2=sty; y<buf_y; y+=b_size, y2++)
       if ((i=(*st)[x2][y2].pic[s])!=-1)
	b_add_layer(x, y, pic[i].ptr[0]);

     break;

    case 3:
     for (x=0, x2=stx; x<buf_x; x+=b_size, x2++)
      for (y=0, y2=sty; y<buf_y; y+=b_size, y2++)
      { if ((i=(*st)[x2][y2].pic[0])!=-1)
	 b_add_layer(x, y, pic[i].ptr[0]);

	if ((i=(*st)[x2][y2].pic[1])!=-1)
	 b_add_layer(x, y, pic[i].ptr[0]);

	if ((i=(*st)[x2][y2].pic[2])!=-1)
	{ b_add_shadow(x+1, y+2, pic[i].ptr[0]);
	  b_add_layer(x, y, pic[i].ptr[0]);
	}
      }

     break;
  }

  for (x=0, x2=stx; x<buf_x; x+=b_size, x2++)
   for (y=0, y2=sty; y<buf_y; y+=b_size, y2++)
   { i=(*st)[x2][y2].links;

     if ((i&UP)!=UP)
      b_add_layer(x, y, intern[BLOCK].ptr);

     if ((i&LEFT)!=LEFT)
      b_add_layer(x, y, intern[BLOCK+1].ptr);
   }

  copy_buffer_to_left();
}

/*************************** KEYBOARD CONTROL ********************************/

void kb_map_control(void)
{ long change=FALSE;
  unsigned k;

  do
  { k=scan_key();

    switch(k)
    { case RKEY_UP:
       if (smy>0) smy--;
       change=TRUE;
       break;

      case RKEY_PGUP:
       if (smy-6>0) smy-=6; else smy=0;
       change=TRUE;
       break;

      case RKEY_DOWN:
       if (smy<max_y) smy++;
       change=TRUE;
       break;

      case RKEY_PGDN:
       if (smy+6<max_y) smy+=6; else smy=max_y;
       change=TRUE;
       break;

      case RKEY_LEFT:
       if (smx>0) smx--;
       change=TRUE;
       break;

      case RKEY_HOME:
       if (smx-6>0) smx-=6; else smx=0;
       change=TRUE;
       break;

      case RKEY_RIGHT:
       if (smx<max_x) smx++;
       change=TRUE;
       break;

      case RKEY_END:
       if (smx+6<max_x) smx+=6; else smx=max_x;
       change=TRUE;
       break;

      case RKEY_ESC:
       edit_mnu[0].s=3;
       file_mnu[0].s=4;
       active_mnu=edit_mnu;
       ao=NULL;
       change=TRUE;
       break;

      case RKEY_TAB:
       view_map();
       change=TRUE;
       break;

      case RKEY_SPACE:
       edit_mnu[0].s=0;
       struct_mnu[0].s=0;
       active_mnu=edit_mnu;
       ao=NULL;
       change=TRUE;
       break;

      case RKEY_F2:
       edit_mnu[0].s=3;
       file_mnu[0].s=2;
       active_mnu=edit_mnu;
       ao=NULL;
       change=TRUE;
       break;

      case RKEY_F3:
       edit_mnu[0].s=3;
       file_mnu[0].s=1;
       active_mnu=edit_mnu;
       ao=NULL;
       change=TRUE;
       break;

      case RKEY_F12:
       editstr_mnu[0].s=0;
       active_mnu=editstr_mnu;
       ao=NULL;
       change=TRUE;
       break;

     case RKEY_PRTSCRN:
       grab_frame();
       break;
    }
  } while (k!=0xffff);

  if (change)
  { execute_smart_menu();
    write_smart_menu();
    write_left_fun();
  }
}

void kb_struct_control(void)
{ long change=FALSE;
  unsigned k;

  do
  { k=scan_key();

    switch(k)
    { case RKEY_UP:
       if (sty>0) sty--;
       change=TRUE;
       break;

      case RKEY_PGUP:
       if (sty-6>0) sty-=6; else sty=0;
       change=TRUE;
       break;

      case RKEY_DOWN:
       if (sty<44) sty++;
       change=TRUE;
       break;

      case RKEY_PGDN:
       if (sty+6<44) sty+=6; else sty=44;
       change=TRUE;
       break;

      case RKEY_LEFT:
       if (stx>0) stx--;
       change=TRUE;
       break;

      case RKEY_HOME:
       if (stx-6>0) stx-=6; else stx=0;
       change=TRUE;
       break;

      case RKEY_RIGHT:
       if (stx<44) stx++;
       change=TRUE;
       break;

      case RKEY_END:
       if (stx+6<44) stx+=6; else stx=44;
       change=TRUE;
       break;

      case RKEY_ESC:
       edit_mnu[0].s=-1;
       active_mnu=edit_mnu;
       change=TRUE;
       break;

      case RKEY_MIN:
       if (s_pic>0) s_pic--;
       change=TRUE;
       break;

      case RKEY_PLUS:
       if (s_pic<num_pic[(int)map.background]-1) s_pic++;
       change=TRUE;
       break;

      case RKEY_PRTSCRN:
       grab_frame();
       break;
    }
  } while (k!=0xffff);

  if (change)
  { execute_smart_menu();
    write_smart_menu();
    write_left_fun();
  }
}			  

/*************************** SMART MENU CONTROL ******************************/

void write_smart_menu(void)
{ updownmenu *u=updown_mnu;
  smartmenu *m=active_mnu;
  long i, x, y, s, c;

  show_marker(-1, -1);

  for (x=size_x+4, y=4, i=0; i!=-1 && m!=NULL; x+=3, i=m[0].s, m=m[i].menu)
   for (i=0; m[i].name!=NULL; i++, y+=6)
   { if (m[m[0].s].menu!=NULL || u!=NULL)
       if (m[0].s!=i) c=7; else c=31;
     else
       if (m[0].s!=i) c=15; else c=47;

     if (!shareware)
     {
       if (m[i].attr == DISABLED) c = 87;
     }
     else
     {
       if (m[i].attr != ENABLED) c = 87;
     }

     m13_bar(size_x, y, x-size_x, 5, 0);
     s=m13_overwrite_text(x, y, c, m[i].name);
     m13_bar(x+s, y, 320-x-s, 5, 0);
   }

  if (u!=NULL)
   for (i=0; u[i].name!=NULL; i++, y+=6)
   { switch(u[i].display_type)
     { case DISPLAY_NORM:
        s=m13_overwrite_text(x, y, 47, "%d %s", *u[i].value, u[i].name);
        break;

       case DISPLAY_SEC:
	s=m13_overwrite_text(x, y, 47, "%.1f %s", (float)*u[i].value/sec, u[i].name);
	break;

       case DISPLAY_100:
	s=m13_overwrite_text(x, y, 47, "%s %3d %%", u[i].name, *u[i].value);
	break;

       case DISPLAY_AFT:
	s=m13_overwrite_text(x, y, 47, "%s %d", u[i].name, *u[i].value);
	break;

       case DISPLAY_SAFT:
	s=m13_overwrite_text(x, y, 47, "%s %.1f S", u[i].name, (float)*u[i].value/sec);
        break;

       case DISPLAY_PIX:
	s=m13_overwrite_text(x, y, 47, "%s %4d P", u[i].name, *u[i].value);
        break;

       case DISPLAY_ACTIVATOR:
	s=m13_overwrite_text(x, y, 47, "%s %s", u[i].name, *u[i].value==0? "GOOD":"EVIL");
        break;

       case DISPLAY_YES:
	s=m13_overwrite_text(x, y, 47, "%s %s", u[i].name, *u[i].value? "YES":" NO");
        break;

       case DISPLAY_2R:
	s=m13_overwrite_text(x, y, 47, "%s %2d", u[i].name, *u[i].value);
        break;

       default:
	s=0;
	break;
     }
     m13_bar(size_x, y, x-size_x, 5, 0);
     m13_bar(x+s, y, 320-x-s, 5, 0);
   }

  m13_bar(size_x, y, 320-size_x, 200-y, 0);
}

void execute_smart_menu(void)
{ smartmenu *m;
  long i;

  click_left=NULL, click_right=NULL, updown_mnu=NULL;
  write_left_fun=write_left_bare, kb_control_fun=kb_map_control;

  for (m=active_mnu; m!=NULL && (i=m[0].s)!=-1; m=m[i].menu)
    if (m[i].fun!=NULL)
      m[i].fun();
}

void control_smart_menu(long b, long s)
{ smartmenu *m=active_mnu;
  updownmenu *u=updown_mnu;
  long i, j=0;			    

  for (i=0; i!=-1 && m!=NULL; i=m[0].s, m=m[i].menu)
   for (i=0; m[i].name!=NULL; i++)
    if (j==s && m!=NULL && i!=-1)
    {
      if (!shareware)
      {
	if (m[i].attr != DISABLED) m[0].s=i;
      }
      else
      {
	if (m[i].attr == ENABLED) m[0].s=i;
      }

      return;

    } else j++;

  if (u!=NULL)
   for (i=0; u[i].name!=NULL; i++, j++)
    if (j==s)
     switch(b)
     { case 1:
	if (u[i].step==0) *u[i].value=!*u[i].value;
        else if (*u[i].value<u[i].max) *u[i].value+=u[i].step;
	break;

       case 2:
	if (u[i].step==0) *u[i].value=!*u[i].value;
        else if (*u[i].value>u[i].min) *u[i].value-=u[i].step;
	break;
     }
}
/*************************** READ STRUCTURES *********************************/

void load_structs(void)
{ char ffn[MAX_PATH];
  FILE *data;
  long x, y;

  sprintf(ffn, "DATA/MAP_%d.DAT", map.background);

  if ((data=fopen(ffn, "rb"))==NULL)
  { for (x=0; x<50; x++)
     for (y=0; y<50; y++)
     { (*st)[x][y].pic[0]=-1;
       (*st)[x][y].pic[1]=-1;
       (*st)[x][y].pic[2]=-1;
       (*st)[x][y].links=NONE;
     }
  }
  else
  { fread(st, sizeof(structtype), 1, data);
    fclose(data);
  }

  s_pic=num_pic[(int)map.background]-1;
}

void save_structs(void)
{ char ffn[MAX_PATH];
  FILE *data;

  sprintf(ffn, "DATA/MAP_%d.DAT", map.background);

  if ((data=fopen(ffn, "wb"))!=NULL)
  { fwrite(st, sizeof(structtype), 1, data);
    fclose(data);
  }
}

/*************************** EDITOR MAIN FUNCTION ****************************/

void edit_map(void)
{
  long x, y, b1, b2=0;

  active_mnu=edit_mnu, smx=0, smy=0, stx=0, sty=0;
  sel_x=0, sel_y=0, updown_mnu=NULL, game.mode=MAP;

  if (!reset_mouse())
  { message_key("MOUSE DRIVER NOT INSTALLED!");
    return;
  }

  st=allocate(sizeof(structtype));

  clear_map(0, 48, 48);

  load_fun();

  execute_smart_menu();
  write_smart_menu();
  write_left_fun();

  for (exit_edit=FALSE; !exit_edit; )
  { b1=get_mouse(&x, &y);
    show_marker(x, y);
    show_location(x, y);

    kb_control_fun();

    if (b1!=b2)
    { b2=b1;

      if (b1!=0)
      { show_marker(-1, -1);

	b2=b1;

	if (b1==4)
	 done_fun();

	else if ((b1==1 || b1==2) && x>size_x+4 && y>=4)
	 control_smart_menu(b1, (y-4)/6);

	else if (x<6*b_size && y<6*b_size)
	 switch(b1)
	 { case 1:
	    if (click_left!=NULL)
	     click_left(x, y);
	    break;

	   case 2:
	    if (click_right!=NULL)
	     click_right(x, y);
	    break;
	 }

	execute_smart_menu();
	write_smart_menu();
	write_left_fun();
      }
    }
  }

  save_fun();

  free(st);

  reset_game();
}
