/* Writes the status of a tank */

#include <tank.h>
#include <string.h>

#define tnk o->d.tk
#define ttnk target->d.tk

long speech[]={0,1,2,3,3,2,2,3,2,1,2,3,2,0,0,1,2,3,3,2,1,2,3,2,1,3,2,0,0,0};

void create_message(objectstruct *target, char *line1, char *line2)
{ if (target!=NULL && target->type==TANK_TYPE)
  { strncpy(ttnk.msg.line[0], line1, 29);
    strncpy(ttnk.msg.line[1], line2, 29);
    ttnk.msg.count=29;
    ttnk.msg.end=tnow;
  }
}

static void write_message(objectstruct *o)
{ long i, m, f;

  if ((m=tnk.msg.count)!=0)
  { f=tnk.shield.armor!=0? speech[29-m]:4;

    b_add_layer(5, buf_y-11-5-b_size, tnk.tank->face[f]);

    if (m>16 || (m&1)==1)
     for (i=0; i<2; i++)
      b_write_text(10+b_size, buf_y-11-b_size+10*i, cmap[15], tnk.msg.line[i]);
  }
}

static void write_supermarket_message(objectstruct *o)
{ long m;

  if ((m=tnk.msg.count)!=0 && (m>16 || (m&1)==1))
    b_write_text(5, buf_y-35, cmap[15], tnk.msg.line[1]);
}

static void write_arrow(objectstruct *o)
{ objectstruct *p;
  long x, y;

  p=game.tk[game.tn^1];

  if (p==NULL || p->d.tk.invis.count!=0 || tnk.invis.count!=0)
   return;

  x=p->x, y=p->y;
  if (x+b_size>ssx+buf_x)
  { if (y<ssy) b_add_layer(buf_x-b_size, 0, intern[ARROW+1].ptr);
    else if (y+b_size<ssy+buf_y) b_add_layer(buf_x-b_size, (buf_y-b_size)/2, intern[ARROW+2].ptr);
    else b_add_layer(buf_x-b_size, buf_y-b_size, intern[ARROW+3].ptr);
  }
  else if (x>ssx)
  { if (y<ssy) b_add_layer((buf_x-b_size)/2, 0, intern[ARROW].ptr);
    else if (y+b_size>ssy+buf_y) b_add_layer((buf_x-b_size)/2, buf_y-b_size, intern[ARROW+4].ptr);
  }
  else
  { if (y<ssy) b_add_layer(0, 0, intern[ARROW+7].ptr);
    else if (y+b_size<ssy+buf_y) b_add_layer(0, (buf_y-b_size)/2, intern[ARROW+6].ptr);
    else b_add_layer(0, buf_y-b_size, intern[ARROW+5].ptr);
  }
}

static void write_number(long x, long y, long n)
{ if (n<10) x+=3;
  b_write_text(x, y, cmap[13], "%ld", n>99? 99:n);
}

static void write_bars(objectstruct *o)
{ long x, i;

  for (x=0, i=0; i<2; x+=b_size, i++)
   b_add_layer(x, 0, intern[METER+i].ptr);

  write_number(7, 7, (tnk.shield.armor*100)/tnk.tank->props.armor);
  write_number(28, 7, (tnk.motion.fuel*100)/tnk.tank->props.max_fuel);
}

static void write_1p_info(void)
{ long x, i;

  for (x=buf_x-2*b_size, i=0; i<2; x+=b_size, i++)
   b_add_layer(x, 0, intern[METER+2+i].ptr);

  write_number(buf_x-59, 7, game.gc_good);
  write_number(buf_x-38, 7, game.gc_bad);
  write_number(buf_x-17, 7, game.id[game.tn]);
}

static void write_lives(objectstruct *o)
{ long i;

  if (o->owner->d.pl.lives!=4)
   for (i=0; i<o->owner->d.pl.lives-1; i++)
    b_add_layer(buf_x-b_size-i*20, 0, intern[TANKICO].ptr);
}

static void write_flags(objectstruct *o)
{ if (tnk.und.flags>0)
   b_add_layer(buf_x-25, -8, intern[VLAGICON+tnk.und.flags-1].ptr);
}

static void write_weapons(objectstruct *o)
{ char str[10];
  long i, j=0;

  for (i=0; i<num_weapon; i++)
   if (tnk.weap.w==i || tnk.weap.nw[i]!=0 || (i==2 && tnk.tank->weap.flame))
   { b_add_layer_l(j, buf_y-11, icons[0][i]);

     if (tnk.weap.w==i)
      b_add_layer(j, buf_y-11, intern[SELECT].ptr);

     j+=9;
   }

  if (map.night)
  {
    b_add_layer_l(j, buf_y-11, icons[0][14]);

    if (tnk.weap.w == num_weapon)
      b_add_layer(j, buf_y-11, intern[SELECT].ptr);
  }

  b_add_layer(buf_x-24, buf_y-27, icons[1][tnk.weap.w==num_weapon? 14:tnk.weap.w]);

  if (tnk.weap.w != 2 && tnk.weap.w != num_weapon)
  { sprintf(str, "%ld", tnk.weap.nw[tnk.weap.w]);
    b_write_text(buf_x-strlen(str)*6-2, buf_y-8, cmap[15], str);
  }
}

static void write_winner_1p(void)
{ long x, x2, y, y2, w=game.winner;

  if (w!=0)
  { x=(buf_x-4*b_size)/2;
    y=(buf_y-3*b_size)/2;

    for (y2=0; y2<2; y2++)
     for (x2=0; x2<4; x2++)
      b_add_layer(x+x2*b_size, y+y2*2*b_size, intern[MISSION+x2+w*y2*4].ptr);
  }
}

static void write_winner_2p(void)
{ long x, x2, y, y2, w=game.winner;

  if (w==game.tn)
  { x=(buf_x-2*b_size)/2;
    y=(buf_y-3*b_size)/2;

    for (y2=0; y2<2; y2++)
     for (x2=0; x2<2; x2++)
      b_add_layer(x+x2*b_size, y+y2*2*b_size, intern[YOU_WIN+x2+y2*2].ptr);
  }
}

void write_timeleft(void)
{
  int timeleft = game.mayhem_time - timer;

  if (!tdone(game.mayhem_time))
    {
      b_write_text(2, 2, cmap[15], "PRODUCTS NEEDED: %d", game.mayhem_products);
      b_write_text(2, 9, cmap[15], "TIME LEFT: %d:%02d.%02d", timeleft / (60 * sec), (timeleft / sec) % 60, timeleft % 100);
      b_write_text(2, 16, cmap[15], "CASH: $%d", game.mayhem_cash);
    }
  else
    modify_goal_counter(-100);
}

void write_products(void)
{
  int i;

  for (i = 0; i < num_products; i++)
  {
    b_add_layer(i*20-6, buf_y-33, intern[PRODUCTS+i].ptr);
    if (numprod[i] != 0)
      b_write_text(i*20+8, buf_y-7, cmap[15], "%d", numprod[i]);
  }
}

void write_status(objectstruct *o)
{
  switch(game.mode)
  { case SINGLE:
     if (game.supermarket_mayhem)
     {
       write_timeleft();
       write_products();
       write_supermarket_message(o);
       write_winner_1p();
     }
     else
     {
       if (o!=NULL)
       { write_bars(o);
	 write_weapons(o);
	 write_message(o);
	 write_winner_1p();
       }
       write_1p_info();
     }
     break;

    case DUAL:
     if (o!=NULL)
     { write_bars(o);
       write_weapons(o);

       if (game.flag_hunt)
	write_flags(o);
	else write_lives(o);

       write_arrow(o);
       write_message(o);
       write_winner_2p();
     }
     break;

    case DUALVC:
     if (game.supermarket_mayhem)
     {
       write_timeleft();
       write_products();
       write_supermarket_message(o);
       write_winner_1p();
     }
     else
     {
       if (o!=NULL)
       { write_bars(o);
	 write_weapons(o);
	 write_message(o);
	 write_winner_1p();
       }
       write_1p_info();
     }
     break;

    case MAP:
     break;
  }
}

void tank_write_marker(objectstruct *o)
{ long l;

  if (!tnk.lock.active) l=2;
  else if (game.mode!=DUAL && !in_radius(tnk.lock.x-o->x, tnk.lock.y-o->y, 400)) l=3;
  else if (nearest_target(tnk.lock.x, tnk.lock.y)!=NULL) l=1;
  else l=0;

  b_add_layer(tnk.lock.x-ssx, tnk.lock.y-ssy, intern[MARKER+l].ptr);
}
