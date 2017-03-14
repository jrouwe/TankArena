/* Fuzzy logic routines to control a computer tank. I actually made
   this as an assignment for school and my teacher was quite happy
   with it. (I'm a little bit less happy about it because it takes
   too much computation time and the tanks are quite stupid.)
*/

#include <tank.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

/* Fuzzy scale. */

#define F_TRUE		255
#define F_HALF		127
#define F_TWOTHIRD	170
#define F_THREEQ        191
#define F_FALSE 	0

/* The fuzzy operators. */

#define NOT(x)		    (F_TRUE-(x))
#define AND(x, y)	    (min(x, y))
#define OR(x, y)	    (max(x, y))

void MODIFY(long *x, long y)
{
  *x += y;

  if (*x < F_FALSE) *x = F_FALSE;
  if (*x > F_TRUE) *x = F_TRUE;
}

#define AND3(x,y,z)	    (AND(AND(x, y), z))
#define AND4(a,b,c,d)	    (AND(AND3(a,b,c),d))
#define AND5(a,b,c,d,e)     (AND(AND4(a,b,c,d),e))
#define AND6(a,b,c,d,e,f)   (AND(AND5(a,b,c,d,e),f))

#define OR3(x,y,z)	    (OR(OR(x, y), z))
#define OR4(a,b,c,d)	    (OR(OR3(a,b,c),d))
#define OR5(a,b,c,d,e)	    (OR(OR4(a,b,c,d),e))
#define OR6(a,b,c,d,e,f)    (OR(OR5(a,b,c,d,e),f))

#define DEG_OF_SUP(p,v)     (((p)*(v))/100)

/* Location of this tank. */

#define tnk		o->d.tk
static long		tnk_x,
			tnk_y,

/* The distance the tank will travel in 1s. Minimal distance is 50 pixels. */

			tnk_range;

/* Location of target, and target itself. */

static objectstruct    *trg;
static long		trg_x,
			trg_y,
			trg_dir;
			
/* Location of next waypoint. */

static long		way_x,
			way_y;

/* Define a structure for our fuzzy position determining. */

typedef struct
{
  /* Is this object close? */

  long close;

  /* Where is it? */

  long left,
       front,
       right;

} fuzzy_position;

static fuzzy_position object, building, pit, danger, goal, fast_terrain, slow_terrain, wayp;

/* Various fuzzy variables. */

static long		want_to_follow_goal,
			want_to_follow_fast_terrain,
			want_to_follow_waypoint,
			steering_required,
			want_left,
			want_right;

/* Current tank. */

static objectstruct *o;

/**************************************************** DETERMINE SOME TARGETS ***************************************************/

/* This function determines a new target, it chooses from: enemy tanks, objects with a goal value, and structures.
   This is basically done by comparing the distance to each object, and scale it with the preferences the user
   indicated.
*/

static void determine_new_target()
{
  long cx, cy, mx, my, x, y, d, dtmp;
  objectstruct *p;

  trg=NULL, trg_x=-1, trg_y=-1, d=MAX_LONG;
  tnk.computer.target_way=FALSE;

  /* Make computer tank inactive if "stay_active" flag is not set. */

  if (!tnk.computer.stay_active)
    tnk.computer.active=FALSE;

  /* Go trough list. */

  for (p=obj; p!=NULL; p=p->next)
    if (p != o)
      switch (p->type)
	{
	  case TANK_TYPE:
	    /* (Re)Activate if enemy tank is in range. */

	   if (!tnk.computer.active && p->d.tk.shield.armor>0
	       && ((tnk.computer.activator==0 && p->d.tk.contr.good) || (tnk.computer.activator==1 && !p->d.tk.contr.good))
	       && in_radius(tnk_x-p->x, tnk_y-p->y, tnk.computer.activation_rad))
	     tnk.computer.active=TRUE;

	   /* Make this tank the target if it is closest. */

	   if (tnk.computer.attack_tanks>0 && p->d.tk.shield.armor>0 &&
	       ((tnk.contr.good && !p->d.tk.contr.good) || (!tnk.contr.good && p->d.tk.contr.good)))
	     {
	       dtmp = (distance(tnk_x-p->x, tnk_y-p->y)*(150-tnk.computer.attack_tanks))/50;
	       if (dtmp<d) trg=p, trg_x=p->x, trg_y=p->y, d=dtmp;
	     }
	   break;

	  case GOAL_TYPE:
	    /* Check if this is a goal for the current tank. */

	    if (p->d.go.who==3 && p->target!=o)
	      break;

	  default:
	    /* Check if this object has a goal value. */

	    if (tnk.computer.attack_goals>0
		&& ((tnk.contr.good && p->gc>0) || (!tnk.contr.good && p->gc<0)))
	      {
		dtmp = (distance(tnk_x-p->x, tnk_y-p->y)*(150-tnk.computer.attack_goals))/50;
		if (dtmp<d) trg=p, trg_x=p->x, trg_y=p->y, d=dtmp;
	      }
	    break;
	}

  /* Check for structure goals. */

  if (tnk.computer.attack_goals>0)
    {
      mx = (tnk_x+16)/b_size;
      my = (tnk_y+16)/b_size;

      for (y=my-15, cy=0; cy<30 && y<map.sy; y++, cy++)
	for (x=mx-15, cx=0; cx<30 && x<map.sx; x++, cx++)
	  if (x>=0 && y>=0 && strength[0][0][x+map.sx*y]!=0
	      && ((tnk.contr.good && mapm.g[x+map.sx*y]>0) || (!tnk.contr.good && mapm.g[x+map.sx*y]<0)))
	    {
	      dtmp=(distance(x-mx, y-my)*b_size*(150-tnk.computer.attack_goals))/50;
	      if (dtmp<d) trg=NULL, trg_x=x*b_size, trg_y=y*b_size, d=dtmp;
	    }
    }
}

/* Determine the next waypoint, if waypoint is reached. */

static void determine_next_waypoint(void)
{
  long c, circ;

  if (tnk.way.type==WAY_INACTIVE || !in_radius(way_x-tnk_x, way_y-tnk_y, 30))
     return;

  c = tnk.way.current, circ = tnk.way.type==WAY_CIRCULAR;

  do c = (circ? (c+1) % 10:c+1); while (c < 10 && tnk.way.point[c].x == -1);
	    
  if (c == 10)
    tnk.way.type = WAY_INACTIVE;
  else
    tnk.way.current = c;
}

/**************************************************** SUBROUTINES FOR FUZZY COMPUTATION ****************************************/

/* Determine if an object is close. If d < min then this function returns F_TRUE, and for
   min < d < max it will gradually go to F_FALSE.
*/

static long object_close(long d, long min, long max)
{
  if (d < min)
    return(F_TRUE);

  if (d > max)
    return(F_FALSE);

  return (F_TRUE - ((d-min)*F_TRUE)/(max-min));
}

/* Determine the fuzzy position of an object. */

#define FRONT_CRIT 20
#define SIDE_CRIT 40

static void angle_to_fuzzy_direction(int dx, int dy, int crit_angle, fuzzy_position *f)
{
  long phi, t;

  phi = (a16torad(tnk.motion.dir)  - angle(dx, dy))/PI*180;
  if (phi < -180) phi += 360;
  if (phi > 180) phi -= 360;

  if (phi >= -FRONT_CRIT && phi <= FRONT_CRIT)
    f->front = ((FRONT_CRIT-abs(phi))*F_TRUE)/FRONT_CRIT;
  else
    f->front = F_FALSE;

  t = phi<0? phi+360:phi;

  if (t < crit_angle)
    {
      if (t < SIDE_CRIT)
	f->right = (t*F_TRUE)/SIDE_CRIT;
      else if (t > crit_angle - SIDE_CRIT)
	f->right = ((crit_angle-t)*F_TRUE)/SIDE_CRIT;
      else
	f->right = F_TRUE;
    }
  else
    f->right = F_FALSE;

  t = abs(phi>0? phi-360:phi);

  if (t < crit_angle)
    {
      if (t < SIDE_CRIT)
	f->left = (t*F_TRUE)/SIDE_CRIT;
      else if (t > crit_angle - SIDE_CRIT)
	f->left = ((crit_angle-t)*F_TRUE)/SIDE_CRIT;
      else
	f->left = F_TRUE;
    }
  else
    f->left = F_FALSE;
}

/* This function checks for a near object in the list. */

static void fdetermine_object()
{
  fuzzy_position f;
  objectstruct *q;

  /* First set all values to false. */

  object.close=F_FALSE;
  object.left=F_FALSE;
  object.front=F_FALSE;
  object.right=F_FALSE;

  /* Scan trough object list. */

  for (q = obj; q != NULL; q = q->next)
    if (q != o && q->r > 0 && abs(o->h-q->h)<2 && q != trg)
      {
	angle_to_fuzzy_direction(q->x-tnk_x, q->y-tnk_y, 90, &f);

	/* The term "close" is related to how far away the object is located away in time. */

	f.close = object_close(distance(q->x-tnk_x, q->y-tnk_y), 20, tnk_range);
	
	object.left = OR(object.left, AND(f.close, f.left));
	object.front = OR(object.front, AND(f.close, f.front));
	object.right = OR(object.right, AND(f.close, f.right));

	/* This variable we can use for collision detection. We only check for the
	   distance of objects in front of us.
	*/

	object.close = object.front;
      }
}

/* This function returns a fuzzy value which becomes true when something checked
   by "func" gets very close in the direction "dir".
*/

static long fdetermine_close(int (*func)(int, int, int), int dir, int range, int steps)
{
  float xm, ym;
  long i;

  xm = cos(a16torad(dir)) * range / F_TRUE;
  ym = sin(a16torad(dir)) * range / F_TRUE;

  for (i = F_TRUE/steps; i <= F_TRUE; i += F_TRUE/steps)
    if (func(dir, tnk_x+xm*i, tnk_y-ym*i))
      return F_TRUE + F_TRUE/steps - i;

  return 0;
}

/* This function returns the average value returned by "func"
   in the direction "dir" and with range "range".
*/

static long determine_average(int (*func)(int, int, int), int dir, int range, int steps)
{
  float xm, ym;
  long i, v;

  xm = cos(a16torad(dir));
  ym = sin(a16torad(dir));

  v = 0;

  for (i = range/steps; i <= range; i += range/steps)
    v += func(dir, tnk_x+xm*i, tnk_y-ym*i);

  return v / steps;
}

/**************************************************** COMPUTATION OF FUZZY INPUT VARIABLES *************************************/

/* This function checks if a building or other structure is getting close. */

static int check_building(int dir, int x, int y)
{
  long xx, yy;

  return overlap(o, tnk.tank->pic[dir], x, y, &xx, &yy) > 0
	 && ((trg_x == -1 && trg_y == -1)
	     || (xx != (trg_x+16)/b_size || yy != (trg_y+16)/b_size));
}

static void fdetermine_building()
{
  /* Directoin of the tank running from 0 to 15. */

  int dir = tnk.motion.dir;

  /* Check in specific directions. */

  building.left = OR(fdetermine_close(check_building, (dir-4)&15, 50, 4),
		     fdetermine_close(check_building, (dir-1)&15, tnk_range, 4));

  building.front = fdetermine_close(check_building, dir, tnk_range, 8);

  building.right = OR(fdetermine_close(check_building, (dir+1)&15, tnk_range, 4),
		      fdetermine_close(check_building, (dir+4)&15, 50, 4));

  /* This value is for determining the collision danger, so we set it to
     the "danger" value in front of us.
  */

  building.close = building.front;
}

/* This function checks the presence of a pit (or something equally bad)
   in the neighbourhood.
*/

static int check_pit(int dir, int x, int y)
{
  return o->h == 1 && (in_pit(x, y) || in_lava(x, y));
}

static void fdetermine_pit()
{
  /* Direction of the tank, running from 0 to 15. */

  int dir = tnk.motion.dir;

  /* Check in specific directions. */

  pit.left = OR(fdetermine_close(check_pit, (dir-4)&15, 50, 5),
		fdetermine_close(check_pit, (dir-1)&15, tnk_range, 5));

  pit.front = fdetermine_close(check_pit, dir, tnk_range, 10);

  pit.right = OR(fdetermine_close(check_pit, (dir+1)&15, tnk_range,5),
		 fdetermine_close(check_pit, (dir+4)&15, 50, 5));

  /* This value is for determining the collision danger, so we set it to
     the "danger" value in front of us.
  */

  pit.close = pit.front;
}

/* This function computes the location of the current goal. */

static void fdetermine_goal()
{
  if (trg_x != -1)
    {
      angle_to_fuzzy_direction(trg_x-tnk_x, trg_y-tnk_y, 220, &goal);

      goal.close = object_close(distance(trg_x-tnk_x, trg_y-tnk_y), 100, 1000);
    }
  else
    {
      goal.left=F_FALSE;
      goal.front=F_FALSE;
      goal.right=F_FALSE;

      goal.close=F_FALSE;
    }
}

/* This function checks the speed bonusses/penalties of the underground. This
   way we can avoid water and follow roads.
*/

static int check_terrain(int dir, int x, int y)
{
  /* The variable "speed" is a modification factor of the terrain.
     1 = no modification, < 1 means the tank will go slower, > 1
     means it will go faster on this terrain.
  */

  float speed;
  int m;

  x = (x+16)/b_size, y = (y+16)/b_size;

  if (o->h != 1)
    return 100;

  if (x < 0 || x >= map.sx || y < 0 || y >= map.sy)
    return 100;

  if ((m = mapm.m[0][x+map.sx*y]) != -1)
    speed = pic[m].speed;
  else
    speed = 1;

  if ((m = mapm.m[1][x+map.sx*y]) != -1 && strength[0][0][x+map.sx*y] == 0)
    speed *= pic[m].speed;

  return speed*100;
}

static void fdetermine_fast_or_slow(long *fast, long *slow, long here, long value)
{
  long t;

  t = here<100? 100:here;

  if (value < t)
    {
      if (value > 50)
	*slow = ((t - value)*F_TRUE)/(t - 50);
      else
	*slow = F_TRUE;
    }
  else
    *slow = F_FALSE;

  t = here>100? 100:here;

  if (value > t)
    {
      if (value < 130)
	*fast = ((value - t)*F_TRUE)/(130 - t);
      else
	*fast = F_TRUE;
    }
  else
    *fast = F_FALSE;
}

static void fdetermine_terrain()
{
  long left, front, right, here;

  /* Direction of tank, running from 0 to 15. */

  int dir = tnk.motion.dir;

  /* Check in specific directions. */

  left = determine_average(check_terrain, (dir-2)&15, tnk_range, 5);
  front = determine_average(check_terrain, dir, tnk_range, 5);
  right = determine_average(check_terrain, (dir+2)&15, tnk_range, 5);
  here = check_terrain(0, tnk_x, tnk_y);

  /* Compute if the terrain is fast or slow. */

  fdetermine_fast_or_slow(&fast_terrain.left, &slow_terrain.left, here, left);
  fdetermine_fast_or_slow(&fast_terrain.front, &slow_terrain.front, here, front);
  fdetermine_fast_or_slow(&fast_terrain.right, &slow_terrain.right, here, right);

  /* When is fast terrain close? */

  fast_terrain.close = OR3(fast_terrain.left, fast_terrain.front, fast_terrain.right);

  /* Slow terrain close is only interesting if it is in front of us. */

  slow_terrain.close = slow_terrain.front;
}

/* This function determines in which direction the current waypoint
   is.
*/

static void fdetermine_wayp()
{
  if (tnk.way.type != WAY_INACTIVE)
    {
      angle_to_fuzzy_direction(way_x-tnk_x, way_y-tnk_y, 220, &wayp);

      wayp.close = object_close(distance(way_x-tnk_x, way_y-tnk_y), 100, 1500);
    }
  else
    {
      wayp.left=F_FALSE;
      wayp.front=F_FALSE;
      wayp.right=F_FALSE;

      wayp.close=F_FALSE;
    }
}

/**************************************************** VARIOUS RULES ************************************************************/

/* Non-fuzzy rule: Point your gun at your target. */

static void rule_aim_gun(void)
{
  switch(go_left_or_right(trg_dir, tnk.motion.ldir))
    {
      case -1:
        aim_left(o);
        break;

      case 1:
        aim_right(o);
        break;
     }
}

/* Non-fuzzy rule: Fire at target. */

static void rule_fire(void)
{
  int dir;

  if (trg_x == -1 && trg_y == -1)
    return;

  dir = (tnk.tank->turret.main_static == MAIN_TURRET && tnk.weap.w != 1 && tnk.weap.w != 2)?
        tnk.motion.ldir:tnk.motion.dir;

  if ((trg_dir == dir && in_radius(tnk_x-trg_x, tnk_y-trg_y, tnk.computer.attack_at)) ||
      (tnk.computer.attack_at > 30 && in_radius(tnk_x-trg_x, tnk_y-trg_y, 30)))
    tank_fire(o, trg);
}

/* Non-fuzzy rule: Make attack waves.

   If target is in a certain range, steering will be disabled for some time,
   this way the plane will fly over and make a turn later.
*/

static void rule_attack_wave()
{
  if (in_radius(tnk_x-trg_x, tnk_y-trg_y, 80))
    tnk.computer.wave = tinit(tnk.computer.wave_delay);
}

/* Fuzzy rule: control speed.

   Basic idea is that the tank never stops, but only slows down if there is a danger
   of colliding or falling into a pit.
*/

static void rule_control_speed()
{ 
  /* Release gas when collision danger gets too high, but keep driving. */

  if ((tnk.motion.v > 20 && danger.front > F_HALF) ||

  /* Release gas entirely if a pit is getting too close. */

      (tnk.motion.v >= 0 && pit.front > F_HALF))
    tnk.contr.up = 0;
  else
    tnk.contr.up = 1;
}

/* Non-fuzzy rule: control speed for planes.

   Planes should never break, they will only crash if they do that.
*/

static void rule_control_speed_plane()
{
  tnk.contr.up = 1;
}

/* Fuzzy rule: control steering.

   Based on very local information, decide which way to go.
*/

static void rule_control_steering()
{
  /* Fuzzy relations. */
  
  if (wayp.close >= goal.close)
    {
      want_to_follow_waypoint = wayp.close;
      want_to_follow_goal = DEG_OF_SUP(40, goal.close);
    }
  else
    {
      want_to_follow_goal = goal.close;    
      want_to_follow_waypoint = DEG_OF_SUP(40, wayp.close);
    }

  want_to_follow_fast_terrain = AND3(
				     fast_terrain.close,
				     NOT(want_to_follow_waypoint),
				     NOT(want_to_follow_goal)
				    );

  /* Compute actions. */

  want_left = AND(
		  NOT(danger.left),
		  OR5(
		      danger.front,
		      AND(
			  goal.left,
			  want_to_follow_goal
			 ),
		      AND(
			  wayp.left,
			  want_to_follow_waypoint
			 ),
		      AND3(
			   NOT(fast_terrain.front),
			       fast_terrain.left,
			       want_to_follow_fast_terrain
			  ),
		      AND(
			  slow_terrain.front,
			  NOT(slow_terrain.left)
			 )
		     )
		 );

  want_right = AND(
		   NOT(danger.right),
		   OR5(
		       danger.front,
		       AND(
			   goal.right,
			   want_to_follow_goal
			  ),
		       AND(
			   wayp.right,
			   want_to_follow_waypoint
			  ),
		       AND3(
			    NOT(fast_terrain.front),
			    fast_terrain.right,
			    want_to_follow_fast_terrain
			   ),
			   AND(
			       slow_terrain.front,
			       NOT(slow_terrain.right)
			      )
                      )
		  );

  steering_required = OR3(
			  danger.front,
			  want_left,
			  want_right
			 );

  /* Bonus to last direction. */

  if (!tdone(tnk.computer.last_dir_active))
    switch (tnk.computer.last_dir)
      {
        case -1:
	  MODIFY(&want_left, F_TWOTHIRD);
          break;

        case 1:
	  MODIFY(&want_right, F_TWOTHIRD);
          break;
      }

  /* Random part. */

  MODIFY(&steering_required, rnd(64)-32);
  MODIFY(&want_left, rnd(64)-32);
  MODIFY(&want_right, rnd(64)-32);

  /* Defuzzify. */

  /* Disable steering if attack wave ended. */

  if (tdone(tnk.computer.wave)

  /* Disable steering while plane is lifting off. */

      && (tnk.tank->type != PLANE || o->h > 10)

  /* Otherwise check if "do_steering" is high enough. */

      && steering_required > F_TWOTHIRD)
    {
      if (want_left > want_right)
	{
	  tnk.contr.left = 1;
	  tnk.computer.last_dir = -1;
	  tnk.computer.last_dir_active = tinit(50);
	}
      else
	{
	  tnk.contr.right = 1;
	  tnk.computer.last_dir = 1;
	  tnk.computer.last_dir_active = tinit(50);
	}
    }
}

/**************************************************** THE MAIN ROUTINE *********************************************************/

void control_computer_tank(objectstruct *q)
{ 
  o = q;

  /* First reset all controls, so we won't do strange things when we exit this routine. */

  tnk.contr.up=0;
  tnk.contr.down=0;
  tnk.contr.left=0;
  tnk.contr.right=0;

  /* Setup variables. */

  tnk_x = o->x, tnk_y = o->y, tnk_range = tnk.motion.v < 50? 50:tnk.motion.v;

  way_x = tnk.way.point[tnk.way.current].x, way_y = tnk.way.point[tnk.way.current].y;

  /* Waypoints. */

  determine_next_waypoint();
  
  /* If time has elapsed, choose new target. */

  trg = o->target, trg_x = tnk.computer.target_x, trg_y = tnk.computer.target_y;

  if (tdone(tnk.computer.new_target))
    {
      determine_new_target(o);
      tnk.computer.new_target = tinit(100+rnd(100));
    }
  else
    if (trg != NULL) trg_x = trg->x, trg_y = trg->y;

  /* Store new target. */

  o->target = trg, tnk.computer.target_x = trg_x, tnk.computer.target_y = trg_y;

  trg_dir = radtoa16(angle(trg_x-tnk_x,trg_y-tnk_y));

  /* Check if we are (still) active. */

  if (!tnk.computer.active)
    return;

  /* Non-fuzzy rules. */

  rule_aim_gun();
  rule_fire();

  /* If you're not allowed to steer yet, then don't bother to do the rest of this routine. */

  if (!tdone(tnk.motion.ndir))
    return;

  /* Determine all fuzzy input variables. */

  fdetermine_object();
  fdetermine_building();
  fdetermine_pit();
  fdetermine_goal();
  fdetermine_terrain();
  fdetermine_wayp();

  /* Combine things. */

  danger.left = OR3(object.left, building.left, pit.left);
  danger.front = OR3(object.front, building.front, pit.front);
  danger.right = OR3(object.right, building.right, pit.right);

  /* Extra modification: The variable "tnk.computer.crashed" records when a tank crashed
     into something. Fuzzy logic must have failed, so we create some kind of a panic
     reaction.
  */

  if (!tdone(tnk.computer.crashed))
    MODIFY(&danger.front, F_TWOTHIRD);

  /* The rules. */

  rule_control_steering();

  if (tnk.tank->type != PLANE)
    rule_control_speed();
  else
    {
      rule_control_speed_plane();
      rule_attack_wave();
    }
}

