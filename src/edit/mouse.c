/* Low level functions for mouse control */

#include <tank.h>
#include <dpmi.h>

long reset_mouse(void)
{ __dpmi_regs regs;

  regs.x.ax=0x0000;
  __dpmi_int(0x33, &regs);

  return(regs.x.ax);
}

long get_mouse(long *x, long *y)
{ __dpmi_regs regs;

  regs.x.ax=0x0003;
  __dpmi_int(0x33, &regs);

  *x=regs.x.cx>>1;
  *y=regs.x.dx;

  return(regs.x.bx);
}

