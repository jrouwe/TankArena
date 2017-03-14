/* This is a very primitive interface in text mode, it is used by the
   setup routine.
*/

#include <tank.h>
#include <dpmi.h>
#include <stdarg.h>
#include <go32.h>
#include <sys/farptr.h>

static ulong text_base;

void m3_init(void)
{
  __dpmi_regs regs;

  regs.h.ah=0;
  regs.h.al=3;
  __dpmi_int(0x10, &regs);

  text_base = _farpeekw(_dos_ds, 0x463)==0x3b4? 0xb0000:0xb8000;
}

void m3_write_background(void)
{
  asm volatile ("
	pushw %%es
	pushw %0
	popw %%es

	movl $(80*25), %%ecx		# write background
	movw $0x1fb0, %%ax
	rep
	stosw

	popw %%es"
       :
       : "g" (_dos_ds), "D" (text_base)
       : "eax", "ecx", "edi");
}

void m3_clear_screen(void)
{
  asm volatile ("
	pushw %%es
	pushw %0
	popw %%es

	movl $(80*25), %%ecx		# clear video memory
	movw $0x0720, %%ax
	rep
	stosw

	popw %%es"
       :
       : "g" (_dos_ds), "D" (text_base)
       : "eax", "ecx", "edi");
}

void m3_write_shade(long x, long y, long nx, long ny)
{
  asm volatile ("
	pushw %%ds
	pushw %0
	popw %%ds

  0:
	pushl %%ecx
	movl %%edx, %%ecx

  1:					# set color attrib to 0x07
	movb $0x07, (%%edi)
	addl $2, %%edi
	loop 1b

	addl %%ebx, %%edi

	popl %%ecx
	loop 0b

	popw %%ds"
       :
       : "g" (_dos_ds), "D" (text_base+x*2+y*160+1), "b" (160-nx*2),
	 "c" (ny), "d" (nx)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
}

void m3_write_window(long x, long y, long nx, long ny, long color)
{
  static char frame[]="…Õª∫ ∫»Õº";

  asm volatile ("
	pushw %%es
	pushw %0
	popw %%es

	movb %1, %%ah			# color in ah

	movl %%edx, %%ecx		# write first line
	lodsb
	stosw
	lodsb
	rep
	stosw
	lodsb
	stosw
	addl %%ebx, %%edi

	movl %2, %%ecx			# write body
0:
	pushl %%ecx
	movl %%edx, %%ecx

	lodsb
	stosw
	lodsb
	rep
	stosw
	lodsb
	stosw

	addl %%ebx, %%edi
	subl $3, %%esi

	popl %%ecx
	loop 0b

	addl $3, %%esi

	movl %%edx, %%ecx		# write last line
	lodsb
	stosw
	lodsb
	rep
	stosw
	lodsb
        stosw

	popw %%es"
       :
       : "g" (_dos_ds), "g" (color), "g" (ny-2), "S" (frame),
	 "D" (text_base+x*2+y*160), "b" (160-nx*2), "d" (nx-2)
       : "eax", "ebx", "ecx", "edx", "edi");
}

void m3_write_text(long x, long y, char *text, ...)
{
  char text_string[80];
  va_list arg;

  va_start(arg, text);
  vsprintf(text_string, text, arg);

  asm volatile ("
	pushw %%es
	pushw %0
	popw %%es

  0:					# write string
	lodsb
	orb %%al, %%al
	jz 1f
	stosb
	incl %%edi
	jmp 0b

  1:
	popw %%es"
       :
       : "g" (_dos_ds), "S" (text_string), "D" (text_base+x*2+160*y)
       : "eax", "ebx", "ecx", "edx", "esi", "edi");
}

void m3_write_bar(long x, long y, long nx, long ny, long xor_color)
{
  asm volatile ("
	pushw %%ds
	pushw %0
	popw %%ds

  1:
	pushl %%ecx
	movl %%edx, %%ecx

  0:					# write bar
	xorb %%al, (%%esi)
	addl $2, %%esi
	loop 0b

	addl %%ebx, %%esi

	popl %%ecx
	loop 1b

	popw %%ds"
       :
       : "g" (_dos_ds), "a" (xor_color), "b" (160-nx*2), "c" (ny),
	 "d" (nx), "S" (text_base+x*2+160*y+1)
       : "eax", "ebx", "ecx", "edx", "edi");
}

