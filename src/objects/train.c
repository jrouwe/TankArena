/* Controls the train, and makes it drive in one nice line. */

#include <tank.h>
#include <math.h>

#define trn o->d.tr
#define frontp o->d.tr.info.front
#define rearp o->d.tr.info.rear

#define rails_radius 	49
#define wagon_distance 	31
#define max_speed 	50

unsigned char *train[num_wagons+1][5];

long smoke[]={0,-11};

static void write_train(objectstruct *o)
{ b_add_layer_d(o->x-ssx, o->y-ssy, trn.motion.dir, train[trn.info.picture]);
}

static void mwrite_train(objectstruct *o, objectstruct *ao)
{ if (ao==o)
  { if (frontp!=NULL)
     draw_line(o->x, o->y, frontp->x, frontp->y, 15);

    if (rearp!=NULL)
     draw_line(o->x, o->y, rearp->x, rearp->y, 15);
  }
}

static void change_speed(objectstruct *o, long nv)
{ long dc, v=trn.motion.v;

  trn.motion.v=nv;

  if (nv==0) return;

  if (v==0) trn.motion.count=timer;

  dc=timer-trn.motion.count;

  dc=(float)dc*v/nv;

  trn.motion.count=timer-dc;
}

static void setup_new_rails(objectstruct *o, long srx, long sry)
{ if (srx<0) srx=map.sx-1;
  if (srx>=map.sx) srx=0;
  if (sry<0) sry=map.sy-1;
  if (sry>=map.sy) sry=0;

  trn.end.srx=srx;
  trn.end.sry=sry;

  switch (get_type(srx, sry, 0))
  { case RAILS_H:
    case BRIDGE_H:
     trn.rails.type=RAILS_HORIZ;
     trn.rails.dir=trn.end.dir;
     trn.rails.start=trn.end.dir==1? -16:16;
     trn.rails.srx=srx*b_size+16;
     trn.rails.sry=sry*b_size+16;

     trn.end.srx+=trn.rails.dir;
     break;

    case RAILS_V:
    case RAILS_S_B:
    case RAILS_S_D:
    case RAILS_W_A:
    case RAILS_W_C:
    case BRIDGE_V:
     trn.rails.type=RAILS_VERT;
     trn.rails.dir=trn.end.dir;
     trn.rails.start=trn.end.dir==1? -16:16;
     trn.rails.srx=srx*b_size+16;
     trn.rails.sry=sry*b_size+16;

     trn.end.sry+=trn.rails.dir;
     break;

    case RAILS_CR:
     trn.rails.type=trn.end.type;
     trn.rails.dir=trn.end.dir;
     trn.rails.start=trn.end.dir==1? -16:16;
     trn.rails.srx=srx*b_size+16;
     trn.rails.sry=sry*b_size+16;

     if (trn.rails.type==RAILS_HORIZ)
      trn.end.srx+=trn.rails.dir;
      else trn.end.sry+=trn.rails.dir;
     break;

    case RAILS_TL_B:
    case RAILS_W_B:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=0.5*PI;
     trn.rails.srx=srx*b_size+b_size;
     trn.rails.sry=sry*b_size+2*b_size;
     trn.rails.dir=-1;

     trn.end.type=RAILS_VERT;
     trn.end.dir=1;
     trn.end.srx=srx-1;
     trn.end.sry=sry+2;
     break;

    case RAILS_TL_C:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=PI;
     trn.rails.srx=srx*b_size+2*b_size;
     trn.rails.sry=sry*b_size+b_size;
     trn.rails.dir=1;

     trn.end.type=RAILS_HORIZ;
     trn.end.dir=1;
     trn.end.srx=srx+2;
     trn.end.sry=sry-1;
     break;

    case RAILS_TR_A:
    case RAILS_S_A:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=0.5*PI;
     trn.rails.srx=srx*b_size;
     trn.rails.sry=sry*b_size+2*b_size;
     trn.rails.dir=1;

     trn.end.type=RAILS_VERT;
     trn.end.dir=1;
     trn.end.srx=srx+1;
     trn.end.sry=sry+2;
     break;

    case RAILS_TR_D:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=0;
     trn.rails.srx=srx*b_size-b_size;
     trn.rails.sry=sry*b_size+b_size;
     trn.rails.dir=-1;

     trn.end.type=RAILS_HORIZ;
     trn.end.dir=-1;
     trn.end.srx=srx-2;
     trn.end.sry=sry-1;
     break;

    case RAILS_BL_A:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=PI;
     trn.rails.srx=srx*b_size+2*b_size;
     trn.rails.sry=sry*b_size;
     trn.rails.dir=-1;

     trn.end.type=RAILS_HORIZ;
     trn.end.dir=1;
     trn.end.srx=srx+2;
     trn.end.sry=sry+1;
     break;

    case RAILS_BL_D:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=1.5*PI;
     trn.rails.srx=srx*b_size+b_size;
     trn.rails.sry=sry*b_size-b_size;
     trn.rails.dir=1;

     trn.end.type=RAILS_VERT;
     trn.end.dir=-1;
     trn.end.srx=srx-1;
     trn.end.sry=sry-2;
     break;

    case RAILS_BR_B:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=0;
     trn.rails.srx=srx*b_size-b_size;
     trn.rails.sry=sry*b_size;
     trn.rails.dir=1;

     trn.end.type=RAILS_HORIZ;
     trn.end.dir=-1;
     trn.end.srx=srx-2;
     trn.end.sry=sry+1;
     break;

    case RAILS_BR_C:
     trn.rails.type=RAILS_CIRCLE;
     trn.rails.start=1.5*PI;
     trn.rails.srx=srx*b_size;
     trn.rails.sry=sry*b_size-b_size;
     trn.rails.dir=-1;

     trn.end.type=RAILS_VERT;
     trn.end.dir=-1;
     trn.end.srx=srx+1;
     trn.end.sry=sry-2;
     break;

    default:
     o->funct.control=NULL;
     trn.motion.v=0;
     break;
  }
}

static void control_train(objectstruct *o)
{ long v=trn.motion.v, dir=trn.rails.dir, tdir=trn.motion.dir;
  long d, dc, cc, c1=timer, c2=trn.motion.count, bx, by;
  float a=0, a2, x=0, y=0;

  if (o->type!=TRAIN_TYPE && o->type!=WAGON_TYPE)
   error("Error in train construction.\n\n"
	 "A train in this map was not created in one piece and therefore\n"
	 "corrupts other objects. Please remove this train and refer to\n"
	 "the manual for creating a new one.\n\n");

  if (v>0)
  { switch(trn.rails.type)
    { case RAILS_HORIZ:
       x=(float)v*(c1-c2)/sec;

       o->x=trn.rails.srx+trn.rails.start+dir*x-16;
       o->y=trn.rails.sry-16;

       trn.motion.dir=trn.rails.dir==1? 4:12;

       break;

      case RAILS_VERT:
       y=(float)v*(c1-c2)/sec;

       o->x=trn.rails.srx-16;
       o->y=trn.rails.sry+trn.rails.start+dir*y-16;

       trn.motion.dir=trn.rails.dir==1? 8:0;

       break;

      case RAILS_CIRCLE:
       a=(float)v*(c1-c2)/sec/rails_radius;

       a2=trn.rails.start-dir*a;

       trn.motion.dir=radtoa16(in2pi(a2+(dir==1? 1.5*PI:0.5*PI)));

       o->x=trn.rails.srx+rails_radius*cos(a2)-16;
       o->y=trn.rails.sry-rails_radius*sin(a2)-16;

       break;
    }

    if (a>0.51*PI || x>=33 || y>=33)
    { trn.motion.count=c1;

      setup_new_rails(o, trn.end.srx, trn.end.sry);
    }
  }

  if (frontp!=NULL && v>0)
  { d=distance(o->x-frontp->x, o->y-frontp->y)-wagon_distance;
    dc=c1-c2, cc=(float)abs(d)*sec/v/10;

    if (abs(d)<100)
    { if (d<-2 && dc>cc) trn.motion.count+=cc;
      else if (d>2) trn.motion.count-=cc;
    }
  }

  check_obj_collide(o, o->x, o->y);

  if (overlap(o, intern[OVERLAP_BLOCK].ptr, o->x, o->y, &bx, &by)>0)
  { if (strength[0][0][bx+map.sx*by]==1000)
     remove_object(o);

    else
    { hit_wall_for(o, bx*b_size, by*b_size, 0, HT_TRAIN, 1000);

      v=v<25? 5:v-20;
      change_speed(o, v);
    }
  }

  if (tdone(trn.gen.nsmoke))
  { trn.gen.nsmoke=tinit(20+rnd(20));

    create_effect(o->x+loc_x(tdir, smoke)-2+rnd(4), o->y+loc_y(tdir, smoke)-2+rnd(4), 2, ROOK);
  }

  if (trn.info.turret!=NULL)
   trn.info.turret->x=o->x, trn.info.turret->y=o->y;

  if (frontp!=NULL)
   change_speed(o, frontp->d.tr.motion.v);

  else if (tdone(trn.gen.nv))
  { switch(o->type)
    { case TRAIN_TYPE:
       v=v<max_speed? v+5:max_speed;
       break;

      case WAGON_TYPE:
       v=v>5? v-5:0;
       break;
    }

    change_speed(o, v);
    trn.gen.nv=tinit(100);
  }

  if (rearp!=NULL)
   control_train(rearp);
}

static void hit_train(objectstruct *o, long power, long type)
{ o->d.tr.gen.armor-=power;

  if (o->d.tr.gen.armor<=0)
   remove_object(o);
}

static void remove_train(objectstruct *o)
{ long i;

  if (trn.info.turret!=NULL)
   remove_object(trn.info.turret);

  if (trn.info.picture==7)
   create_abomb(o, o->x, o->y, 0);

  for (i=0; i<5; i++)
   create_effect(o->x-8+rnd(16), o->y-8+rnd(16), 2, EXPLOSION);

  if (frontp!=NULL)
   frontp->d.tr.info.rear=NULL;

  if (rearp!=NULL)
  { rearp->funct.control=control_train;
    rearp->d.tr.info.front=NULL;
  }
}

void train_change(objectstruct *o)
{ long x=trn.info.x, y=trn.info.y;

  switch(trn.motion.dir)
  { case 0:
     o->x=x*b_size;
     o->y=y*b_size+16;
     trn.rails.type=RAILS_VERT;
     trn.rails.dir=-1;
     trn.rails.start=16;
     trn.end.srx=x;
     trn.end.sry=y-1;
     break;

    case 4:
     o->x=x*b_size-16;
     o->y=y*b_size;
     trn.rails.type=RAILS_HORIZ;
     trn.rails.dir=1;
     trn.rails.start=-16;
     trn.end.srx=x+1;
     trn.end.sry=y;
     break;

    case 8:
     o->x=x*b_size;
     o->y=y*b_size-16;
     trn.rails.type=RAILS_VERT;
     trn.rails.dir=1;
     trn.rails.start=-16;
     trn.end.srx=x;
     trn.end.sry=y+1;
     break;

    case 12:
     o->x=x*b_size+16;
     o->y=y*b_size;
     trn.rails.type=RAILS_HORIZ;
     trn.rails.dir=-1;
     trn.rails.start=16;
     trn.end.srx=x-1;
     trn.end.sry=y;
     break;
  }

  trn.rails.srx=x*b_size+16;
  trn.rails.sry=y*b_size+16;
  trn.end.dir=trn.rails.dir;
  trn.end.type=trn.rails.type;

  if (trn.info.turret!=NULL)
   trn.info.turret->x=o->x, trn.info.turret->y=o->y;
}

objectstruct *make_train(long x, long y, long type, objectstruct *f)
{ objectstruct *o;

  o=create_object(type==0? TRAIN_TYPE:WAGON_TYPE, x, y, 1, 12);

  if (type==0)
   o->funct.control=control_train;

  o->funct.write=write_train;
  o->funct.mwrite=mwrite_train;
  o->funct.hit=hit_train;
  o->funct.remove=remove_train;

  trn.motion.v=0;
  trn.motion.dir=type==0 || f==NULL? 0:f->d.tr.motion.dir;

  if (f!=NULL)
   f->d.tr.info.rear=o;

  trn.gen.nsmoke=type==0? tnow:tnever;
  trn.gen.armor=type==0? 60:40;

  trn.info.x=x;
  trn.info.y=y;

  trn.info.picture=type;
  trn.info.turret=type==8? make_turret(o->x, o->y, 2, 2, 5, o):NULL;
  frontp=f;
  rearp=NULL;

  train_change(o);

  return(o);
}

void prepare_train(objectstruct *o)
{ if (o->funct.control!=NULL)
   o->funct.control=control_train;

  o->funct.write=write_train;
  o->funct.mwrite=mwrite_train;
  o->funct.hit=hit_train;
  o->funct.remove=remove_train;

  trn.gen.nv=tinit(100);

  relocate(&trn.info.turret);
  relocate(&frontp);
  relocate(&rearp);
}
