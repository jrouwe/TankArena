/* These are the functions which have not been translated into Allegro calls
   yet. They work in the standard VGA mode 13h.
*/

#include <tank.h>
#include <dpmi.h>
#include <stdarg.h>
#include <go32.h>
#include <sys/farptr.h>

long _overwrite_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y);
long _write_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y);

void m13_pixel(long x, long y, long color)
{ if (x>=0 && x<320 && y>=0 && y<200)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	movb %%al, %%es:(%%edi)

	popw %%es"
       :
       : "g" (_dos_ds), "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_xpixel(long x, long y, long color)
{ if (x>=0 && x<320 && y>=0 && y<200)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	xorb %%al, %%es:(%%edi)

	popw %%es"
       :
       : "g" (_dos_ds), "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_bar(long x, long y, long nx, long ny, long color)
{ if (nx>0 && ny>0)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	movl %1, %%ecx
  0:
	pushl %%ecx
	pushl %%edi

	movl %2, %%ecx
	rep
	stosb

	popl %%edi
	popl %%ecx

	addl $320, %%edi
	loop 0b
		 
	popw %%es"
       :
       : "g" (_dos_ds), "g" (ny), "g" (nx),
	 "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_add_bar(long x, long y, long nx, long ny, long color)
{ if (nx>0 && ny>0)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

	movl %1, %%ecx
  0:
	pushl %%ecx
	pushl %%edi

	movl %2, %%ecx
  1:
	addb %%al, %%es:(%%edi)
	incl %%edi
	loop 1b

	popl %%edi
	popl %%ecx

	addl $320, %%edi
	loop 0b
		 
	popw %%es"
       :
       : "g" (_dos_ds), "g" (ny), "g" (nx),
	 "D" (0xa0000+x+320*y), "a" (color)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m13_add_image(long x, long y, long nx, long ny, unsigned char *img)
{ if (img!=NULL)
   asm volatile ("
	pushw %%es

	pushw %0
	popw %%es

  0:
	pushl %%ecx
	movl %%edx, %%ecx

   1:
	lodsb
	cmpb $0xff, %%al
	je 2f
	movb %%al, %%es:(%%edi)
  2:
	incl %%edi
	loop 1b

	popl %%ecx
	addl %%ebx, %%edi
	loop 0b

	popw %%es"
       :
       : "g" (_dos_ds), "c" (ny), "d" (nx),
	 "b" (320-nx), "S" (img), "D" (0xa0000+x+y*320)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
}

long m13_overwrite_text(long x, long y, long color, char *text, ...)
{ char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  return(_overwrite_text(x, y, color, text_string, _dos_ds, (unsigned char *)0xa0000, 320, 200));
}

long m13_write_text(long x, long y, long color, char *text, ...)
{ char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  return(_write_text(x, y, color, text_string, _dos_ds, (unsigned char *)0xa0000, 320, 200));
}

