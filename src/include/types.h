/****************************** TURRET *************************************/

typedef struct
{ long delay, power, radius, shootat, fixed, shootwith, loc[5];
  uchar *pic[5];
} turretinfo;

/****************************** MAP ****************************************/

#pragma pack(1)

typedef struct
{ char this_mission_code[22], next_mission_code[22];
  long sx, sy, mission_text[4];
  short random_bonus;
  char background;
  char public_pwd;
  short night;
  char map_type;
  uchar map_version;
  short map_contents;
  short sizeofobj;
  char shareware;
  char __extra_char__;
  short lock;
} maptype;

typedef struct
{ short *m[3];
  char *g;
  char *b;
  char *man[2];
} mapmemtype;

typedef short *strengthtype[2][3];

#pragma pack()

/****************************** PICTURES ***********************************/

#pragma pack(1)

typedef struct
{ char name[12];
  long addr;
} indextype;

#pragma pack()

typedef struct
{ char *name;
  long night_vis;
  uchar *ptr;
} internaltype;

typedef struct
{ char *name[3];
  long str[3];
  long exptype;
  float speed;
  long linked;
  long type;
  uchar *ptr[3];
} pictureinfotype;

/****************************** EFFECTS ************************************/

typedef struct
{ char *name;
  unsigned delay;
  long night_vis;
  enum {NEXT, END} next;
  uchar *ptr;
} effectstype;

typedef struct effectctrl
{ struct effectctrl *prev, *next;
  long x, y, h, pn, done;
} effectstruct;

/****************************** TANK ***************************************/

typedef struct
{ enum {CAR, TANK, CHOPPER, PLANE} type;

  struct
  { float mass, acc, ff_0, ff_v, fs_0, fs_v, crash, lift;
    long rotation, change;
  } motion;

  struct
  { long loc[2];
    enum {MAIN_STATIC, MAIN_TURRET, MAIN_ROTATE} main_static;
    long auto_rotation;
  } turret;

  struct
  { long h, r, armor, max_fuel;
  } props;

  struct
  { long crash;
    long smoke;
    char *name;
  } general;

  struct
  { long main, flame, initial_weap;
    long num[num_weapon], speed[num_weapon], power[num_weapon];
  } weap;

  struct
  { long wave_delay;
  } computer;

  long main_gun[9], chain_gun[9], smoke[4], rocket[2], light[13];

  uchar *pic[54], *face[5];

} tankinfotype;

#pragma pack(1)

typedef struct
{ tankinfotype *tank;
  long tank_num;

  struct
  { float vx, vy, vz, fuel, ns;
    long v, dir, ldir, overlap, select, vcount, acount;
    long ndir, nldir, nfuel, boost, smoke, rotate, nhit, nh, dead;
  } motion;

  struct
  { long unused1, unused2, unused3, unused4;
    long attack_at, attack_tanks, attack_goals;
    long fire_delay, wave_delay, wave;
    long activator, active, activation_rad, stay_active;
    long crashed, crashed2;
    long target_x, target_y, new_target, target_way;
    long last_dir, last_dir_active;
  } computer;

  struct
  { long type, current;
    struct
    { long x, y;
    } point[10];
  } way;

  struct
  { long id, num, computer, good, left, right, up, down;
    long fire, shield, disable;
  } contr;

  struct
  { long count, ncount, flash, nflash;
  } invis;

  struct
  { long armor, power, pcount, icount, invun, ruin;
  } shield;

  struct
  { long w, chf, activegun, extragun, nw[num_weapon], reload;
    long nf, nws, nm, power[num_weapon];
  } weap;

  struct
  { long x, y, vx, vy, active, fired;
  } lock;

  struct
  { long nw, old, smokes, in_base, neffect, nsmoke, flags;
  } und;

  struct
  { char line[2][30];
    long count, end;
  } msg;

  struct
  { long lava_hit;
    long light_on;
    long destroy_wreck;
  } last_min;

} tankstruct;

#pragma pack()

/****************************** CONTROL ************************************/

typedef unsigned keytype[2][10];

typedef struct
{ long sensitivity;
  unsigned but[4];
  long x_min, x_mid, x_max, y_min, y_mid, y_max;
} joytype[2];

/****************************** OBJECTS ************************************/

#pragma pack(1)

typedef struct
{ long v, count, dir, armor, bomb, light, nb, nh, nv, nexp, nl;
} b52struct;

typedef struct
{ float dist;
  long v, count, target_x, target_y, angle, power, smoke;
} rocketstruct;

typedef struct
{ long count, target_x, target_y, type, bullets, dir;
  long power, select, ns, fire;
} manstruct;

typedef struct
{ enum {LIGHT_MINE, HEAVY_MINE} type;
  long active, power;
} minestruct;

typedef struct
{ long count, nuke_count, next;
} abombstruct;

typedef struct
{ long type;
} bonusstruct;

typedef struct
{ long type, dir, nd, delay, power, radius, shootat, fixed, activegun, wait;
  ushort forbid_dir;
} turretstruct;

typedef enum {RAILS_HORIZ, RAILS_VERT, RAILS_CIRCLE} railstype;

typedef struct
{ struct
  { long v, count, dir;
  } motion;

  struct
  { long armor, nsmoke, nv;
  } gen;

  struct
  { struct object *front, *rear, *turret;
    long x, y, picture;
  } info;

  struct
  { float start;
    railstype type;
    long dir, srx, sry;
  } rails;

  struct
  { railstype type;
    long dir, srx, sry;
  } end;

} trainstruct;

typedef struct
{ long type, dir, lives, laps, last_chk, num_chk;

  struct
  { short dir, x, y, passed;
  } chk[25];

  struct
  { long active;
    long nw[num_weapon];
  } weap;

} playerstruct;

typedef struct
{ long x, y;
} warpstruct;

typedef struct
{ long expl, nexpl;
} mortarstruct;

typedef struct
{ long type;
  long num;
} flagstruct;
 
typedef struct
{ long type;
  long price;
} productstruct;

typedef struct
{ long x, y;
  long lock_activate;
  long lock_target;

  long activation_range;

} lockstruct;

typedef struct
{ long r, who;
} goalstruct;

typedef struct
{ long nh, nexp, expl;
} bombstruct;

typedef struct
{
  long r;
  long immediate;
  long what;
} destroyerstruct;

typedef struct
{
  long r;
  long enf_good;
  long enf_evil;
  long weapon;
  long fire;
  long change;
  long delay;
  long done;
} enforcerstruct;

typedef struct
{
  float vx;
  long count, img, nimg;
} zeppelinstruct;

typedef struct object
{ long id;

  struct object *this_structure, *target, *owner;
  struct object *prev, *next;

  struct
  { void (*control)(struct object *o);
    void (*write)(struct object *o);
    void (*mwrite)(struct object *o, struct object *ao);
    void (*hit)(struct object *o, long power, long type);
    void (*remove)(struct object *o);
    void (*light)(struct object *o);
  } funct;

  long type, x, y, h, r, gc;
  float fx, fy, fh;

  union
  { tankstruct tk;
    turretstruct tu;
    b52struct b52;
    rocketstruct ro;
    manstruct ma;
    minestruct mi;
    abombstruct ab;
    bonusstruct bo;
    trainstruct tr;
    playerstruct pl;
    warpstruct wa;
    mortarstruct mo;
    flagstruct fl;
    productstruct pr;
    lockstruct lo;
    bombstruct bmb;
    goalstruct go;
    destroyerstruct dest;
    enforcerstruct enf;
    zeppelinstruct zep;
  } d;
} objectstruct;

#pragma pack()

/****************************** BULLETS ************************************/

typedef struct
{ long explosion, kind;
} hitinfotype;

/****************************** GENERAL ************************************/

typedef struct
{ struct object *tk[2];
  enum {SINGLE, DUAL, DUALVC, MAP} mode;
  long flag_hunt, tn, id[2], running, exit_game;
  long winner, gc_good, gc_bad, round, wins[2];
  long end_round, flag_no[2];
  int supermarket_mayhem, mayhem_time;
  int mayhem_cash, mayhem_products;
} gameinfo;

typedef struct
{ char *string;
  void (*funct)(void);
} menustruct;

typedef struct
{ char *string;
  void (*funct)(int n);
} textmenustruct;

/***************************************************************************/
