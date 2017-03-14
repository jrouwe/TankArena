/* Function to load and save options */

#include <tank.h>
	 
extern long pl_tanks[2], lose, rnds, max_effects, max_parts, grab_no;
extern long gamcor, victory_count, race_laps, show_radar;
extern char single_code[], dual_v_computer_code[], dual_map_name[];
extern char edit_map_name[];

static struct
{ char *name;
  enum {DEC_LONG, STRING, HEX_LONG} type;
  void *dest;
} cfg[]={
	 {"graph_drv", DEC_LONG, &graph_drv},
	 {"num_pages", DEC_LONG, &num_pages},
	 {"res_x", DEC_LONG, &res_x},
	 {"res_y", DEC_LONG, &res_y},
	 {"gamma", DEC_LONG, &gamcor},
	 {"window_size", DEC_LONG, &win_size},
	 {"show_radar", DEC_LONG, &show_radar},
	 {"sound_volume", DEC_LONG, &sound_volume},
	 {"mucic_volume", DEC_LONG, &music_volume},
	 {"max_effects", DEC_LONG, &max_effects},
	 {"max_parts", DEC_LONG, &max_parts},
	 {"grab_no", DEC_LONG, &grab_no},
	 {"v_count", DEC_LONG, &victory_count},
	 {"loser_tank", DEC_LONG, &lose},
	 {"number_rounds", DEC_LONG, &rnds},
	 {"number_laps", DEC_LONG, &race_laps},
	 {"dual_map_name", STRING, &dual_map_name},
	 {"edit_map_name", STRING, &edit_map_name},
	 {"single_code", STRING, &single_code},
	 {"dual_v_computer_code", STRING, &dual_v_computer_code},
	 {"pl1_contr", DEC_LONG, &key_or_joy[0]},
	 {"pl1_mode", DEC_LONG, &rel_or_dir[0]},
	 {"pl2_contr", DEC_LONG, &key_or_joy[1]},
	 {"pl2_mode", DEC_LONG, &rel_or_dir[1]},
	 {"pl1_tank", DEC_LONG, &pl_tanks[0]},
	 {"pl2_tank", DEC_LONG, &pl_tanks[1]},
	 {"key1_up", HEX_LONG, &keys[0][PLAYER_UP]},
	 {"key1_down", HEX_LONG, &keys[0][PLAYER_DOWN]},
	 {"key1_left", HEX_LONG, &keys[0][PLAYER_LEFT]},
	 {"key1_right", HEX_LONG, &keys[0][PLAYER_RIGHT]},
	 {"key1_fire", HEX_LONG, &keys[0][PLAYER_FIRE]},
	 {"key1_aim_left", HEX_LONG, &keys[0][PLAYER_AIM_LEFT]},
	 {"key1_aim_right", HEX_LONG, &keys[0][PLAYER_AIM_RIGHT]},
	 {"key1_weapon_left", HEX_LONG, &keys[0][PLAYER_WEAPON_LEFT]},
	 {"key1_weapon_right", HEX_LONG, &keys[0][PLAYER_WEAPON_RIGHT]},
	 {"key1_shield", HEX_LONG, &keys[0][PLAYER_SHIELD]},
	 {"key2_up", HEX_LONG, &keys[1][PLAYER_UP]},
	 {"key2_down", HEX_LONG, &keys[1][PLAYER_DOWN]},
	 {"key2_left", HEX_LONG, &keys[1][PLAYER_LEFT]},
	 {"key2_right", HEX_LONG, &keys[1][PLAYER_RIGHT]},
	 {"key2_fire", HEX_LONG, &keys[1][PLAYER_FIRE]},
	 {"key2_aim_left", HEX_LONG, &keys[1][PLAYER_AIM_LEFT]},
	 {"key2_aim_right", HEX_LONG, &keys[1][PLAYER_AIM_RIGHT]},
	 {"key2_weapon_left", HEX_LONG, &keys[1][PLAYER_WEAPON_LEFT]},
	 {"key2_weapon_right", HEX_LONG, &keys[1][PLAYER_WEAPON_RIGHT]},
	 {"key2_shield", HEX_LONG, &keys[1][PLAYER_SHIELD]},
	 {"joy1_sensitivity", DEC_LONG, &joy[0].sensitivity},
	 {"joy1_fire", HEX_LONG, &joy[0].but[0]},
	 {"joy1_aim", HEX_LONG, &joy[0].but[1]},
	 {"joy1_select", HEX_LONG, &joy[0].but[2]},
	 {"joy1_shield", HEX_LONG, &joy[0].but[3]},
	 {"joy2_sensitivity", DEC_LONG, &joy[1].sensitivity},
	 {"joy2_fire", HEX_LONG, &joy[1].but[0]},
	 {"joy2_aim", HEX_LONG, &joy[1].but[1]},
	 {"joy2_select", HEX_LONG, &joy[1].but[2]},
	 {"joy2_shield", HEX_LONG, &joy[1].but[3]},
	 {NULL}};

void read_settings(void)
{
  int i;

  for (i = 0; cfg[i].name != NULL; i ++)
  {
    switch(cfg[i].type)
    {
      case DEC_LONG:
	*(long *)cfg[i].dest = (long)get_config_int("tank", cfg[i].name, *(int *)cfg[i].dest);
        break;

      case HEX_LONG:
	*(long *)cfg[i].dest = (long)get_config_hex("tank", cfg[i].name, *(int *)cfg[i].dest);
        break;

      case STRING:
	strcpy((char *)cfg[i].dest, get_config_string("tank", cfg[i].name, (char *)cfg[i].dest));
	break;
    }
  }
}

void save_settings(void)
{
  int i;

  for (i = 0; cfg[i].name != NULL; i ++)
  {
    switch(cfg[i].type)
    {
      case DEC_LONG:
	set_config_int("tank", cfg[i].name, *(int *)cfg[i].dest);
        break;

      case HEX_LONG:
	set_config_hex("tank", cfg[i].name, *(int *)cfg[i].dest);
        break;

      case STRING:
	set_config_string("tank", cfg[i].name, (char *)cfg[i].dest);
	break;
    }
  }
}
