# Some functions to write structures into the buffer ubuf. To reduce
# computation time there are functions for clipping and scrolling.

		.extern _scr
		.extern _ubuf
		.extern _buf_x
		.extern _buf_y

		.globl _s_write_picture
		.globl _s_add_picture
		.globl _s_move_left
		.globl _s_move_right
		.globl _s_move_up
		.globl _s_move_down
		.globl __bkground

		.text

		.align 4

###############################################################################
# void s_write_picture(long x, long y, long sx, long sy, long nx, long ny, unsigned char *source);
###############################################################################
_s_write_picture:
		pushl	%ebp
		movl	%esp, %ebp
		
		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = x
        # 12(%ebp) = y
        # 16(%ebp) = sx
        # 20(%ebp) = sy
        # 24(%ebp) = nx
        # 28(%ebp) = ny
        # 32(%ebp) = source

		movl	32(%ebp),  %esi 			# esi=source
		orl 	%esi, %esi
		jz		wp_end

		movl	20(%ebp), %eax				# esi+=33*sy+sx
		addl	%eax, %esi
		shll	$5, %eax
		addl	%eax, %esi
		addl	16(%ebp), %esi

		movl	_ubuf, %edi 				# edi=ubuf+y*buf_x+x
		movl	12(%ebp), %eax
		movl	_buf_x, %edx
		imull	%edx, %eax
		movl	8(%ebp), %edx
		addl	%edx, %eax
		addl	%eax, %edi

		movl	24(%ebp), %edx				# edx=number of columns to write
		movl	$33, %eax					# eax=number to skip in source
		subl	%edx, %eax
		movl	_buf_x, %ebx				# ebx=number to skip in target
		subl	%edx, %ebx

		movl	28(%ebp), %ecx				# ecx=number of rows

		.align 4, 0x90

wp_y_loop:
		pushl	%ecx

		movl	%edx, %ecx
		andl	$0B11, %ecx
		rep
		movsb

		movl	%edx, %ecx
		shrl	$2, %ecx
		rep
		movsl

		addl	%eax, %esi					# next scanline
		addl	%ebx, %edi

		popl	%ecx
		loop	wp_y_loop

wp_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

		.align 4

###############################################################################
# void s_add_picture(long x,  long y,  long sx,  long sy,  long nx,  long ny,  unsigned char *source);
###############################################################################
_s_add_picture:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = x
		# 12(%ebp) = y
		# 16(%ebp) = sx
		# 20(%ebp) = sy
		# 24(%ebp) = nx
		# 28(%ebp) = ny
        # 32(%ebp) = source

		movl	32(%ebp), %edi				# edi=source+33*sy+sx
		orl 	%edi, %edi
		jz		ap_end

		movl	20(%ebp), %eax
		addl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		addl	16(%ebp), %edi

		movl	_ubuf, %esi 				# esi=target+buf_x*y+x
		movl	12(%ebp), %eax
		movl	_buf_x, %edx
		imull	%edx, %eax
		movl	8(%ebp), %edx
		addl	%edx, %eax
		addl	%eax, %esi

		movl	24(%ebp), %eax				# edx=number of bytes to skip in target
		movl	$33, %ebx					# ebx=number of bytes to skip in source
		subl	%eax, %ebx
		movl	_buf_x, %edx
		subl	%eax, %edx

		movb	$0xff, %al					# mask out code in al

		movl	28(%ebp), %ecx

		.align 4, 0x90

ap_y_loop:
		pushl	%ecx
		movl	24(%ebp), %ecx

		.align 4, 0x90

ap_scan_pixels:
		addl	%ecx, %esi
		repe
		scasb
		je		ap_end_loop
		subl	%ecx, %esi

		movb	-1(%edi), %ah
		movb	%ah, -1(%esi)

		orl 	%ecx, %ecx
		jnz 	ap_scan_pixels

ap_end_loop:
		addl	%edx, %esi
		addl	%ebx, %edi

		popl	%ecx
		loop	ap_y_loop

ap_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

		.align 4

###############################################################################
# void s_move_left(long n);
###############################################################################
_s_move_left:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = n

		movl	_ubuf, %esi 				# esi, edi=target
		movl	%esi, %edi

		movl	8(%ebp), %eax				# esi += n
		addl	%eax, %esi

		movl	%eax, %ebx					# ebx=n
		movl	_buf_x, %edx				# edx=buf_x-n
		subl	%ebx, %edx
		movl	_buf_y, %ecx				# ecx=buf_y

		.align 4, 0x90

ml_y_loop:
		pushl	%ecx
		movl	%edx, %ecx

		rep
		movsb

		addl	%ebx, %esi					# next scanline
		addl	%ebx, %edi

		popl	%ecx
		loop	ml_y_loop

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

		.align 4

###############################################################################
# void s_move_right(long n);
###############################################################################
_s_move_right:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = n

		std 								# direction down

		movl	_buf_x, %eax				# eax=buf_x*buf_y-1
		movl	_buf_y, %edx
		imull	%edx, %eax
		decl	%eax

		movl	_ubuf, %esi 				# esi, edi=target
		movl	%esi, %edi

		addl	%eax, %esi					# esi+=eax-n
		movl	8(%ebp), %edx
		subl	%edx, %esi

		addl	%eax, %edi					# edi+=eax

		movl	8(%ebp), %ebx				# ebx=n
		movl	_buf_x, %edx				# edx=buf_x*n
		subl	%ebx, %edx

		movl	_buf_y, %ecx				# ecx=buf_y

		.align 4, 0x90

mr_y_loop:
		pushl	%ecx
		movl	%edx, %ecx

		rep
		movsb

		subl	%ebx, %esi					# next scanline
		subl	%ebx, %edi

		popl	%ecx
		loop	mr_y_loop

		cld

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

		.align 4

###############################################################################
# void s_move_up(long n);
###############################################################################
_s_move_up:
		pushl	%ebp
        movl    %esp, %ebp

        pushl   %ebx
        pushl   %esi
        pushl   %edi

        # 8(%ebp) = n

		movl	_ubuf, %esi 				# esi, edi=target
		movl	%esi, %edi

		movl	8(%ebp), %eax				# esi+=buf_x*n
		movl	_buf_x, %edx
		imull	%edx, %eax
		addl	%eax, %esi

		movl	_buf_y, %eax				# ecx=(buf_y-n)*buf_x
		subl	8(%ebp), %eax
		movl	_buf_x, %ecx
		imull	%eax, %ecx

		pushl	%ecx
		andl	$0B11, %ecx
		rep
		movsb
		popl	%ecx

		shrl	$2, %ecx
		rep
		movsl

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
        ret

		.align 4

###############################################################################
# void s_move_down(long n);
###############################################################################
_s_move_down:
		pushl	%ebp
        movl    %esp, %ebp

        pushl   %ebx
        pushl   %esi
        pushl   %edi

        # 8(%ebp) = n

		std 								# direction up

		movl	_ubuf, %esi 				# esi, edi=target
		movl	%esi, %edi

		movl	_buf_y, %eax				# ecx=(buf_y-n)*buf_x
		subl	8(%ebp), %eax
		movl	_buf_x, %ecx
		imull	%eax, %ecx

		addl	%ecx, %esi					# esi+=ecx-1
		decl	%esi

		movl	_buf_y, %eax				# edi+=buf_x*buf_y-1
		movl	_buf_x, %edx
		imull	%edx, %eax
		addl	%eax, %edi
		decl	%edi

		pushl	%ecx						# save counter

		andl	$0B11, %ecx 				# align on dwords
		rep
		movsb

		popl	%ecx						# get counter div 4
		shrl	$2, %ecx

		subl	$3, %esi					# polong to next dword
		subl	$3, %edi

		rep
		movsl

		cld

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
        ret

		.align 4

###############################################################################
# long _bkground(long x, long y)
###############################################################################
__bkground:

        # 4(%esp) = x
        # 8(%esp) = y

        movl    8(%esp), %eax
        shll    $8, %eax
        addl    4(%esp), %eax
		addl	$0x2000, %eax
		movb	%cs:(%eax), %eax
        andl    $3, %eax

        ret

