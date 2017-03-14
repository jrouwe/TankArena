/* Creates nice parts which come from explosions. Looks good ! */

#include <tank.h>
#include <math.h>

typedef struct partstr
{ struct partstr *next, *prev;
  long type, pic;
  float x, y, a, d;
  long v, count;
  long nsmoke;
} partsstruct;

typedef struct
{ long str_tnk;
  long str_wall;
  long numpart;
  long min_d, rand_d;
  long min_v, rand_v;
  long pic, rand_pic;
  long smokes;
} partsproperties;

static partsproperties pp[]={{0, 0, 10, 10, 40, 30, 60, 0, 1, FALSE},
			     {7, 10, 10, 50, 20, 30, 20, FLAME, 1, FALSE},
			     {0, 0, 20, 40, 30, 50, 60, 0, 1, FALSE},
			     {0, 0, 10, 50, 20, 30, 20, PARTS, 6, TRUE}};

static partsstruct *parts=NULL;
long max_parts=1000;

static void add_parts_to_list(partsstruct *p)
{ max_parts--;

  p->prev=NULL;
  p->next=parts;

  if (parts!=NULL)
   parts->prev=p;

  parts=p;
}

static void remove_parts_from_list(partsstruct *p)
{ max_parts++;

  if (p->prev!=NULL)
  { p->prev->next=p->next;

    if (p->next!=NULL)
     p->next->prev=p->prev;
  }
  else
  { parts=p->next;

    if (parts!=NULL)
     parts->prev=NULL;
  }
}

void clear_parts(void)
{ partsstruct *p;

  while(parts!=NULL)
  { max_parts++;

    p=parts, parts=parts->next;
    free(p);
  }
}

void create_parts(long type, long x, long y)
{ partsstruct *p;
  long i;

  for (i=0; i<pp[type].numpart; i++)
   if (max_parts>0)
   { p=allocate(sizeof(partsstruct));
     p->type=type;
     p->x=x+rnd(10)-5;
     p->y=y+rnd(10)-5;
     p->a=2*PI*rnd(360)/360;
     p->v=pp[type].min_v+rnd(pp[type].rand_v);
     p->d=pp[type].min_d+rnd(pp[type].rand_d);
     p->pic=pp[type].pic+rnd(pp[type].rand_pic)*5;
     p->count=timer;
     p->nsmoke=pp[type].smokes? 30+rnd(30):tnever;

     add_parts_to_list(p);
   }
}

void control_parts(void)
{ long x, y, x2, y2;
  partsstruct *p, *np;

  for (p=parts; p!=NULL; p=np)
  { np=p->next, x=pround(p->x), y=pround(p->y);

    if (tdone(p->nsmoke))
    { create_effect(x-16, y-16, 8, PARTS_SMOKE);
      p->nsmoke=tinit(20+rnd(20));
    }

    p->d-=add_angle(&p->x, &p->y, p->v, p->a, &p->count);
    x2=pround(p->x), y2=pround(p->y);

    if ((pp[p->type].str_tnk!=0 && line_obj_hit_point(NULL, x, y, &x2, &y2, radtoa16(p->a), HT_PARTS_TNK, pp[p->type].str_tnk)) ||
	(pp[p->type].str_wall!=0 && line_wall_hit_point(NULL, x, y, &x2, &y2, HT_PARTS_WALL, pp[p->type].str_wall)) ||
	p->d<0)
    { remove_parts_from_list(p);
      free(p);
    }
  }
}

void write_parts(void)
{ partsstruct *p;
  long x, y;

  for (p=parts; p!=NULL; p=p->next)
  { x=pround(p->x)-ssx, y=pround(p->y)-ssy;

    switch(p->type)
    { case PT_PIXEL:
      case PT_MORTAR:
       b_pixel(x, y, 0);
       break;

      default:
       b_add_layer(x-16, y-16, eff[p->pic+4-(long)(p->d*5/(pp[p->type].min_d+pp[p->type].rand_d))].ptr);
       break;
    }
  }
}
