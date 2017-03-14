/* General functions for loading the maps */

#include <tank.h>
#include <string.h>

extern long id_select, lock_maps;

maptype map;
mapmemtype mapm;
strengthtype strength;

void reset_game(void)
{ int i;

  game.running=FALSE;
  game.flag_hunt=FALSE;
  game.tk[0]=NULL;
  game.tk[1]=NULL;
  game.flag_no[0] = 0;
  game.flag_no[1] = 0;
  game.supermarket_mayhem = FALSE;
  id_select=1;
  for (i = 0; i < num_products; i++)
    numprod[i] = 0;

  clear_objects();
  clear_effects();
  clear_bullets();
  clear_layers();
  clear_parts();
  clear_abombs();
  clear_transp();
  reset_controls();

  init_player("DATA/TUNE.MID");
}

void clear_map(long b, long nx, long ny)
{ long x, y, i, j;

  reset_game();

  map.background=b;
  pic=pictures[b];

  map.random_bonus=TRUE;
  map.night=FALSE;
  map.lock=FALSE;
  map.this_mission_code[0]=0;
  map.next_mission_code[0]=0;
  map.map_type=MAPT_DONTCARE;
  map.sizeofobj=sizeof(objectstruct);
  map.shareware=TRUE;
  map.sx=nx;
  map.sy=ny;
  map.public_pwd=FALSE;

  load_pictures();

  b*=4;

  for (x=0; x<map.sx; x++)
   for (y=0; y<map.sy; y++)
   { mapm.b[x+map.sx*y]=-1;
     mapm.g[x+map.sx*y]=0;

     for (i=0; i<2; i++)
     { mapm.man[i][x+map.sx*y]=0;

       for (j=0; j<3; j++)
	strength[i][j][x+map.sx*y]=0;
     }

     for (i=0; i<3; i++)
      mapm.m[i][x+map.sx*y]=-1;
   }

  init_player(map.background == 3? "DATA/BELVED.MID":"DATA/TUNE.MID");
}

FILE *openmapfile(char *fn, char *ext, char *om)
{
  char ffn[MAX_PATH];
  FILE *f;

  sprintf(ffn, "MAPS/%s%s", fn, ext);
  f = fopen(ffn, om);

  return(f);
}

FILE *read_header(char *fn, char *ext, char *om, maptype *m)
{
  FILE *f;

  f = openmapfile(fn, ext, om);

  if (f != 0)
  {
    fread(m, sizeof(maptype), 1, f);
    if (m->lock) protect(m, sizeof(maptype));
  }

  return f;
}

long check_suitable(void)
{
  if (map.map_type!=MAPT_DONTCARE)
   switch(game.mode)
   {
     case DUAL:
      if (map.map_type!=MAPT_DUAL)
       goto not_suitable;
      break;

     case SINGLE:
      if (map.map_type!=MAPT_SINGLE && map.map_type!=MAPT_SINGLEORDUAL)
       goto not_suitable;
      break;

     case DUALVC:
      if (map.map_type!=MAPT_DUALVC && map.map_type!=MAPT_SINGLEORDUAL)
       goto not_suitable;
      break;

     not_suitable:;
      message_key("THIS MAP IS NOT SUITABLE FOR THIS MODE");
      reset_game();
      return(FALSE);

     default:
      break;
   }

  return(TRUE);
}

long check_startpoints(void)
{ switch(game.mode)
  { case DUAL:
     if (game.tk[0]==NULL || game.tk[1]==NULL)
     {
       message_key("NO START POINT FOR THE TANK YOU SELECTED");
       reset_game();
       return(FALSE);
     }
     break;

    case DUALVC:
     if (game.tk[1]==NULL)
      goto notanks;

    case SINGLE:
     if (game.tk[0]==NULL)
      goto notanks;
     break;

    notanks:;
     message_key("NO START POINT, USE: OBJECT,ADD,TANK,1P MODE,..");
     reset_game();
     return(FALSE);

    default:
     break;
  }

  return(TRUE);
}

long load_map(char *fn, long include_objects)
{ long x, y, i, j, m;
  objectstruct *o;
  FILE *data;
  maptype t;

  message("LOADING MAP...");
  set_palette(pal);

  reset_game();
					   
  data = read_header(fn, ".MAP", "rb", &t);

  if (data == NULL)
  {
    message_key("FILE NOT FOUND");
    return(FALSE);
  }

  if (shareware && !t.shareware)
  {
    message_key("PLEASE REGISTER TANK ARENA TO PLAY THIS LEVEL");
    return(FALSE);
  }

  if (t.map_version > MAP_VERSION)
  {
    message_key("THIS LEVEL REQUIRES A NEWER VERSION OF TANK ARENA");
    return(FALSE);
  }

  memcpy(&map, &t, sizeof(map));

  for (i=0; i<3; i++)
    fread(mapm.m[i], map.sx*map.sy*2, 1, data);

  fread(mapm.g, map.sx*map.sy, 1, data);

  fread(mapm.b, map.sx*map.sy, 1, data);

  for (i=0; i<2; i++)
   fread(mapm.man[i], map.sx*map.sy, 1, data);

  pic=pictures[(int)map.background];

  load_pictures();

  for (x=0; x<map.sx; x++)
   for (y=0; y<map.sy; y++)
    for (i=0; i<2; i++)
     if ((m=mapm.m[i+1][x+map.sx*y])!=-1)
      for (j=0; j<3; j++)
       strength[i][j][x+map.sx*y]=pic[m].str[j];
      else for (j=0; j<3; j++)
       strength[i][j][x+map.sx*y]=0;

  for (x=0; x<map.sx; x++)
   for (y=0; y<map.sy; y++)
    for (i=0; i<2; i++)
     if (strength[i][0][x+map.sx*y]==1000)
     { for (i=0; i<2; i++)
	for (j=0; j<3; j++)
	 strength[i][j][x+map.sx*y]=j==0? 1000:0;
       break;
     }

  if (include_objects)
  { fseek(data, map.mission_text[3], SEEK_SET);

    do
    { o=allocate(sizeof(objectstruct));
      memset(o, 0, sizeof(objectstruct));
      fread(o, map.sizeofobj, 1, data);
      add_object_to_list(o);
    } while(!feof(data));

    remove_object_from_list(o);
    free(o);

    for (o=obj; o!=NULL; o=o->next)
    {
      if (o->id!=OBJ_MAGIC)
	error("load_map: invalid magic number in object\n");

      relocate(&o->owner);
      relocate(&o->target);

      switch(o->type)
      { case TANK_TYPE:
	 prepare_tank(o);
	 break;

	case MINE_TYPE:
	 prepare_mine(o);
	 break;

	case TRAIN_TYPE:
	case WAGON_TYPE:
	 prepare_train(o);
	 break;

	case B52_TYPE:
	 prepare_b52(o);
	 break;

	case TURRET_TYPE:
	 prepare_turret(o);
	 break;

	case PLAYER_TYPE:
	 prepare_player(o);
	 break;

	case WARP_TYPE:
	 prepare_warp(o);
	 break;

        case FLAG_TYPE:
         prepare_flag(o);
         break;

        case LOCK_TYPE:
         prepare_lock(o);
         break;

	case GOAL_TYPE:
	 prepare_goal(o);
	 break;

	case DESTROYER_TYPE:
	 prepare_destroyer(o);
	 break;

	case ENFORCER_TYPE:
	 prepare_enforcer(o);
	 break;

	case PRODUCT_TYPE:
	 prepare_product(o);
	 break;

	default:
	 error("load_map: invalid object type\n");
      }
    }
  }

  fclose(data);

  if (game.mode!=MAP)
  {
    create_all_bonusses();

    if (shareware)
      make_zeppelin();

    if (game.flag_hunt)
    {
      if (game.flag_no[0] != 0)
	game.flag_no[0] = rnd(game.flag_no[0]);

      if (game.flag_no[1] != 0)
	game.flag_no[1] = rnd(game.flag_no[1]);
    }
  }

  init_player(map.background == 3? "DATA/BELVED.MID":"DATA/TUNE.MID");

  clear(screen);

  return(TRUE);
}

long save_map(char *fn)
{ objectstruct *o;
  FILE *data, *text;
  char *buf, str[20];
  long i, size;
  
  data = openmapfile(fn, ".MAP", "rb");

  if (data != NULL)
  { fclose(data);

    message("OVERWRITE MAP? (Y/N)");

    if (wait_yesno()!=RKEY_Y)
     return(FALSE);
  }

  map.map_version = MAP_VERSION;

  map.map_contents=0;

  for (o=obj; o!=NULL; o=o->next)
   switch(o->type)
   { case FLAG_TYPE:
      map.map_contents|=MAPC_FLAG;
      break;

     case PLAYER_TYPE:
      if (o->d.pl.type<6)
       map.map_contents|=1<<(o->d.pl.type>>1);
       else map.map_contents|=MAPC_RACE;
      break;
   }

  if ((orred(MAPC_RACE, map.map_contents) && orred(MAPC_TANK|MAPC_CHOP|MAPC_PLANE, map.map_contents)) ||
      (orred(MAPC_RACE, map.map_contents) && orred(MAPC_FLAG, map.map_contents)))
   message_key("WARNING: STARTING POINT CONFLICT");

  data = openmapfile(fn, ".MAP", "wb");

  if (data == NULL)
  {
    message_key("UNABLE TO SAVE MAP.");
    return(FALSE);
  } else message("SAVING MAP...");

  if (map.this_mission_code[0]==0)
   strcpy(map.this_mission_code, fn);
   
  fseek(data, sizeof(map), SEEK_SET);

  for (i=0; i<3; i++)
   fwrite(mapm.m[i], map.sx*map.sy*2, 1, data);

  fwrite(mapm.g, map.sx*map.sy, 1, data);

  fwrite(mapm.b, map.sx*map.sy, 1, data);

  for (i=0; i<2; i++)
   fwrite(mapm.man[i], map.sx*map.sy, 1, data);

  buf=allocate(10000);

  for (i=0; i<3; i++)
  { map.mission_text[i]=ftell(data);

    sprintf(str, ".%ld", i);
    text = openmapfile(fn, str, "rt");

    if (text != NULL)
    { size=fread(buf, 1, 10000, text);
      fwrite(buf, size, 1, data);
      fclose(text);
    }
  }

  map.mission_text[3]=ftell(data);

  free(buf);

  for (o=obj; o!=NULL; o=o->next)
  { o->this_structure=o;

    fwrite(o, sizeof(objectstruct), 1, data);
  }

  map.sizeofobj=sizeof(objectstruct);

  if (lock_maps)
  { map.lock=TRUE;
    protect(&map, sizeof(map));
  }

  rewind(data);
  fwrite(&map, sizeof(map), 1, data);

  if (lock_maps)
   protect(&map, sizeof(map));

  fclose(data);

  clear(screen);

  return(TRUE);
}

void relocate(objectstruct **ptr)
{ objectstruct *o;

  if (*ptr!=NULL)
  { for (o=obj; o!=NULL; o=o->next)
     if (*ptr==o->this_structure)
     { *ptr=o;
       return;
     }

    *ptr=NULL;
  }
}

void protect(void *ptr, long size)
{ size/=4;

  for (; size>0; size--, ptr+=4)
   *(long *)ptr^=0x34312e33;
}
