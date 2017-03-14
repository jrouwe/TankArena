# functions for detecting and getting input from joysticks, this is not very easy
# because you have to measure a small delay, and so dummy instructions have
# to be executed to make the counters always equal.

		.globl _detect_joystick
		.globl _read_joy_buttons
		.globl _read_joystick

###############################################################################
# long detect_joystick(long joy);
###############################################################################
_detect_joystick:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx

		# 8(%ebp) = joy

		cli							# disable interrupts

		movb	$0B11, %ah					# get bits to test
		movb	8(%ebp), %cl
		shlb	$1, %cl
		shlb	%cl, %ah

		movw	$0x201, %dx					# check if bits fall to zero
		outb	%al, %dx
		movl	$0x0fffff, %ecx
dj_loop:

		inb 	%dx, %al
		testb	%ah, %al
		loopnz	dj_loop

		sti							# enable interrupts

		movl	%ecx, %eax					# ecx=0 -> no joystick present

		popl	%ebx
		popl	%ebp
		ret

###############################################################################
# long read_joy_buttons(void);
###############################################################################
_read_joy_buttons:
		xorl	%eax, %eax					# clear eax

		movw	$0x201, %dx					# get buttons
		inb 	%dx, %al

		notb	%al 						# put them in bit 0..3
		shrb	$4, %al

		ret

###############################################################################
	# void read_joystick(int j,  long *x,  long *y);
###############################################################################
_read_joystick:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = joy
		# 12(%ebp) = x
		# 16(%ebp) = y

		xorl	%esi, %esi					# reset counters
		xorl	%edi, %edi

		movb	8(%ebp), %cl				# get number of bits to shift
		shlb	$1, %cl

		movw	$0x201, %dx 				# joystick port in dx

		cli 								# disable interrupts

rj_wait_zero:								# wait until joystick ready
		inb 	%dx, %al
		shrb	%cl, %al
		testb	$0B11, %al
		jnz 	rj_wait_zero

		outb	%al, %dx					# start RC circuit

rj_loop:
		inb 	%dx, %al					# read status
		shrb	%cl, %al

		testb	$0B1, %al					# check end x
		jz		rj_not_x_zero
		incl	%esi
		jnz 	rj_x_zero					# equal timing
rj_not_x_zero:
		incl	%ebx
rj_x_zero:

		testb	$0B10, %al					# check end y
		jz		rj_not_y_zero
		incl	%edi
		jnz 	rj_y_zero					# equal timing
rj_not_y_zero:
		incl	%ebx
rj_y_zero:

		testb	$0B11, %al					# test end loop
		jnz 	rj_loop

		sti 								# enable interrupts

		movl	12(%ebp), %ebx	# store values
		movl	%esi, (%ebx)
		movl	16(%ebp), %ebx
		movl	%edi, (%ebx)

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

