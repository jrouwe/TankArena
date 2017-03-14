/* Bullets are stored in a separate list because otherwise they would
   take too much computation time.
*/

#include <tank.h>
#include <math.h>

#define tnk o->d.tk

typedef struct bulletctrl
{ struct bulletctrl *prev, *next;
  objectstruct *owner;
  float x, y, dist, safety;
  long h, power, v, count, dir, type, smoke;
} bulletstruct;

extern unsigned char *rock[2][5];

static bulletstruct *bul=NULL;

static void add_bullet_to_list(bulletstruct *b)
{ b->prev=NULL;
  b->next=bul;

  if (bul!=NULL)
   bul->prev=b;

  bul=b;
}

static void remove_bullet_from_list(bulletstruct *b)
{ if (b->prev!=NULL)
  { b->prev->next=b->next;

    if (b->next!=NULL)
     b->next->prev=b->prev;
  }
  else
  { bul=b->next;

    if (bul!=NULL)
     bul->prev=NULL;
  }
}

void clear_bullets(void)
{ bulletstruct *b;

  while(bul!=NULL)
  { b=bul, bul=bul->next;
    free(b);
  }
}

static void remove_bullet(bulletstruct *b)
{ remove_bullet_from_list(b);
  free(b);
}

void create_bullet(objectstruct *o, long type, long x, long y, long dir, long power, long max, long safety)
{ bulletstruct *b;
  float a;
  long v;

  b=allocate(sizeof(bulletstruct));

  b->x=x, b->y=y, b->dir=dir, b->owner=o, b->v=200;
  b->type=type, b->power=power, b->dist=max, b->smoke=tnever;
  b->safety=safety, b->count=timer;

  switch(type)
  { case HT_FLAME:
    case HT_MORTAR:
     b->v=75;
     b->dist=120;
     break;

    case HT_PLANEBOMB:
     b->dist=10*o->h;
     b->h=o->h;
     b->v=75;
     break;

    case HT_SROCK:
     b->smoke=tnow;

    default:
     if (o->type==TANK_TYPE)
     { a=a16torad(dir), v=b->v;
       b->v=sqrt(sqr(tnk.motion.vx+cos(a)*v)+sqr(tnk.motion.vy-sin(a)*v));

       if (b->v<75) b->v=75;
       b->dist*=(float)b->v/v;
     }
     break;
  }

  switch(type)
  { case HT_MAIN:
     create_effect(x-16, y-16, o->h+1, FIRE);
     create_sound_relative(o, SND_MAIN);
     break;

    case HT_CHAIN:
     if (o->type!=TANK_TYPE)
      create_effect(x-16, y-16, o->h+1, SFIRE);
     create_sound_relative(o, SND_CHAIN);
     break;

    case HT_FLAME:
     create_sound_relative(o, SND_FLAME);
     break;

    case HT_SROCK:
     create_sound_relative(o, SND_SROCKET);
     break;

    case HT_MORTAR:
     create_effect(x-16, y-16, o->h+1, MORTAR_FIRE);
     create_sound_relative(o, SND_MORTAR);
     break;
  }

  add_bullet_to_list(b);
}

void control_bullets(void)
{ bulletstruct *b, *nb;
  long x, y, x2, y2, type;
  objectstruct *p;
  float d;

  for (b=bul; b!=NULL; b=nb)
  { nb=b->next, x2=pround(b->x), y2=pround(b->y), type=b->type;
    d=add_angle(&b->x, &b->y, b->v, a16torad(b->dir), &b->count);
    x=pround(b->x), y=pround(b->y), b->dist-=d, p=b->owner;

    if (b->dist<0 || x<0 || x>b_size*map.sx || y<0 || y>b_size*map.sy)
    { switch(type)
      { case HT_CHAIN:
	 create_effect(x-16, y-16, 1, PLUF);
	 break;

	case HT_MAIN:
	case HT_SROCK:
	 create_effect(x-16, y-16, 1, EXPLOSION);
	 break;

	case HT_MORTAR:
	case HT_PLANEBOMB:
	 create_mortar(b->owner, b->x, b->y);
	 break;
      }

      remove_bullet(b);
      continue;
    }

    if (b->safety>0)
    { b->safety-=d;
      continue;
    }

    if (tdone(b->smoke))
    { create_effect(x2-16, y2-16, 5, EXHAUST+1);
      b->smoke=tinit(5+rnd(5));
    }

    if (type!=HT_MORTAR && type!=HT_PLANEBOMB &&
	(p==NULL || p->type!=TURRET_TYPE || p->owner!=NULL || !in_radius(p->x+16-x, p->y+16-y, 2*b_size))
	&& line_wall_hit_point(b->owner, x2, y2, &x, &y, type, b->power))
    { if (type!=HT_FLAME && strength[0][0][(x/b_size)+(y/b_size)*map.sx]!=0)
      { b->x=x, b->y=y, b->owner=NULL, b->safety=4;

	switch(get_type(x/b_size, y/b_size, 1))
	{ case FIELD_H:
	   b->dir=(8-b->dir)&15;
	   continue;

	  case FIELD_V:
	   b->dir=(0-b->dir)&15;
	   continue;

	  case FIELD_P:
	   b->dir=(4-b->dir)&15;
	   continue;

	  case FIELD_N:
	   b->dir=(12-b->dir)&15;
	   continue;
	}

	if (type!=HT_CHAIN || rnd(5)==0)
	 create_parts(PT_PIXEL, x, y);
      }

      remove_bullet(b);
      continue;
    }

    switch(type)
    { case HT_MORTAR:
      case HT_PLANEBOMB:
       break;

      case HT_FLAME:
       if (line_obj_hit_radius(b->owner, x2, y2, &x, &y, 15, HT_FLAME, b->power))
       { remove_bullet(b);
	 continue;
       }
       break;

      default:
       if (line_obj_hit_point(b->owner, x2, y2, &x, &y, b->dir, type, b->power))
       { if (type!=HT_CHAIN || rnd(5)==0)
	  create_parts(PT_PIXEL, x, y);

	 remove_bullet(b);
	 continue;
       }
       break;
    }
  }
}

void write_bullets(void)
{ long black=cmap_dark[0];
  long white=cmap_dark[15];
  bulletstruct *b;
  long x, y, p, h;

  for (b=bul; b!=NULL; b=b->next)
  { x=pround(b->x), y=pround(b->y);

    switch(b->type)
    { case HT_MAIN:
       b_pixel(x-ssx, y-ssy, black);
       b_pixel(x-ssx+1, y-ssy, black);
       b_pixel(x-ssx, y-ssy+1, black);
       b_pixel(x-ssx-1, y-ssy, black);
       b_pixel(x-ssx, y-ssy-1, black);
       break;

      case HT_CHAIN:
       b_pixel(x-ssx, y-ssy, white);
       break;

      case HT_FLAME:
       b_add_layer(x-ssx-16, y-ssy-16, eff[FLAME+4-(long)b->dist/25].ptr);
       break;

      case HT_SROCK:
       b_add_layer_d(x-ssx-16, y-ssy-16, b->dir, rock[0]);
       break;

      case HT_MORTAR:
       #define mortar_h_max 15
       h=-mortar_h_max*sqr(b->dist)/3600+mortar_h_max*b->dist/30;
       p=MORTAR+4-(long)b->dist/25;
       goto do_write;

      case HT_PLANEBOMB:
       p=MORTAR+(long)b->dist/50;
       h=(b->dist*b->h)/120;

      do_write:;
       b_add_layer(x-ssx-16, y-ssy-16, intern[p].ptr);
       b_add_shadow(x-ssx-16+h, y-ssy-16+2*h, intern[p].ptr);
       break;
    }
  }
}
