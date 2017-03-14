/* File functions (create/load/save map) */

#include <edit.h>

void create_fun(void)
{ long x, y, i, t1, t2, t3, t4, nx=50, ny=50;
  char dimx[4]="50", dimy[4]="50";

  if (!read_string("ENTER X SIZE: ", dimx, 3) ||
      !read_string("ENTER Y SIZE: ", dimy, 3))
   return;

  sscanf(dimx, "%ld", &nx);
  sscanf(dimy, "%ld", &ny);
 
  nx=(nx+3)&0xfffffffc;
  ny=(ny+3)&0xfffffffc;

  if (nx<8) nx=8;
  if (ny<8) ny=8;

  if (nx*ny>max_map_x*max_map_y) nx=max_map_x, ny=max_map_y;

  clear_map(maptype_mnu[0].s, nx, ny);

  switch (stdmap_mnu[0].s)
  { case 1:
     t1=search_type(BARRIER_L);
     t2=search_type(BARRIER_R);

     for (y=0; y<map.sy; y++)
      for (i=0; i<2; i++)
      { mapm.m[i+1][map.sx*y]=t1;
	strength[i][0][map.sx*y]=1000;

	mapm.m[i+1][map.sx-1+map.sx*y]=t2;
	strength[i][0][map.sx-1+map.sx*y]=1000;
      }

     t1=search_type(BARRIER_T);
     t2=search_type(BARRIER_B);

     for (x=0; x<map.sx; x++)
      for (i=0; i<2; i++)
      { mapm.m[i+1][x]=t1;
	strength[i][0][x]=1000;

	mapm.m[i+1][x+map.sx*(map.sy-1)]=t2;
	strength[i][0][x+map.sx*(map.sy-1)]=1000;
      }

     t1=search_type(BARRIER_TL);
     t2=search_type(BARRIER_TR);
     t3=search_type(BARRIER_BL);
     t4=search_type(BARRIER_BR);

     for (i=0; i<2; i++)
     { mapm.m[i+1][0]=t1;
       strength[i][0][0]=1000;

       mapm.m[i+1][map.sx-1]=t2;
       strength[i][0][map.sx-1]=1000;

       mapm.m[i+1][map.sx*(map.sy-1)]=t3;
       strength[i][0][map.sx*(map.sy-1)]=1000;

       mapm.m[i+1][map.sx-1+map.sx*(map.sy-1)]=t4;
       strength[i][0][map.sx-1+map.sx*(map.sy-1)]=1000;
     }
     break;

    case 2:
     t1=search_type(maptype_mnu[0].s==2? PIT_C:WATER);

     for (x=0; x<map.sx; x++)
      for (y=0; y<map.sy; y++)
       mapm.m[0][x+map.sx*y]=t1;

     break;
  }

  load_structs();

  stdmap_mnu[0].s=-1;

  map_setup_menu();
}

void load_fun(void)
{
  if (read_string("LOAD FILE (ESC = LIST): ", edit_map_name, 8) || select_from_list(edit_map_name))
  { load_map(edit_map_name, file_mnu[0].s!=3);

    if (map.lock && !lock_maps && !unlock_maps)
    { message_key("MAP IS LOCKED!");
      clear_map(0, 48, 48);
    }

    map.lock = FALSE;
  }

  load_structs();

  map_setup_menu();

  file_mnu[0].s=-1;
  edit_mnu[0].s=-1;
}

void save_fun(void)
{ do
  { if (read_string("SAVE AS: ", edit_map_name, 8) && save_map(edit_map_name))
     break;
    else message("WANT TO SAVE? (Y/N)");
  } while (wait_yesno()!=RKEY_N);

  clear(screen);

  file_mnu[0].s=-1;
  edit_mnu[0].s=-1;
}

void quit_fun(void)
{ exit_edit=TRUE;
}
