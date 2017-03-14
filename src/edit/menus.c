/* All the menus */

#include <edit.h>

/*************************** 2P START MENU ***********************************/

smartmenu	 modweap_mnu[]={{-1, "YES", NULL, plweap_fun},
				{-1, "NO", NULL, plnoweap_fun},
				{-1, NULL}},

		 player_mnu[]={{-1, "POSITION/DIRECTION", NULL, startpos_fun},
			       {-1, "MODIFY WEAPONS", modweap_mnu, NULL},
			       {-1, "DONE", NULL, done_fun},
			       {-1, NULL}};

/*************************** STRUCTURE MAP EDIT MENU *************************/

smartmenu	 show_mnu[]={{3, "SHOW LAYER 0", NULL, show_fun},
			     {-1, "SHOW LAYER 1", NULL, show_fun},
			     {-1, "SHOW LAYER 2", NULL, show_fun},
			     {-1, "SHOW ALL", NULL, show_fun},
			     {-1, NULL}},

		 editstr_mnu[]={{-1, "EDIT STRUCTURES", show_mnu, NULL},
				{-1, "LINK BLOCKS", NULL, link_fun},
				{-1, "SAVE MAP", NULL, savestr_fun},
				{-1, "DONE", NULL, done_fun},
				{-1, NULL}};

/*************************** MAIN MENU ***************************************/

smartmenu	 struct_mnu[]={{-1, "SELECT (SPACEBAR)", NULL, select_fun},
			       {-1, "ADD/REMOVE", NULL, addremove_fun},
			       {-1, "SP/ADD REGION", NULL, addreg_fun},
			       {-1, "SP/REMOVE REGION", NULL, remreg_fun},
			       {-1, NULL}},

		 stdmap_mnu[]={{-1, "EMPTY", NULL, create_fun},
			       {-1, "WALLS", NULL, create_fun},
			       {-1, "PIT/WATER", NULL, create_fun, },
			       {-1, NULL}},

		 maptype_mnu[]={{-1, "DESERT", stdmap_mnu, NULL},
				{-1, "GRASS", stdmap_mnu, NULL},
				{-1, "TILE", stdmap_mnu, NULL, NOT_IN_SHAREWARE},
				{-1, "MARKET", stdmap_mnu, NULL, NOT_IN_SHAREWARE},
				{-1, NULL}},

		 file_mnu[]={{-1, "CREATE MAP", maptype_mnu, NULL},
			     {-1, "LOAD MAP (F3)", NULL, load_fun},
			     {-1, "SAVE MAP (F2)", NULL, save_fun},
			     {-1, "LOAD BARE MAP", NULL, load_fun},
			     {-1, "QUIT (ESC)", NULL, quit_fun},
			     {-1, NULL}},

		 usertank_mnu[]={{0, "STANDARD", NULL, usertank_fun},
				 {-1, "DEVASTATOR", NULL, usertank_fun},
				 {-1, "MOUSE", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, "CROCODILE", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, "FAIRCHILD", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, "CHOPPER", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, "RACE TANK", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, "LOSERS TANK", NULL, usertank_fun, NOT_IN_SHAREWARE},
				 {-1, NULL}},

		 start_mnu[]={{-1, "TANK P1", NULL, start_fun},
			      {-1, "TANK P2", NULL, start_fun},
			      {-1, "CHOPPER P1", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "CHOPPER P2", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "PLANE P1", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "PLANE P2", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "RACE P1", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "RACE P2", NULL, start_fun, NOT_IN_SHAREWARE},
			      {-1, "FLAG P1", NULL, addflag_fun},
			      {-1, "FLAG P2", NULL, addflag_fun},
			      {-1, NULL}},

		 comptank_mnu[]={{0, "LIZZARD", NULL, comptank_fun},
				 {-1, "COMPUTER DRONE", NULL, comptank_fun},
				 {-1, "LAUNCHER", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "SIDE-ARM", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "GREEN PLANE", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "JET", NULL, comptank_fun},
				 {-1, "CAR", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "BOB'S BUS", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "BATTLE CHOPPER", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "BUG LAUNCHER", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "TRUCK", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "FLAMER", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "BULLDOZER", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, "ATOMIC TANK", NULL, comptank_fun, NOT_IN_SHAREWARE},
				 {-1, NULL}},

		 supermarkt_mnu[]={{-1, "SHOPPING CAR", NULL, supermarkt_fun, NOT_IN_SHAREWARE},
				   {-1, NULL}},

		 addtank_mnu[]={{-1, "1P MODE", usertank_mnu, NULL},
				{-1, "2P MODE", start_mnu, NULL},
				{-1, "COMPUTER", comptank_mnu, NULL},
				{-1, "MARKET", supermarkt_mnu, NULL, NOT_IN_SHAREWARE},
				{-1, NULL}},

		 addtur_mnu[]={{0, "BIG ROUND GRAY", NULL, addtur_fun},
			       {-1, "ROCKET LAUNCHER", NULL, addtur_fun},
			       {-1, "BIG ROUND YELLOW", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "CHAIN TURRET", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "SMALL ROUND GRAY", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "SHIELDED GRAY", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "SQUARE YELLOW", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "SMALL YELLOW", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, "SHIP TURRET", NULL, addtur_fun, NOT_IN_SHAREWARE},
			       {-1, NULL}},

		 addwagon_mnu[]={{0, "BLUE ROOF", NULL, addloco_fun},
				 {-1, "EMPTY", NULL, addloco_fun},
				 {-1, "WITH BOXES", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, "WITH FUEL", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, "WITH MUD", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, "BROWN ROOF", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, "WITH MISSILES", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, "WITH TURRET", NULL, addloco_fun, NOT_IN_SHAREWARE},
				 {-1, NULL}},

		 addtrain_mnu[]={{0, "NEW TRAIN", NULL, addloco_fun},
				 {-1, "ADD WAGON", addwagon_mnu, NULL},
				 {-1, NULL}},

		 addmine_mnu[]={{0, "LIGHT", NULL, addmine_fun},
				{-1, "HEAVY", NULL, addrmine_fun, NOT_IN_SHAREWARE},
				{-1, NULL}},

		 misc_mnu[]={{-1, "WARP", NULL, createwarp_fun},
			     {-1, "LOCK", NULL, createlock_fun},
			     {-1, "GOAL", NULL, creategoal_fun},
			     {-1, "DESTROYER", NULL, createdest_fun, NOT_IN_SHAREWARE},
			     {-1, "ENFORCER", NULL, createenf_fun, NOT_IN_SHAREWARE},
			     {-1, "PRODUCT", NULL, createprod_fun, NOT_IN_SHAREWARE},
			     {NULL}},

		 addobj_mnu[]={{-1, "TANK", addtank_mnu, NULL},
			       {-1, "TRAIN", addtrain_mnu, NULL},
			       {-1, "TURRET", addtur_mnu, NULL},
			       {-1, "B52", NULL, addb52_fun, NOT_IN_SHAREWARE},
			       {-1, "MINE", addmine_mnu, NULL},
			       {-1, "MISC.", misc_mnu, NULL},
			       {-1, NULL}},

		 objects_mnu[]={{-1, "ADD", addobj_mnu, NULL},
				{-1, "EDIT", NULL, editobj_fun},
				{-1, "CLONE", NULL, copyobj_fun},
				{-1, "REMOVE", NULL, remove_fun},
				{-1, NULL}},

		 ogoal_mnu[]={{0, "INC/DEC BY 10 %%", NULL, ogoal_fun10},
			      {-1, "INC/DEC BY 1 %%", NULL, ogoal_fun1},
			      {-1, NULL}},

		 goal_mnu[]={{0, "INC/DEC BY 10 %%", NULL, goal_fun10},
			     {-1, "INC/DEC 1 %%", NULL, goal_fun1},
			     {-1, NULL}},

		 bonus_mnu[]={{0, "GRENADES", NULL, addbonus_fun},
			      {-1, "EXTRA MGUN", NULL, addbonus_fun},
			      {-1, "FUEL", NULL, addbonus_fun},
			      {-1, "MINES", NULL, addbonus_fun},
			      {-1, "ROCKETS", NULL, addbonus_fun},
			      {-1, "MORTAR SHELL", NULL, addbonus_fun},
			      {-1, "ABOMB", NULL, addbonus_fun},
			      {-1, "MEN WITH CG", NULL, addbonus_fun},
			      {-1, "MEN WITH FL", NULL, addbonus_fun},
			      {-1, "SMOKE SCREEN", NULL, addbonus_fun},
			      {-1, "INVISIBILITY", NULL, addbonus_fun},
			      {-1, "SPEEDUP", NULL, addbonus_fun},
			      {-1, "INVUNERABILITY", NULL, addbonus_fun, NOT_IN_SHAREWARE},
			      {-1, "ARMOR", NULL, addbonus_fun, NOT_IN_SHAREWARE},
			      {-1, NULL}},

		 bonustype_mnu[]={{-1, "RANDOM BONUS", NULL, ranbonus_fun},
				  {-1, "NON RANDOM BONUS", bonus_mnu, ranbonus_fun},
				  {-1, NULL}},

		 menstruct_mnu[]={{0, "CHAIN GUN", NULL, incdecman_fun},
				  {-1, "FLAME THROWER", NULL, incdecman_fun, NOT_IN_SHAREWARE},
				  {-1, "SMALL ROCKETS", NULL, incdecman_fun, NOT_IN_SHAREWARE},
				  {-1, NULL}},

		 time_mnu[]={{-1, "DAY", NULL, time_fun},
			     {-1, "NIGHT", NULL, time_fun, NOT_IN_SHAREWARE},
			     {-1, NULL}},

		 misscode_mnu[]={{-1, "THIS MISSION", NULL, thiscode_fun},
				 {-1, "NEXT MISSION", NULL, nextcode_fun},
				 {-1, NULL}},

		 seltype_mnu[]={{-1, "DON'T CARE", NULL, seltype_fun},
				{-1, "DUAL PLAYER", NULL, seltype_fun},
				{-1, "SINGLE PLAYER", NULL, seltype_fun},
				{-1, "DUAL VS COMPUTER", NULL, seltype_fun},
				{-1, "S/D VS COMPUTER", NULL, seltype_fun},
				{-1, NULL}},

		 misspubl_mnu[]={{-1, "YES", NULL, misspubl_fun},
				 {-1, "NO", NULL, misspubl_fun},
				 {-1, NULL}},

		 missshare_mnu[]={{-1, "YES", NULL, missshare_fun},
				  {-1, "NO", NULL, missshare_fun},
				  {-1, NULL}},

		 special_mnu[]={{-1, "MAP TYPE", seltype_mnu, NULL},
				{-1, "MISSION CODES", misscode_mnu, NULL},
				{-1, "CODE PUBLIC", misspubl_mnu, NULL},
				{-1, "IN SHAREWARE", missshare_mnu, NULL, NOT_IN_SHAREWARE},
				{-1, "MISSION TIME", time_mnu, NULL},
				{-1, "MEN IN STRUCTURES", menstruct_mnu, NULL},
				{-1, "STRUCTURE GOALS", goal_mnu, NULL},
				{-1, "OBJECT GOALS", ogoal_mnu, NULL},
				{-1, "BONUS", bonustype_mnu, NULL},
				{-1, NULL}},

		 edit_mnu[]={{-1, "STRUCTURES", struct_mnu, struct_fun},
			     {-1, "OBJECTS", objects_mnu, NULL},
			     {-1, "SPECIAL", special_mnu, NULL},
			     {-1, "FILE", file_mnu, NULL},
			     {-1, NULL}};

/*************************** TANK MENU ***************************************/

smartmenu	 contr_mnu[]={{-1, "PLAYER", NULL, player_fun},
			      {-1, "GOOD", NULL, good_fun},
			      {-1, "EVIL", NULL, evil_fun},
			      {-1, NULL}},

		 tkweapon_mnu[]={{0, "EDIT", NULL, tkweapon_fun},
				 {-1, "CLEAR", NULL, clweapon_fun},
				 {-1, NULL}},

		 eddway_mnu[]={{0, "SET/RESET 0", NULL, eddway_fun},
			       {-1, "SET/RESET 1", NULL, eddway_fun},
			       {-1, "SET/RESET 2", NULL, eddway_fun},
			       {-1, "SET/RESET 3", NULL, eddway_fun},
			       {-1, "SET/RESET 4", NULL, eddway_fun},
			       {-1, "SET/RESET 5", NULL, eddway_fun},
			       {-1, "SET/RESET 6", NULL, eddway_fun},
			       {-1, "SET/RESET 7", NULL, eddway_fun},
			       {-1, "SET/RESET 8", NULL, eddway_fun},
			       {-1, "SET/RESET 9", NULL, eddway_fun},
			       {0, NULL}},

		 waypoint_mnu[]={{0, "INACTIVE", NULL, deact_way_fun},
                                 {-1, "ACTIVE", eddway_mnu, act_way_fun},
                                 {-1, "CIRCULAR", eddway_mnu, circ_way_fun},
				 {0, NULL}},

		 tank_mnu[]={{-1, "POSITION/DIRECTION", NULL, tkpos_fun},
			     {-1, "CONTROLLER", contr_mnu, NULL},
			     {-1, "WEAPONS", tkweapon_mnu, NULL},
			     {-1, "ACTIVATION", NULL, activate_fun},
			     {-1, "TARGETS", NULL, target_fun},
			     {-1, "FIRE", NULL, fire_fun},
			     {-1, "WAYPOINTS", waypoint_mnu, NULL},
			     {-1, "DONE", NULL, done_fun},
			     {-1, NULL}};

updownmenu	 weapon_mnu[]={{"MAIN", NULL, 0, 100, 1, DISPLAY_NORM},
			       {"CHAIN GUN", NULL, 0, 5000, 100, DISPLAY_NORM},
			       {"MINES", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"ROCKETS", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"MORTARS", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"A-BOMBS", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"MEN WITH CG", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"MEN WITH FL", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"SMOKE", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"CLOAK", NULL, 0, 50, 1, DISPLAY_NORM},
			       {"SPEEDUP", NULL, 0, 50, 1, DISPLAY_NORM},
			       {NULL}},

		 activate_mnu[]={{"START ACTIVE ", NULL, 0, 1, 0, DISPLAY_YES},
				 {"STAY ACTIVE  ", NULL, 0, 1, 0, DISPLAY_YES},
				 {"ACTIVATOR   ", NULL, 0, 1, 0, DISPLAY_ACTIVATOR},
				 {"RADIUS    ", NULL, 0, 2500, 25, DISPLAY_PIX},
				 {NULL}},

		 target_mnu[]={{"TANKS      ", NULL, 0, 100, 10, DISPLAY_100},
			       {"GOALS      ", NULL, 0, 100, 10, DISPLAY_100},
			       {NULL}},

		 fire_mnu[]={{"POWER         ", NULL, 0, 50, 1, DISPLAY_2R},
			     {"DELAY      ", NULL, 0, 500, 10, DISPLAY_SAFT},
			     {"RADIUS    ", NULL, 0, 300, 10, DISPLAY_PIX},
			     {"WAVE DELAY ", NULL, 0, 990, 10, DISPLAY_SAFT},
			     {NULL}},

		 id_mnu[]={{"ID =", NULL, 1, 10, 1, DISPLAY_AFT},
			   {NULL}};

/*************************** B52 MENU ****************************************/

smartmenu	 b52_mnu[]={{-1, "POSITION/DIRECTION", NULL, b52pos_fun},
			    {-1, "DONE", NULL, done_fun},
			    {-1, NULL}};

/*************************** TURRET MENU *************************************/

smartmenu	 shootat_mnu[]={{-1, "ALL TANKS", NULL, shootat_fun},
				{-1, "GOOD TANKS", NULL, shootat_fun},
				{-1, "EVIL TANKS", NULL, shootat_fun},
				{-1, "ALL EXCEPT P1", NULL, shootat_fun},
				{-1, "ALL EXCEPT P2", NULL, shootat_fun},
				{-1, NULL}},

		 tur_mnu[]={{-1, "POSITION/DIRECTION", NULL, turpos_fun},
			    {-1, "FORBIDDEN ZONES", NULL, turforbid_fun},
			    {-1, "SHOOT AT", shootat_mnu, NULL},
			    {-1, "PROPS", NULL, turprops_fun},
			    {-1, "DONE", NULL, done_fun},
			    {-1, NULL}};

updownmenu	 turprops_mnu[]={{"FIRE POWER     ", NULL, 0, 50, 1, DISPLAY_2R},
				 {"FIRE RADIUS", NULL, 50, 500, 10, DISPLAY_PIX},
				 {"FIRE DELAY  ", NULL, 10, 990, 10, DISPLAY_SAFT},
				 {"FIXED DIR.    ", NULL, 0, 1, 0, DISPLAY_YES},
				 {NULL}};

/*************************** TRAIN MENU **************************************/

smartmenu	 train_mnu[]={{-1, "POSITION/DIRECTION", NULL, trpos_fun},
			      {-1, "EDIT TURRET", NULL, tredtur_fun},
			      {-1, "DONE", NULL, done_fun},
			      {-1, NULL}};

/*************************** WARP MENU ***************************************/

smartmenu	 warp_mnu[]={{-1, "EXIT/ENTRY", NULL, warp_fun},
			     {-1, "DONE", NULL, done_fun},
			     {-1, NULL}};

/*************************** LOCK MENU ***************************************/

smartmenu	 lcksrc_mnu[]={{-1, "BLOW STRUCTURE", NULL, lcksrc_fun},
			       {-1, "BLOW OBJECT", NULL, lcksrc_fun},
			       {-1, NULL}},

		 lcktrg_mnu[]={{-1, "BLOW STRUCTURE", NULL, lcktrg_fun},
			       {-1, "KILL OBJECT", NULL, lcktrg_fun},
			       {-1, "REMOVE OBJECT", NULL, lcktrg_fun},
			       {-1, NULL}},

		 lock_mnu[]={{-1, "LOCK/KEY", NULL, eddlock_fun},
			     {-1, "ACTIVATE ON", lcksrc_mnu, NULL},
			     {-1, "LOCK TYPE", lcktrg_mnu, NULL},
			     {-1, "DONE", NULL, done_fun},
			     {-1, NULL}};

/*************************** GOAL MENU ***************************************/

smartmenu	 goalwho_mnu[]={{-1, "GOOD", NULL, goalwho_fun},
				{-1, "EVIL", NULL, goalwho_fun},
				{-1, "TRAIN", NULL, goalwho_fun},
				{-1, "OBJECT", NULL, goalpoint_fun},
				{-1, NULL}},

		 goalobj_mnu[]={{-1, "POINT TO REACH", NULL, eddgoal_fun},
				{-1, "GOAL FOR WHO", goalwho_mnu, NULL},
				{-1, "RADIUS", NULL, goalrad_fun},
				{-1, "DONE", NULL, done_fun},
				{-1, NULL}};

updownmenu	 goalrad_mnu[]={{"PIXELS", NULL, 10, 200, 5, DISPLAY_NORM},
				{NULL}};

/*************************** DESTROYER MENU *********************************/

smartmenu	 destroy_mnu[]={{-1, "POSITION", NULL, destpos_fun},
				{-1, "DESTROY WHAT", destwhat_mnu, NULL},
				{-1, "PROPERTIES", NULL, destprop_fun},
				{-1, "DONE", NULL, done_fun},
				{-1, NULL}},

		 destwhat_mnu[]={{-1, "STRUCTURES", NULL, destwhat_fun},
                                 {-1, "OBJECTS", NULL, destwhat_fun},
                                 {-1, "BOTH", NULL, destwhat_fun},
				 {-1, NULL}};

updownmenu	 destprop_mnu[]={{"IMMEDIATELY  ", NULL, 0, 1, 0, DISPLAY_YES},
				 {"RANGE     ", NULL, 10, 200, 5, DISPLAY_PIX},
				 {NULL}};

/*************************** ENFORCER MENU *********************************/

smartmenu	 enforce_mnu[]={{-1, "POSITION", NULL, enfpos_fun},
				{-1, "WEAPON", enfweap_mnu, NULL},
				{-1, "PROPERTIES", NULL, enfprop_fun},
				{-1, "DONE", NULL, done_fun},
				{-1, NULL}},

		 enfweap_mnu[]={{-1, "MAIN", NULL, enfweap_fun},
				{-1, "CHAIN GUN", NULL, enfweap_fun},
				{-1, "FLAME THROWER", NULL, enfweap_fun},
				{-1, "MINES", NULL, enfweap_fun},
				{-1, "ROCKETS", NULL, enfweap_fun},
				{-1, "MORTARS", NULL, enfweap_fun},
				{-1, "A-BOMBS", NULL, enfweap_fun},
				{-1, "MEN WITH CG", NULL, enfweap_fun},
				{-1, "MEN WITH FL", NULL, enfweap_fun},
				{-1, "SMOKE", NULL, enfweap_fun},
				{-1, "CLOAK", NULL, enfweap_fun},
				{-1, "SPEEDUP", NULL, enfweap_fun},
				{-1, NULL}};

updownmenu	 enfprop_mnu[]={{"ENFORCE GOOD ", NULL, 0, 1, 0, DISPLAY_YES},
				{"ENFORCE EVIL ", NULL, 0, 1, 0, DISPLAY_YES},
				{"CHANGE PERM. ", NULL, 0, 1, 0, DISPLAY_YES},
				{"FIRE WEAPON  ", NULL, 0, 1, 0, DISPLAY_YES},
				{"RANGE     ", NULL, 10, 200, 5, DISPLAY_PIX},
				{"DELAY      ", NULL, 10, 1000, 10, DISPLAY_SAFT},
				{NULL}};

/*************************** FLAG MENU ***************************************/

smartmenu        flag_mnu[]={{-1, "POSITION", NULL, flagpos_fun},
                             {-1, "DONE", NULL, done_fun},
                             {-1, NULL}};

/*************************** PRODUCTS MENU ***********************************/

updownmenu	 prodtype_mnu[]={{"", NULL, 0, num_products-1, 1, DISPLAY_AFT},
				 {NULL}};

updownmenu	 prodprice_mnu[]={{"$", NULL, 1, 20, 1, DISPLAY_AFT},
				  {NULL}};

smartmenu	 product_mnu[]={{-1, "POSITION", NULL, prodpos_fun},
				{-1, "TYPE", NULL, prodtype_fun},
				{-1, "PRICE", NULL, prodprice_fun},
				{-1, "DONE", NULL, done_fun},
				{-1, NULL}};
