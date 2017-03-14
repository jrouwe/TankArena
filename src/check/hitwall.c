/* Functions to check and handle when things hit structures (buildings and
   such).
*/

#include <tank.h>
#include <math.h>

extern unsigned char *pic_data;

hitinfotype hit[]={{EXPLOSION, 0},     // HT_MAIN
		   {CHAIN, 0},	       // HT_CHAIN
		   {FLAME, 2},	       // HT_FLAME
		   {LEXPLOSION, 0},    // HT_ROCKET
		   {EXPLOSION, 0},     // HT_ABOMB
		   {RUIN, 1},	       // HT_CRASH
		   {EXPLOSION, 0},     // HT_MINE
		   {EXPLOSION, 0},     // HT_BOMB
		   {EXPLOSION, 0},     // HT_DESTROY
		   {RUIN, 0},	       // HT_TANK_CRASH
		   {EXPLOSION, 0},     // HT_SROCK
		   {CHAIN, 0},	       // HT_PARTS_TNK
		   {EXPLOSION, 0},     // HT_PARTS_WALL
		   {EXPLOSION, 0},     // HT_TRAIN
		   {EXPLOSION, 0},     // HT_MORTAR
		   {EXPLOSION, 0},     // HT_REMOVE
		   {EXPLOSION, 0},     // HT_PLANEBOMB
		   {LAVA_EFFECT, 0}};  // HT_LAVA

static void determine_hit(long sx, long sy, char **p1, char **p2, long *layer, long exclude)
{ long m;

  *p1=NULL, *p2=NULL;

  if (sx<0 || sx>=map.sx || sy<0 || sy>=map.sy)
   return;

  if (exclude!=0 && strength[0][0][sx+map.sx*sy]!=0 && (m=mapm.m[1][sx+map.sx*sy])!=-1)
   *p2=pic[m].ptr[0];

  if (exclude!=1 && strength[1][0][sx+map.sx*sy]!=0 && (m=mapm.m[2][sx+map.sx*sy])!=-1)
   *p1=pic[m].ptr[0], *layer=1;

  else if (exclude!=1 && strength[0][0][sx+map.sx*sy]!=0 && (m=mapm.m[2][sx+map.sx*sy])!=-1)
   *p1=pic[m].ptr[1], *layer=0;

  else if (exclude!=0)
   *p1=*p2, *layer=0;
}

long check_wall_hit_point(objectstruct *owner, long x, long y, long exclude, long type, long power)
{ long sx, sy, bx, by;
  long layer;
  char *p, *p2;

  sx=x/b_size, bx=x%b_size;
  sy=y/b_size, by=y%b_size;

  determine_hit(sx, sy, &p, &p2, &layer, exclude);

  if ((p!=NULL && p[bx+by*b_size]!=-1) || (p2!=NULL && p2[bx+by*b_size]!=-1))
  { hit_wall_for(owner, x, y, layer, type, power);
    return(TRUE);
  }

  return(FALSE);
}

long line_wall_hit_point(objectstruct *owner, long x1, long y1, long *x2, long *y2, long type, long power)
{ long rx, ry, srx, sry, rxrm, ryrm, steps, max, i;
  long sx, sy, bx, by, bxrm, byrm;
  long layer;
  char *p, *p2;

  rx=*x2-x1, srx=sign(rx);
  ry=*y2-y1, sry=sign(ry);

  steps=abs(rx)>abs(ry)? abs(rx):abs(ry);
  if (steps==0) steps=1;

  max=steps/2;

  rxrm=abs(rx%steps), rx/=steps;
  ryrm=abs(ry%steps), ry/=steps;

  sx=x1/b_size, bx=x1%b_size, bxrm=0;
  sy=y1/b_size, by=y1%b_size, byrm=0;

  determine_hit(sx, sy, &p, &p2, &layer, 2);

  for (i=0; i<steps; i++)
  { bx+=rx, by+=ry;
    bxrm+=rxrm, byrm+=ryrm;

    if (bxrm>max) bxrm-=steps, bx+=srx;
    if (byrm>max) byrm-=steps, by+=sry;

    if (bx>=b_size)
    { sx++, bx-=b_size;
      determine_hit(sx, sy, &p, &p2, &layer, 2);
    }
    else if (bx<0)
    { sx--, bx+=b_size;
      determine_hit(sx, sy, &p, &p2, &layer, 2);
    }

    if (by>=b_size)
    { sy++, by-=b_size;
      determine_hit(sx, sy, &p, &p2, &layer, 2);
    }
    else if (by<0)
    { sy--, by+=b_size;
      determine_hit(sx, sy, &p, &p2, &layer, 2);
    }

    if ((p!=NULL && p[bx+by*b_size]!=-1) || (p2!=NULL && p2[bx+by*b_size]!=-1))
    { *x2=sx*b_size+bx, *y2=sy*b_size+by;
      hit_wall_for(owner, *x2, *y2, layer, type, power);
      return(TRUE);
    }
  }

  return(FALSE);
}

long check_wall_hit(objectstruct *owner, long x, long y, long exclude_layer, long type, long power)
{ long sx=x/b_size, sy=y/b_size, layer;

  if ((sx<0 || sx>=map.sx || sy<0 || sy>=map.sy))
   return(FALSE);

  for (layer=1; layer>=0; layer--)
  { if (layer==exclude_layer) continue;

    if (strength[layer][0][sx+map.sx*sy]!=0)
    { hit_wall_for(owner, x, y, layer, type, power);
      return(TRUE);
    }
  }

  return(FALSE);
}

void hit_wall_big(objectstruct *owner, long x, long y, long r, long exclude_layer, long type, long power)
{ long i, x2, y2;
  float angle;

  for (x2=x-r; x2<x+r; x2+=b_size)
   for (y2=y-r; y2<y+r; y2+=b_size)
    if (in_radius(x2-x, y2-y, r + 16))
     check_wall_hit(owner, x2, y2, exclude_layer, type, power);

  for (i=0; i<5; i++)
  { angle=2*PI*rnd(360)/360;
    create_effect(x-16+r*cos(angle), y-16+r*sin(angle), 24, EXPLOSION);
  }
}

static void hit_wall(objectstruct *owner, long from, long x, long y, long layer, long type, long new_str)
{ long t, x2, y2, m, p, explosion, i;

  if (x<0 || x>=map.sx || y<0 || y>=map.sy)
   return;

  if ((m=mapm.m[layer+1][x+map.sx*y])==-1)
   return;

  p=pic[m].linked;
  i=strength[layer][hit[type].kind][x+map.sx*y];
  t=get_type(x, y, 1);
  
  if ((from!=NONE && (p&STOP)==STOP) ||
      (t==BUSHES && from!=NONE) ||
      (type!=HT_REMOVE && i==1000) ||
      i==new_str || i==0)
   return;

  x2=x*b_size, y2=y*b_size;

  strength[layer][hit[type].kind][x+map.sx*y]=new_str;

  if ((p&LEFT)==LEFT)
   hit_wall(owner, RIGHT, x-1, y, layer, type, new_str);

  if ((p&RIGHT)==RIGHT)
   hit_wall(owner, LEFT, x+1, y, layer, type, new_str);

  if ((p&UP)==UP)
   hit_wall(owner, DOWN, x, y-1, layer, type, new_str);

  if ((p&DOWN)==DOWN)
   hit_wall(owner, UP, x, y+1, layer, type, new_str);

  if (new_str==0)
  { strength[layer][0][x+map.sx*y]=0;
      
    if (type!=HT_ABOMB && type!=HT_BOMB && t!=BUSH_SIDE)
    {
      create_sound_absolute(x2-16, y2-16, SND_COLLAPSE+rnd(2));

      if (pic[mapm.m[layer+1][x+map.sx*y]].exptype!=RUIN)
       explosion=pic[mapm.m[layer+1][x+map.sx*y]].exptype;
       else explosion=hit[type].explosion;

      if (explosion==CHAIN) explosion=RUIN;
    
      switch(explosion)
      { case OIL_SPLASH:
	 create_effect(x2, y2, 0, OIL_SPLASH);
	 break;

	default:
	 for (i=0; i<5; i++)
	  create_effect(x2+rnd(26)-13, y2+rnd(26)-13, 5, explosion);
	 break;
      }
    }

    if (layer==0)
    { strength[1][0][x+map.sx*y]=0;

      modify_goal_counter(mapm.g[x+map.sx*y]);

      create_bonus(x, y);

      for (i=0; i<mapm.man[1][x+map.sx*y]; i++)
       if (owner==NULL)
	make_man(NULL, x2-16+rnd(32), y2-16+rnd(32), x2-100+rnd(200), y2-100+rnd(200), mapm.man[0][x+map.sx*y], mapm.man[0][x+map.sx*y]==HT_SROCK? 5:1);
	else make_man(NULL, x2-16+rnd(32), y2-16+rnd(32), owner->x, owner->y, mapm.man[0][x+map.sx*y], mapm.man[0][x+map.sx*y]==HT_SROCK? 5:1);

      switch(t)
      { case BRIDGE:
	 mapm.m[0][x+map.sx*y]=-1;
	 break;

	case BRIDGE_H:
	case BRIDGE_V:
	case BRIDGE_CR:
	 mapm.m[0][x+map.sx*y]=search_type(WATER);
	 mapm.m[1][x+map.sx*y]=-1;
	 break;

	case FUEL_DUMP:
	 create_parts(PT_FLAME, x2+16, y2+16);
	 break;

	case ABOX:
	 create_abomb(0, x2+16, y2+16, 0);
	 break;

	default:
	 if (pic[m].exptype==EXPLOSION)
	  create_parts(PT_PARTS, x2+16, y2+16);
	 break;
      }

      rewrite_block(x, y);
    }
  }
}

void hit_wall_for(objectstruct *owner, long x, long y, long layer, long type, long power)
{ long sx, sy, str;

  sx=x/b_size, sy=y/b_size;
  str=strength[layer][hit[type].kind][sx+map.sx*sy]-power;

  if (type!=HT_ABOMB && type!=HT_CRASH)
  { if (type==HT_CHAIN)
     create_effect(x+rnd(8)-20, y+rnd(8)-20, 5, hit[type].explosion);
     else create_effect(x-16, y-16, 5, hit[type].explosion);
  }

  switch(type)
  { case HT_CHAIN:
     if (rnd(10)==0)
      create_sound_absolute(x-16, y-16, SND_RICOCHETS+rnd(6));
     break;

    case HT_TANK_CRASH:
    case HT_CRASH:
    case HT_FLAME:
    case HT_ABOMB:
    case HT_MORTAR:
     break;

    default:
     create_sound_absolute(x-16, y-16, SND_EXPLODE+rnd(2));
     break;
  }

  hit_wall(owner, NONE, sx, sy, layer, type, str<0? 0:str);
}
