#define MAP_VERSION	6

#define uchar           unsigned char
#define ushort		unsigned short
#define ulong		unsigned long

#define MAX_LONG	0x7fffffff

#define tnow            0
#define tnever		MAX_LONG

#define FALSE 		0
#define TRUE  		-1

#define MAX_PATH	128

#define sec		100

#define b_size 		33

#define num_tanks	23
#define num_wagons	8
#define num_turrets	9
#define num_weapon 	12
#define max_back	4
#define max_sounds	20
#define max_map_x	200
#define max_map_y	200
#define max_files	1000
#define num_products	15

#define max_mayhem_time 	(180*sec)
#define max_mayhem_products	20
#define max_mayhem_cash 	40

#define PLAYER_UP 	        0
#define	PLAYER_DOWN 	        1
#define PLAYER_LEFT 	        2
#define PLAYER_RIGHT    	3
#define PLAYER_FIRE 	        4
#define PLAYER_AIM_LEFT 	5
#define PLAYER_AIM_RIGHT 	6
#define	PLAYER_WEAPON_LEFT 	7
#define	PLAYER_WEAPON_RIGHT 	8
#define PLAYER_SHIELD 	        9

#define DUST 		0
#define SMOKE 		3
#define RUIN 		5
#define FIRE 		9
#define EXPLOSION 	12
#define CHAIN 		17
#define FLAME 		20
#define EXHAUST 	25
#define BLOOD 		27
#define PLUF 		30
#define SPLASH		32
#define SFIRE		35
#define OIL_SPLASH	36
#define ROOK		39
#define TELEPORT_SPARK	44
#define SMOKE_SCREEN	50
#define LEXPLOSION	77
#define MORTAR_FIRE	82
#define LAVA_EFFECT	85
#define PARTS		89
#define PARTS_SMOKE	119

#define ARROW 		0
#define MINE 		8
#define MARKER 		10
#define ABOMB 		14
#define ARMOR		18
#define TANKICO		19
#define STATIC		20
#define BLOCK		24
#define YOU_WIN 	27
#define MISSION 	31
#define SELECT		43
#define HOLE		44
#define B52_LIGHT	45
#define MORTAR		46
#define VLAG		51
#define VLAGICON	53
#define METER		55
#define OVERLAP_BLOCK	59
#define PRODUCTS	60

#define HT_MAIN 	0
#define HT_CHAIN 	1
#define HT_FLAME 	2
#define HT_ROCKET 	3
#define HT_ABOMB 	4
#define HT_CRASH	5
#define HT_MINE 	6
#define HT_BOMB 	7
#define HT_DESTROY	8
#define HT_TANK_CRASH	9
#define HT_SROCK	10
#define HT_PARTS_TNK	11
#define HT_PARTS_WALL	12
#define HT_TRAIN	13
#define HT_MORTAR	14
#define HT_REMOVE	15
#define HT_PLANEBOMB	16
#define HT_LAVA 	17

#define PT_PIXEL	0
#define PT_FLAME	1
#define PT_MORTAR	2
#define PT_PARTS	3

#define NONE		0x00
#define UP   		0x01
#define DOWN            0x02
#define LEFT            0x04
#define RIGHT           0x08
#define STOP		0x10

#define NOTHING 	-1
#define HELISITE 	1
#define RUNWAY          2
#define WATER 		5
#define WATER_SIDE	6
#define SHELTER		15
#define ROAD		18
#define BUSHES		19
#define BUSH_SIDE	20
#define NORM_KRAT	21
#define ROAD_KRAT	22
#define PIT_TL		23
#define PIT_BL		24
#define PIT_T		25
#define PIT_B		26
#define PIT_L		27
#define PIT_R		28
#define PIT_TR		29
#define PIT_BR		30
#define PIT_C		31
#define FUEL_DUMP	32
#define T_LEFT		33
#define T_RIGHT         34
#define T_UP            35
#define T_DOWN          36
#define WARP_IN		37
#define WARP_OUT	38
#define RAILS_H         39
#define RAILS_V         40
#define RAILS_CR        41
#define RAILS_TL_A      42
#define RAILS_TL_B      43
#define RAILS_TL_C      44
#define RAILS_TL_D      45
#define RAILS_TR_A      46
#define RAILS_TR_B      47
#define RAILS_TR_C      48
#define RAILS_TR_D      49
#define RAILS_BL_A      50
#define RAILS_BL_B      51
#define RAILS_BL_C      52
#define RAILS_BL_D      53
#define RAILS_BR_A      54
#define RAILS_BR_B      55
#define RAILS_BR_C      56
#define RAILS_BR_D      57
#define RAILS_S_A	58
#define RAILS_S_B	59
#define RAILS_S_C	60
#define RAILS_S_D	61
#define LIGHT_SRC	62
#define STATION		63
#define OIL		64
#define PLOEF		65
#define BRIDGE_H	68
#define BRIDGE_V	69
#define ROTATE_L	70
#define ROTATE_R	71
#define FIELD_H  	72
#define FIELD_V		73
#define FIELD_P		74
#define FIELD_N		75
#define START		76
#define CHECK_P1_UP  	77
#define CHECK_P1_RIGHT 	78
#define CHECK_P1_DOWN  	79
#define CHECK_P1_LEFT  	80
#define CHECK_P2_UP  	81
#define CHECK_P2_RIGHT 	82
#define CHECK_P2_DOWN  	83
#define CHECK_P2_LEFT  	84
#define S_PIT_TL	85
#define S_PIT_BL	86
#define S_PIT_T		87
#define S_PIT_B		88
#define S_PIT_L		89
#define S_PIT_R		90
#define S_PIT_TR	91
#define S_PIT_BR	92
#define RAILS_W_A	93
#define RAILS_W_B	94
#define RAILS_W_C	95
#define RAILS_W_D	96
#define BRIDGE_CR	97
#define BRIDGE		98
#define BARRIER_L       99
#define BARRIER_R       100
#define BARRIER_T       101
#define BARRIER_B       102
#define BARRIER_TL      103
#define BARRIER_TR      104
#define BARRIER_BL      105
#define BARRIER_BR      106
#define S_RAMP_L	107
#define S_RAMP_R	108
#define S_RAMP_U	109
#define S_RAMP_D	110
#define B_RAMP_L	111
#define B_RAMP_R	112
#define B_RAMP_U	113
#define B_RAMP_D	114
#define LAVA_PUT	115
#define ABOX		116

#define OBJ_MAGIC	0x4a424f54L

#define TANK_TYPE	0
#define B52_TYPE	1
#define MAN_TYPE	2
#define TURRET_TYPE	3
#define ROCKET_TYPE	4
#define MINE_TYPE	5
#define ABOMB_TYPE	6
#define BONUS_TYPE	7
#define TRAIN_TYPE	8
#define WAGON_TYPE	9
#define PLAYER_TYPE	10
#define WARP_TYPE	11
#define MORTAR_TYPE	12
#define FLAG_TYPE       13
#define LOCK_TYPE       14
#define GOAL_TYPE	15
#define DESTROYER_TYPE	16
#define ENFORCER_TYPE	17
#define ZEPPELIN_TYPE	18
#define PRODUCT_TYPE	19

#define LI_LARGE	0
#define LI_MEDIUM	1
#define LI_SMALL	2

#define CONTR_KEY       0
#define CONTR_JOY       1

#define WAY_INACTIVE    0
#define WAY_ACTIVE      1
#define WAY_CIRCULAR    2

#define MAPT_DONTCARE		0
#define MAPT_DUAL		1
#define MAPT_SINGLE		2
#define MAPT_DUALVC		3
#define MAPT_SINGLEORDUAL	4

#define MAPC_TANK		1
#define MAPC_CHOP		2
#define MAPC_PLANE		4
#define MAPC_RACE		8
#define MAPC_FLAG		16

#define FNT_NORM		7
#define FNT_HIGH		15

