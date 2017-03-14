/*********************** INIT ********************************************/

void init_screen_buffer(void);
void init_graph(void);
void init_various_graph(void);
void init_pictures(void);
uchar *read_file(char *fn);
FILE *open_file(char *fn);
long get_file_size(FILE *f);
void read_settings(void);
void save_settings(void);
void setup_hardware(void);
void load_pictures(void);
void error(char *msg, ...);
void setsound(void);
void set_paths(char *);

/*********************** LOAD/SAVE/START GAME ***************************/

void reset_game(void);
void clear_map(long background, long nx, long ny);
void relocate(objectstruct **ptr);
long check_suitable(void);
long check_startpoints(void);
void search_passwd(void);
void list_codes(void);
long load_map(char *fn, long include_objects);
long save_map(char *fn);
void write_title(void);
FILE *openmapfile(char *fn, char *ext, char *om);
FILE *read_header(char *fn, char *ext, char *om, maptype *m);
long select_tanks(char *fn);
long select_from_list(char *fn);
long select_password_from_list(char *pw);
void add_password_to_list(char *pw);
void debrief(FILE *mapfile);

/*********************** NIGHT MISSION **********************************/

void init_night_mission(void);
void write_bundle(long i, long x, long y, long d);

/*********************** MODE 13 GRAPHICS CONTROL ***********************/

void m13_add_image(long x, long y, long sx, long sy, uchar *image);
long m13_write_text(long x, long y, long col, char *string, ...);
long m13_overwrite_text(long x, long y, long col, char *string, ...);
void m13_bar(long x, long y, long nx, long ny, long color);
void m13_add_bar(long x, long y, long nx, long ny, long color);
void m13_pixel(long x, long y, long color);
void m13_xpixel(long x, long y, long color);
void message(char *msg);
void message_key(char *msg);
long read_string(char *prompt, char *str, long maxlen);
void view_file(FILE *f, long start, long end);
void display_picture(char *fn);

#define big_height	text_height(big_font)
#define big_length(s)	text_length(big_font, s)

/*********************** TEXT MODE CONTROL ******************************/

void get_mode(void);
void restore_mode(void);
void m3_init(void);
void m3_write_background(void);
void m3_write_shade(long x, long y, long nx, long ny);
void m3_write_window(long x, long y, long nx, long ny, long color);
void m3_write_text(long x, long y, char *msg, ...);
void m3_write_bar(long x, long y, long nx, long ny, long xor_color);
void m3_clear_screen(void);

/************************ PALETTE CONTROL *******************************/

void do_gamma_correction(void);

/*********************** PANELS *****************************************/

void write_panel(void);
void m13_write_gray_panel(void);
void m13_write_panel(void);

/*********************** GAME CONTROL ***********************************/

void game_control(void);
void write_screen(void);
void write_radar(objectstruct *o);
void modify_goal_counter(long percentage);

/*********************** DISPLAY CONTROL ********************************/

void read_palette(char *fn, PALETTE *p);
void start_fade_in(PALETTE *p);
void start_fade_out(void);
long do_fade_step(void);
void palette_effects(void);
#define set_black() set_palette(black_palette);

/*********************** BUFFER CONTROL *********************************/

void b_convert(uchar *conv, uchar *table, long size);
long b_write_text(long x, long y, long col, char *string, ...);
long b_overwrite_text(long x, long y, long col, char *string, ...);
void b_bar(long x, long y, long lx, long ly, long color);
void b_first_layer(long x, long y, uchar *layer);
void b_add_layer(long x, long y, uchar *layer);
void b_set_layer(long x, long y, uchar *layer);
void b_add_layer_l(long x, long y, uchar *layer);
void b_add_layer_x(long x, long y, uchar *layer);
void b_add_layer_y(long x, long y, uchar *layer);
void b_add_layer_xy(long x, long y, uchar *layer);
void b_add_layer_d(long x, long y, long dir, uchar **layer);
void b_add_layer_set_high(long x, long y, uchar *layer);
void b_add_shadow(long x, long y, uchar *layer);
void b_copy_picture(long x, long y, uchar *mask, uchar *source, uchar *target);
void draw_line(long x1, long y1, long x2, long y2, long color);
void draw_circle(long x, long y, long r, long color);

#define b_pixel(x,y,c)	    if ((x)>=0 && (x)<buf_x && (y)>=0 && (y)<buf_y) scr[(x)+(y)*buf_x]=(uchar)(c)

/*********************** SECONDARY BUFFER CONTROL ***********************/

void s_add_picture(long x, long y, long sx, long sy, long nx, long ny, uchar *source);
void s_write_picture(long x, long y, long sx, long sy, long nx, long ny, uchar *source);
void s_move_up(long n);
void s_move_down(long n);
void s_move_left(long n);
void s_move_right(long n);
void s_copy_buffer(void);

/*********************** DECLARATIONS FOR BULLETS ***********************/

void clear_bullets(void);
void create_bullet(objectstruct *o, long type, long x, long y, long dir, long power, long maxdist, long safety);
void control_bullets(void);
void write_bullets(void);

/*********************** HIT FUNCTIONS **********************************/

void check_obj_overlap(void);
long check_obj_collide(objectstruct *o, long x, long y);
long check_obj_hit_point(objectstruct *owner, long x, long y, long type, long power);
long check_obj_hit_radius(objectstruct *owner, long x, long y, long r, long type, long power);
long line_obj_hit_point(objectstruct *owner, long x1, long y1, long *x2, long *y2, long dir, long type, long power);
long line_obj_hit_radius(objectstruct *owner, long x1, long y1, long *x2, long *y2, long r, long type, long power);
long check_wall_hit_point(objectstruct *owner, long x, long y, long excl_layer, long type, long power);
long line_wall_hit_point(objectstruct *owner, long x1, long y1, long *x2, long *y2, long type, long power);
long check_wall_hit(objectstruct *owner, long x, long y, long excl_layer, long type, long power);
void hit_wall_big(objectstruct *owner, long x, long y, long r, long exclude_layer, long type, long power);
void hit_obj_for(objectstruct *o, long x, long y, long type, long power);
void hit_wall_for(objectstruct *owner, long x, long y, long layer, long type, long power);

/*********************** DECLARATIONS FOR OBJECTS ***********************/

objectstruct *create_object(long type, long x, long y, long h, long r);
objectstruct *nearest_target(long x, long y);
objectstruct *nearest_object(objectstruct *o, long x, long y);
void control_objects(void);
void write_objects(objectstruct *ao);
void add_object_to_list(objectstruct *o);
void remove_object_from_list(objectstruct *o);
void resort_object(objectstruct *o);
void remove_object(objectstruct *o);
void clear_objects(void);
void clear_abombs(void);

/*********************** EFFECTS ****************************************/

void clear_effects(void);
void create_effect(long x, long y, long h, long pn);
void control_effects(void);

/*********************** DECLARATIONS FOR NEW OBJECTS *******************/

void create_bonus(long x, long y);
void create_all_bonusses(void);
void create_abomb(objectstruct *owner, long x, long y, long count);
void create_mine(long x, long y, long type, long power);
void create_rocket(objectstruct *owner, long x, long y, long target_x, long target_y, long power);
void create_mortar(objectstruct *owner, long x, long y);
objectstruct *make_flag(long x, long y, long type);
objectstruct *make_product(long x, long y, long type);
objectstruct *make_turret(long x, long y, long dir, long h, long type, objectstruct *owner);
objectstruct *make_tank(objectstruct *owner, long tank_num, long x, long y, long dir, long control);
objectstruct *make_train(long x, long y, long type, objectstruct *front);
objectstruct *make_b52(long x, long y, long dir);
objectstruct *make_player(long x, long y, long type, long dir);
objectstruct *make_warp(long x, long y);
objectstruct *make_lock(long x, long y);
objectstruct *make_waypoint(long x, long y);
objectstruct *make_man(objectstruct *owner, long x, long y, long tx, long ty, long type, long power);
objectstruct *make_goal(long x, long y);
objectstruct *make_destroyer(long x, long y);
objectstruct *make_enforcer(long x, long y);
objectstruct *make_zeppelin(void);
void train_change(objectstruct *o);
void prepare_b52(objectstruct *o);
void prepare_tank(objectstruct *o);
void prepare_turret(objectstruct *o);
void prepare_train(objectstruct *o);
void prepare_mine(objectstruct *o);
void prepare_player(objectstruct *o);
void prepare_warp(objectstruct *o);
void prepare_flag(objectstruct *o);
void prepare_lock(objectstruct *o);
void prepare_goal(objectstruct *o);
void prepare_destroyer(objectstruct *o);
void prepare_enforcer(objectstruct *o);
void prepare_product(objectstruct *o);
void tanks_collide(objectstruct *o, objectstruct *p);
void tank_add_impulse(objectstruct *o, float a, long power);
void static_collide(objectstruct *o);
void blowup_mine(objectstruct *o);

/*********************** CONTROLS ***************************************/

long init_keyboard(void);
void deinit_keyboard(void);
unsigned scan_key(void);
unsigned wait_key(void);
void clear_key_buffer(void);
unsigned wait_yesno(void);
void init_joystick(void);
long detect_joystick(long j);
long read_joy_buttons(void);
void read_joystick(long j, long *x, long *y);
void read_controls(void);
void reset_controls(void);
void reset_control(long n);
void clear_controls(objectstruct *o);
void copy_controls_direct(objectstruct *o);
void copy_controls_relative(objectstruct *o);
long reset_mouse(void);
long get_mouse(long *x, long *y);

/*********************** OBJECTS CONTROL ********************************/

void tank_fire(objectstruct *o, objectstruct *target);
void weapon_left(objectstruct *o);
void weapon_right(objectstruct *o);
void aim_left(objectstruct *o);
void aim_right(objectstruct *o);
void tank_write_marker(objectstruct *o);
void tank_marker_control(objectstruct *o);
void pickup_bonus(objectstruct *o, objectstruct *b);
void pickup_product(objectstruct *t, objectstruct *o);
void control_tank(objectstruct *o);
void control_computer_tank(objectstruct *o);
void clear_transp(void);
void transportation(void);

/*********************** WRITE AREA *************************************/

void set_window_size(int x, int y);
void compute_window_size(void);
void clear_layers(void);
void rewrite_block(long px, long py);
void bottom_layer(long tn, long x, long y);
void top_layer(long x, long y);

/*********************** MOTION AND HOMING ******************************/

float add_angle(float *x, float *y, long v, float a, long *count);
float add_vector(float *x, float *y, float vx, float vy, long *count);
long home(objectstruct *o, long *target_x, long *target_y, float *a);
long loc_x(long dir, long *loc);
long loc_y(long dir, long *loc);
float in2pi(float a);
float a16torad(long a);
long radtoa16(float a);
float angle(long dx, long dy);

/*********************** STATUS AND MESSAGES ****************************/

void write_status(objectstruct *o);
void create_message(objectstruct *target, char *line1, char *line2);

/*********************** RANGE CHECKING *********************************/

long overlap(objectstruct *o, uchar *t, long x, long y, long *bx, long *by);
long in_pit(long x, long y);
long in_lava(long x, long y);
long on_big_ramp(long x, long y, long xn, long yn);
long on_small_ramp(long x, long y, long xn, long yn);

#define in_radius(dx, dy, r)	((dx)*(dx)+(dy)*(dy)<(r)*(r))
#define distance(dx, dy)	sqrt((dx)*(dx)+(dy)*(dy))
#define distance2(dx, dy)	((dx)*(dx)+(dy)*(dy))
#define check(x,y)		((x)>=0 && (y)>=0 && (x)<map.sx && (y)<map.sy)

/*********************** TIMER ******************************************/

long init_game_timer(void);

#define tdone(x)    (timer>=(x))
#define tinit(x)    (timer+(x))
#define twait(x)    while (x>=timer)

/*********************** OVERLAPPING OF PICTURES ************************/

long overlap_lb(uchar *p1, long x, long y, uchar *p2);
long overlap_lt(uchar *p1, long x, long y, uchar *p2);
long overlap_rb(uchar *p1, long x, long y, uchar *p2);
long overlap_rt(uchar *p1, long x, long y, uchar *p2);

/*********************** PICTURES ***************************************/

long search_type(long type);
long get_type(long x, long y, long layer);

/*********************** PARTS ******************************************/

void clear_parts(void);
void create_parts(long type, long x, long y);
void control_parts(void);
void write_parts(void);

/*********************** ALL KIND OF FUNCTIONS **************************/

int show_fli(char *path);
void menu(menustruct *m);
long go_left_or_right(long wanted, long actual);
void protect(void *buffer, long size);
void grab_frame(void);
void arena_info(void);
long _bkground(long x, long y);

#define round(x)	    ((x)>=0? ((long)((x)+0.5)):((long)((x)-0.5)))
#define pround(x)	    ((long)((x)+0.5))
#define sqr(x)		    ((x)*(x))
#define sign(x) 	    (x)<0? -1:((x)>0? 1:0)
#define rnd(x)		    (random()%(x))
#define orred(x,y)	    (((x)&(y))!=0)
#define bkground(x,y)	    (back[_bkground(x,y)+4*map.background])
#define max(x,y)	    ((x)>(y)? (x):(y))
#define min(x,y)	    ((x)<(y)? (x):(y))
#define store_max(x, y)     if ((y) > (x)) x = (y)
#define store_min(x, y)     if ((y) < (x)) x = (y)

/*********************** MEMORY *****************************************/

void *allocate(long size);

/*********************** SOUND ******************************************/

void init_sound_card(void);
void init_sounds(void);
void init_player(char *tune);
void create_sound_absolute(long x, long y, long sample);
#define create_sound_relative(o,s)  if (o!=NULL) create_sound_absolute(o->x, o->y, s);

/************************************************************************/
