/* Prompts for map/tank to play and initialises game. After that it calls
   the functions in mainloop.c. Cleans up after game is ended.
*/

#include <tank.h>
#include <string.h>
#include <unistd.h>

#define tnk o->d.tk

char dual_map_name[9]="FLAGISLE";

void dual_game(void)
{ char str1[20], str2[20];
  long end_game, done;
  objectstruct *o;
  FILE *dual;
  
  game.mode=DUAL, game.id[0]=-1, game.id[1]=-1, game.wins[0]=0, game.wins[1]=0;
  game.exit_game=FALSE, end_game=FALSE;

  if (!read_string("ENTER MAP (ESC = LIST): ", dual_map_name, 8)
      && !select_from_list(dual_map_name))
    return;

  dual = read_header(dual_map_name, ".MAP", "rb", &map);

  if (dual == 0)
  {
    message_key("FILE NOT FOUND");

    return;
  }

  view_file(dual, map.mission_text[0], map.mission_text[1]);
  fclose(dual);

  for (game.round=0; !game.exit_game && !end_game; game.round++)
  {
    if (!check_suitable() || !select_tanks(dual_map_name) ||
	!load_map(dual_map_name, TRUE) || !check_startpoints())
     return;

    game.end_round=tnever, game.winner=-1;

    init_graph();

    compute_window_size();

    for (o=obj; o!=NULL; o=o->next)
     if (o->type==TANK_TYPE)
     { sprintf(str1, "ROUND %ld", game.round+1);
       sprintf(str2, "%ld : %ld", game.wins[tnk.contr.num], game.wins[tnk.contr.num^1]);
       create_message(o, str1, str2);
     }

    set_black();
    write_panel();
	    
    game.running=TRUE;

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

    if (!game.exit_game)
    { game.wins[game.winner]++;
      end_game=rnds!='~' && (game.wins[0]>rnds/2 || game.wins[1]>rnds/2);
    }
    
    reset_game();

    set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
  }

  if (!shareware && !game.exit_game)
  {
    show_fli("FLI/VICTORY.FLI");
    fade_out(3);
  }

  clear(screen);
}
