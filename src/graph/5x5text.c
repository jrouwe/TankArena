/* Here are two functions to write text into a buffer or on screen, it uses
   the fixed 5x5 font in 5x5font.s.
*/

#include <tank.h>
#include <go32.h>

extern unsigned char start_5x5_font[];

long _overwrite_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y)
{ long s=0;

  if (y>=0 && y<res_y-4)
   for (; *text!=0; text++, x+=6, s+=6)
    if (x>=0 && x<res_x-6)
    { asm volatile ("
	pushw %%es

	pushw %0				# load es register
	popw %%es

	movb %2, %%ah				# get color in ah

	movl $5, %%ecx
    0:
	pushl %%ecx
	movl $6, %%ecx

	movb (%%esi), %%bl			# get letter byte
	incl %%esi

    1:
	xorb %%al, %%al 			# decide which color to write
	shlb $1, %%bl
	jnc 2f
	movb %%ah, %%al
    2:
	stosb
	loop 1b

	popl %%ecx				# next scanline
	addl %1, %%edi

	loop 0b

	popw %%es"
       :
       : "g" (ldt), "g" (res_x-6), "g" (color),
	 "S" (&start_5x5_font[*text*5]), "D" (addr+x+y*res_x)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
    }

  return(s);
}

long _write_text(long x, long y, long color, char *text, long ldt, unsigned char *addr, long res_x, long res_y)
{ long s=0;

  if (y>=0 && y<res_y-5)
   for (; *text!=0; text++, x+=6, s+=6)
    if (x>=0 && x<res_x-6)
    { asm volatile ("
	pushw %%es

	pushw %0				# load es register
	popw %%es

	movb %2, %%ah				# get color in ah

	movl $5, %%ecx
    0:
	pushl %%ecx
	movl $6, %%ecx

	movb (%%esi), %%al			# get letter byte
	incl %%esi

    1:
	shlb $1, %%al				# decide to write pixel
	jnc 2f
	movb %%ah, %%es:(%%edi)
	movb $0, %%es:(%%ebx,%%edi,)
    2:
	incl %%edi
	loop 1b

	popl %%ecx				# next scanline
	addl %1, %%edi

	loop 0b

	popw %%es"
       :
       : "g" (ldt), "g" (res_x-6), "g" (color), "b" (res_x+1),
	 "S" (&start_5x5_font[*text*5]), "D" (addr+x+y*res_x)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
    }

  return(s);
}

