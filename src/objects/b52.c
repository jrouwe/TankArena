/* Handles everything about the B52 */

#include <tank.h>

extern char *b52pics[4][3][4];

static void hit_b52(objectstruct *o, long power, long type);

static long bm[10][2]={{-27,17}, {-15,17}, {-21,23}, {-14,28}, {-27,30},
		      {16,17}, {28,20}, {15,24}, {27,29}, {9,32}};

static long ex[4][2]={{-16,9}, {-10,8}, {9,8}, {15,9}};

static long li[2][4][2]={{{1,49}, {16,1}, {1,16}, {49,1}},
			{{65,49}, {16,65}, {65,16}, {49,65}}};

#define b52 o->d.b52
#define bomb_power 20

static void write_b52_wreck(objectstruct *o)
{ long x=o->x-16, y=o->y-16, d=b52.dir, j, k, l;

  if (get_type((o->x+16)/b_size, (o->y+16)/b_size, 0)!=WATER)
   for (l=0, k=y; k<=y+b_size; k+=b_size)
    for (j=x; j<=x+b_size; j+=b_size, l++)
     b_add_layer(j-ssx, k-ssy, b52pics[d][1][l]);
}

static void control_b52_wreck(objectstruct *o)
{ if (!tdone(b52.nexp))
   create_effect(o->x-20+rnd(40), o->y-20+rnd(40), o->h+1, EXPLOSION);
}

static void hit_b52_wreck(objectstruct *o, long power, long type)
{ long i;

  if (tdone(b52.nexp))
  { for (i=0; i<10; i++)
     create_effect(o->x-20+rnd(40), o->y-20+rnd(40), o->h+1, EXPLOSION);

    remove_object(o);
  }
}

static void write_b52(objectstruct *o)
{ long x=o->x-16, y=o->y-16, h=o->h, d=b52.dir, j, k, l;

  for (l=0, k=y; k<=y+b_size; k+=b_size)
   for (j=x; j<=x+b_size; j+=b_size, l++)
   { b_add_shadow(j-ssx+h, k-ssy+2*h, b52pics[d][0][l]);

     if (((d==0 && k!=y) || (d==1 && j==x) || (d==2 && k==y) || (d==3 && j!=x)) && b52.bomb!=4)
      b_add_layer(j-ssx, k-ssy, b52pics[d][2][b52.bomb]);

     b_add_layer(j-ssx, k-ssy, b52pics[d][0][l]);
   }

  if (b52.light==1)
   for (j=0; j<2; j++)
    b_set_layer(x-16+li[j][d][0]-ssx, y-16+li[j][d][1]-ssy, intern[B52_LIGHT].ptr);
}

static void reset_b52(objectstruct *o)
{ b52.dir=rnd(4);

  switch(b52.dir)
  { case 0:
     o->x=rnd(map.sx*b_size);
     o->y=(map.sy+2)*b_size;
     break;

    case 1:
     o->x=-2*b_size;
     o->y=rnd(map.sy*b_size);
     break;

    case 2:
     o->x=rnd(map.sx*b_size);
     o->y=-2*b_size;
     break;

    case 3:
     o->x=(map.sx+2)*b_size;
     o->y=rnd(map.sy*b_size);
     break;
  }

  o->fx=o->x, o->fy=o->y;
}

static void control_b52(objectstruct *o)
{ add_angle(&o->fx, &o->fy, b52.v, a16torad(b52.dir*4), &b52.count);

  o->x=round(o->fx), o->y=round(o->fy);

  check_obj_collide(o, o->x, o->y);

  if (o->h<=3 && check_wall_hit(o, o->x+16, o->y+16, 2, HT_DESTROY, 1000))
   hit_b52(o, 1000, HT_DESTROY);

  if (tdone(b52.nl))
  { b52.light^=1;
    b52.nl=tinit(b52.light==0? 200+rnd(100):10);
  }
}

static void fly_b52(objectstruct *o)
{ long x=o->x, y=o->y, x2, y2, i;
  short *m0, *m1;

  control_b52(o);

  if (x>(map.sx+3)*b_size || x<-3*b_size || y>(map.sy+3)*b_size || y<-3*b_size)
  { reset_b52(o);
    return;
  }

  if (tdone(b52.nb))
  { b52.bomb++;
    if (b52.bomb==5) b52.bomb=0;

    b52.nb=tinit(b52.bomb==4? 500:30);

    if (b52.bomb==4)
     for (i=0; i<10; i++)
     { x2=o->x+loc_x(b52.dir*4, bm[i])+16;
       y2=o->y+loc_y(b52.dir*4, bm[i])+16;

       if (!check_wall_hit_point(o, x2, y2, 2, HT_BOMB, bomb_power) &&
	   !check_obj_hit_point(o, x2, y2, HT_BOMB, bomb_power))
       { create_effect(x2-16, y2-16, o->h-1, EXPLOSION);

	 if (rnd(10)==0)
	 { m0=&mapm.m[0][(x2/b_size)+(y2/b_size)*map.sx];
	   m1=&mapm.m[1][(x2/b_size)+(y2/b_size)*map.sx];

	   if (*m0==-1 && *m1==-1)
	   { *m0=search_type(NORM_KRAT);
	     rewrite_block(x2/b_size, y2/b_size);
	   }
	   else if (*m1==-1 && pic[*m0].type==ROAD)
	   { *m1=search_type(ROAD_KRAT);
	     rewrite_block(x2/b_size, y2/b_size);
	   }
	 }
       }
     }
  }
}

static void lift_b52(objectstruct *o)
{ control_b52(o);

  if (tdone(b52.nv) && b52.v<25)
  { b52.v++;
    b52.nv=tinit(80);
  }

  if (tdone(b52.nh) && b52.v>20)
  { o->h++;
    resort_object(o);
    b52.nh=tinit(60);
  }

  if (o->h==19)
   o->funct.control=fly_b52;
}

static void crash_b52(objectstruct *o)
{ long engine;

  control_b52(o);

  if (tdone(b52.nh))
  { o->h--;
    resort_object(o);
    b52.nh=tinit(60);
  }

  if (o->h<=2)
   create_effect(o->x-20+rnd(40), o->y-20+rnd(40), o->h+1, EXPLOSION);

  if (o->h==0)
  { o->funct.write=write_b52_wreck;
    o->funct.control=control_b52_wreck;
    o->funct.hit=hit_b52_wreck;

    modify_goal_counter(o->gc);
    o->gc=0;

    b52.nexp=tinit(300);

    return;
  }

  if (tdone(b52.nexp))
  { engine=rnd(4);
    create_effect(o->x+loc_x(b52.dir*4, ex[engine]), o->y+loc_y(b52.dir*4, ex[engine]), o->h+1, EXPLOSION);
    b52.nexp=tinit(40);
  }
}

static void hit_b52(objectstruct *o, long power, long type)
{ b52.armor-=power;

  if (b52.armor<=0)
  { b52.armor=0;
    b52.bomb=4;

    o->funct.control=crash_b52;
  }
}

objectstruct *make_b52(long x, long y, long dir)
{ objectstruct *o;

  o=create_object(B52_TYPE, x, y, 1, 20);

  o->funct.control=control_b52;
  o->funct.write=write_b52;
  o->funct.hit=hit_b52;

  b52.v=0;
  b52.dir=dir;
  b52.count=timer;
  b52.bomb=4;
  b52.armor=30;
  b52.nh=tnow;
  b52.nb=tnow;
  b52.nv=tnow;
  b52.nexp=tnow;
  b52.light=0;

  return(o);
}

void prepare_b52(objectstruct *o)
{ o->funct.control=lift_b52;
  o->funct.write=write_b52;
  o->funct.hit=hit_b52;

  b52.nv=tinit(500+rnd(500));
  b52.nl=map.night? tnow:tnever;
}
