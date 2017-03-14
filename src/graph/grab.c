/* This function grabs an image from screen and saves it in a BMP */

#include <tank.h>
#include <string.h>

long grab_no = 0, grab = FALSE;

void grab_frame(void)
{
  char name[MAX_PATH];
  PALETTE p;
  BITMAP *b;

  get_palette(p);

  b = create_bitmap(SCREEN_W, SCREEN_H);
  blit(screen, b, 0, active_page * res_y, 0, 0, SCREEN_W, SCREEN_H);

  sprintf(name, "TANK%ld.BMP", grab_no);
  save_bmp(name, b, p);

  destroy_bitmap(b);

  grab = FALSE, grab_no ++;
}
