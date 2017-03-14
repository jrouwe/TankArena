/* We use a single 100 Hz timer to control the speed of the game. */

#include <tank.h>

volatile long game_pause = FALSE, timer = 0;

void game_tick(void)
{
  if (!game_pause) timer++;
}

END_OF_FUNCTION(game_tick);

long init_game_timer(void)
{
  install_timer();

  LOCK_FUNCTION(game_tick);
  LOCK_VARIABLE(timer);
  LOCK_VARIABLE(game_pause);

  install_int_ex(game_tick, BPS_TO_TIMER(sec));

  return(TRUE);
}
