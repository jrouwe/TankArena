/* This function shows a FLI and aborts on a key press */

#include <tank.h>

static int check_key()
{
  return (scan_key() & 0x80) != 0 ? FLI_OK:FLI_ERROR;
}

int show_fli(char *fli)
{
  clear_key_buffer();

  clear(screen);

  return play_fli(fli, screen, FALSE, check_key) == FLI_OK;
}

