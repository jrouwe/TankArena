/* Initialises dual player vs computer game. Calls functions in mainloop.c
   and cleans up after end of game.
*/

#include <tank.h>
#include <dir.h>
#include <string.h>
#include <unistd.h>

char dual_v_computer_code[21]="AIR STRIKE";

void dual_v_computer_game(void)
{
  long done = FALSE;
  struct ffblk ffblk;
  FILE *mapfile = NULL;

  game.mode=DUALVC, game.id[0]=1, game.id[1]=2;
  game.exit_game=FALSE, game.end_round=tnever, game.winner=0;
  game.gc_good=0, game.gc_bad=0;

  if (!read_string("MISSION CODE (ESC = LIST): ", dual_v_computer_code, 20))
  {
    message("SCANNING FOR KNOWN PASSWORDS...");

    search_passwd();

    if (!select_password_from_list(dual_v_computer_code))
      return;
  }

  message("VALIDATING MISSION CODE...");

  done=findfirst("MAPS/*.MAP", &ffblk, 0);

  while(!done)
  { mapfile = read_header(ffblk.ff_name, "", "rb", &map);

    if (strcmp(map.this_mission_code, dual_v_computer_code)==0)
     break;

    fclose(mapfile);

    mapfile=NULL;

    done=findnext(&ffblk);
  }

  if (mapfile==NULL)
  {
    message_key("INVALID CODE, TRY \"AIR STRIKE\".");

    return;
  }

  add_password_to_list(dual_v_computer_code);

  view_file(mapfile, map.mission_text[0], map.mission_text[1]);

  *strchr(ffblk.ff_name, '.')=0;

  if (!load_map(ffblk.ff_name, TRUE) || !check_suitable() || !check_startpoints())
  { fclose(mapfile);
    return;
  }

  init_graph();

  compute_window_size();

  set_black();
  write_panel();

  game.running=TRUE;
  game.mayhem_time = tinit(max_mayhem_time);

  do
  { if (!tdone(game.end_round) && !game.exit_game)
     start_fade_in(map.night? &night_pal:&pal);
     else start_fade_out();

    do
    { game_control();
      write_screen();

      done=!do_fade_step();

    } while (!tdone(game.end_round) && !game.exit_game);
  } while (!done);
  
  reset_game();
  
  if (game.winner==1)
  {
    strcpy(dual_v_computer_code, map.next_mission_code);
    add_password_to_list(dual_v_computer_code);
  }

  debrief(mapfile);

  fclose(mapfile);
}

