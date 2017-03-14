/* These are some functions which operate on the various graphic buffers */

#include <tank.h>
#include <stdarg.h>
#include <string.h>
#include <sys/segments.h>

long _overwrite_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y);
long _write_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y);

long b_overwrite_text(long x, long y, long color, char *text, ...)
{ char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  return(_overwrite_text(x, y, color, text_string, _my_ds(), scr, buf_x, buf_y));
}

long b_write_text(long x, long y, long color, char *text, ...)
{ char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  return(_write_text(x, y, color, text_string, _my_ds(), scr, buf_x, buf_y));
}

void b_bar(long x, long y, long nx, long ny, long color)
{ if (x+nx>buf_x) nx=buf_x-x;
  if (y+ny>buf_y) ny=buf_y-y;

  if (nx>0 && ny>0)
   asm volatile ("
   0:
	pushl %%ecx
	movl %%edx, %%ecx
	rep
	stosb
	addl %%ebx, %%edi
	popl %%ecx
	loop 0b"
       :
       : "a" (color), "b" (buf_x-nx), "c" (ny), "d" (nx), "D" (scr+y*buf_x+x)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void b_convert(unsigned char *conv, unsigned char *tabl, long size)
{ asm volatile ("
  0:	   
	movb (%%edi), %%al
	xlat
	stosb
	loop 0b"
       :
       : "b" (tabl), "c" (size), "D" (conv)
       : "eax", "ebx", "ecx", "edi");
}

