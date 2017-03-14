/* Wish we had thought of the pit/lava/ramps in the first place. This
   is some kind of patch to the standard structures to make the tank
   fall in pits, lose hitpoints in lava and jump over ramps.
*/

#include <tank.h>

#define big_low 	8
#define big_high 	24
#define small_low 	14
#define small_high 	18

long in_lava(long x, long y)
{ return (get_type((x+16)/b_size, (y+16)/b_size, 0)==LAVA_PUT);
}

long in_pit(long x, long y)
{ long x2=(x+16)%b_size, y2=(y+16)%b_size, type;
  x=(x+16)/b_size, y=(y+16)/b_size;

  type=get_type(x, y, 0);
  if (type==-1) type=get_type(x, y, 1);

  switch(type)
  { case PIT_C:
     return (TRUE);

    case PIT_T:
     if (y2>big_low)
      return(TRUE);
     break;

    case PIT_B:
     if (y2<big_high)
      return(TRUE);
     break;

    case PIT_L:
     if (x2>big_low)
      return(TRUE);
     break;

    case PIT_R:
     if (x2<big_high)
      return(TRUE);
     break;

    case PIT_TL:
     if (x2>big_low && y2>big_low)
      return(TRUE);
     break;

    case PIT_TR:
     if (x2<big_high && y2>big_low)
      return(TRUE);
     break;

    case PIT_BL:
     if (x2>big_low && y2<big_high)
      return(TRUE);
     break;

    case PIT_BR:
     if (x2<big_high && y2<big_high)
      return(TRUE);
     break;

    case S_PIT_T:
     if (y2>small_low)
      return(TRUE);
     break;

    case S_PIT_B:
     if (y2<small_high)
      return(TRUE);
     break;

    case S_PIT_L:
     if (x2>small_low)
      return(TRUE);
     break;

    case S_PIT_R:
     if (x2<small_high)
      return(TRUE);
     break;

    case S_PIT_TL:
     if (x2>small_low && y2>small_low)
      return(TRUE);
     break;

    case S_PIT_TR:
     if (x2<small_high && y2>small_low)
      return(TRUE);
     break;

    case S_PIT_BL:
     if (x2>small_low && y2<small_high)
      return(TRUE);
     break;

    case S_PIT_BR:
     if (x2<small_high && y2<small_high)
      return(TRUE);
     break;
  }

  return(FALSE);
}

long on_small_ramp(long x, long y, long nx, long ny)
{ long bx, by, nbx, nby;

  bx=(x+16)/b_size, by=(y+16)/b_size;
  nbx=(nx+16)/b_size, nby=(ny+16)/b_size;

  switch (get_type(bx, by, 0))
  { case S_RAMP_L:
     return(nbx==bx-1);

    case S_RAMP_R:
     return(nbx==bx+1);

    case S_RAMP_U:
     return(nby==by-1);

    case S_RAMP_D:
     return(nby==by+1);

    default:
     return(FALSE);
  }
}

long on_big_ramp(long x, long y, long nx, long ny)
{ long bx, by, nbx, nby;

  bx=(x+16)/b_size, by=(y+16)/b_size;
  nbx=(nx+16)/b_size, nby=(ny+16)/b_size;

  switch (get_type(bx, by, 0))
  { case B_RAMP_L:
     return(nbx==bx-1);

    case B_RAMP_R:
     return(nbx==bx+1);

    case B_RAMP_U:
     return(nby==by-1);

    case B_RAMP_D:
     return(nby==by+1);

    default:
     return(FALSE);
  }
}

