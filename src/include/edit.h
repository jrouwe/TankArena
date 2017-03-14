#include <tank.h>
#include <math.h>

#define tnk ao->d.tk
					     
#define size_x 200
#define size_y 200
#define max_x  (map.sx-6)
#define max_y  (map.sy-6)

#define ENABLED 	    0
#define DISABLED	    1
#define NOT_IN_SHAREWARE    2

typedef struct
{ short pic[3], links;
} structtype[50][50];

typedef struct smart
{ long s;
  char *name;
  struct smart *menu;
  void (*fun)(void);
  long attr;
} smartmenu;

typedef struct
{ char *name;
  long *value, min, max, step;
  enum {DISPLAY_NORM, DISPLAY_100, DISPLAY_SEC,
	DISPLAY_AFT, DISPLAY_SAFT, DISPLAY_PIX,
	DISPLAY_ACTIVATOR, DISPLAY_YES,
	DISPLAY_2R} display_type;
} updownmenu;

extern smartmenu modweap_mnu[], player_mnu[], show_mnu[], editstr_mnu[], struct_mnu[], stdmap_mnu[], maptype_mnu[],
                 file_mnu[], usertank_mnu[], start_mnu[], comptank_mnu[], addtank_mnu[], addtur_mnu[], addwagon_mnu[],
                 addtrain_mnu[], addmine_mnu[], misc_mnu[], addobj_mnu[], objects_mnu[], ogoal_mnu[], goal_mnu[],
                 bonus_mnu[], bonustype_mnu[], menstruct_mnu[], time_mnu[], misscode_mnu[], seltype_mnu[],
                 special_mnu[], edit_mnu[], contr_mnu[], tkweapon_mnu[], eddway_mnu[], waypoint_mnu[],
                 tank_mnu[], b52_mnu[], shootat_mnu[], tur_mnu[], train_mnu[], warp_mnu[], lock_mnu[],
		 goalwho_mnu[], goalobj_mnu[], lcksrc_mnu[], lcktrg_mnu[], destroy_mnu[],
		 destwhat_mnu[], enforce_mnu[], enfweap_mnu[], flag_mnu[], supermarkt_mnu[],
		 product_mnu[], misspubl_mnu[], missshare_mnu[];

extern updownmenu goalrad_mnu[], weapon_mnu[], activate_mnu[], target_mnu[], fire_mnu[], id_mnu[], turprops_mnu[],
		  lckrad_mnu[], destprop_mnu[], enfprop_mnu[], prodtype_mnu[], prodprice_mnu[];

extern void (*click_left)(long x, long y);
extern void (*click_right)(long x, long y);
extern void (*kb_control_fun)(void);
extern void (*write_left_fun)(void);

extern objectstruct *ao, *prev_wagon;
extern long exit_edit, stx, sty, smx, smy, s_pic, sel_x, sel_y, start_x, start_y, lock_maps, unlock_maps;
extern char edit_map_name[];

extern smartmenu *active_mnu;
extern updownmenu *updown_mnu;
extern structtype *st;

void write_map(void);
void load_fun(void);
void create_fun(void);
void save_fun(void);
void done_fun(void);
void addmine_fun(void);
void addrmine_fun(void);
void usertank_fun(void);
void comptank_fun(void);
void addloco_fun(void);
void addtur_fun(void);
void addb52_fun(void);
void tkpos_fun(void);
void b52pos_fun(void);
void flagpos_fun(void);
void turpos_fun(void);
void shootat_fun(void);
void turprops_fun(void);
void editobj_fun(void);
void remove_fun(void);
void quit_fun(void);
void player_fun(void);
void good_fun(void);
void evil_fun(void);
void tkweapon_fun(void);
void clweapon_fun(void);
void show_fun(void);
void link_fun(void);
void select_fun(void);
void addremove_fun(void);
void execute_smart_menu(void);
void write_smart_menu(void);
void map_setup_menu(void);
void savestr_fun(void);
void start_fun(void);
void startpos_fun(void);
void addbonus_fun(void);
void ranbonus_fun(void);
void fire_fun(void);
void activate_fun(void);
void target_fun(void);
void trpos_fun(void);
void tredtur_fun(void);
void time_fun(void);
void createwarp_fun(void);
void warp_fun(void);
void copyobj_fun(void);
void goal_fun1(void);
void goal_fun10(void);
void ogoal_fun1(void);
void ogoal_fun10(void);
void thiscode_fun(void);
void nextcode_fun(void);
void incdecman_fun(void);
void addflag_fun(void);
void createlock_fun(void);
void eddlock_fun(void);
void eddway_fun(void);
void act_way_fun(void);
void deact_way_fun(void);
void circ_way_fun(void);
void addreg_fun(void);
void remreg_fun(void);
void plweap_fun(void);
void plnoweap_fun(void);
void seltype_fun(void);
void creategoal_fun(void);
void eddgoal_fun(void);
void goalrad_fun(void);
void goalwho_fun(void);
void goalpoint_fun(void);
void struct_fun(void);
void done_fun(void);
void new_position(long x, long y);
void time_fun(void);
void seltype_fun(void);
objectstruct *closest_object(long x, long y);
void view_map(void);
void write_left(void);
void write_left_bare(void);
void write_left_with_block(void);
void write_left_with_men(void);
void write_left_with_struct_goals(void);
void write_left_with_obj_goals(void);
void write_left_with_bonus(void);
void write_struct_left(void);
void kb_map_control(void);
void kb_struct_control(void);
void load_structs(void);
void save_structs(void);
void tank_setup_mnu(void);
void player_setup_mnu(void);
void goal_setup_mnu(void);
void tur_setup_mnu(void);
void lock_setup_mnu(void);
void destroyer_setup_mnu(void);
void enforcer_setup_mnu(void);
void train_setup_mnu(void);
void product_setup_mnu(void);
void show_marker(long x, long y);
void lcksrc_fun(void);
void lcktrg_fun(void);
void destpos_fun(void);
void destprop_fun(void);
void createdest_fun(void);
void destwhat_fun(void);
void enfpos_fun(void);
void enfprop_fun(void);
void createenf_fun(void);
void enfweap_fun(void);
void turforbid_fun(void);
void supermarkt_fun(void);
void createprod_fun(void);
void prodpos_fun(void);
void prodtype_fun(void);
void prodprice_fun(void);
void misspubl_fun(void);
void missshare_fun(void);
