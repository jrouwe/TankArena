# Low level routines to determine pixel precise collisions. Wish we had
# used object collision by using bounding boxes/circles/triangles or so
# of every object.

		.globl _overlap_rb
		.globl _overlap_lb
		.globl _overlap_rt
		.globl _overlap_lt

		.text

		.align 4

###############################################################################
# long overlap_rb(unsigned char *picture1, long x, long y, unsigned char *picture2);
###############################################################################
_overlap_rb:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = picture1
		# 12(%ebp) = x
		# 16(%ebp) = y
		# 20(%ebp) = picture2

		movl	8(%ebp), %esi				# esi=picture1

		movl	20(%ebp), %edi				# edi=picture2+y*33+x
		movl	16(%ebp), %eax
		addl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		addl	12(%ebp), %edi

		xorl	%eax, %eax					# reset counter

		movl	$33, %edx					# check overlaps
		movl	%edx, %ecx
		subl	16(%ebp), %ecx
		jz		rb_end
		movl	12(%ebp), %ebx
		subl	%ebx, %edx
		jz		rb_end

		call	check_overlap

rb_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
		ret

		.align 4

###############################################################################
# long overlap_lb(unsigned char *picture1,	long x,  long y,  unsigned char *picture2);
###############################################################################
_overlap_lb:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = picture1
		# 12(%ebp) = x
		# 16(%ebp) = y
        # 20(%ebp) = picture2

		movl	8(%ebp), %esi				# esi=picture1+33-x
		addl	$33, %esi
		subl	12(%ebp), %esi

		movl	20(%ebp), %edi				# edi=picture2+y*33
		movl	16(%ebp), %eax
		addl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi

		xorl	%eax, %eax					# reset counter

		movl	$33, %ecx					# check for overlaps
		subl	16(%ebp), %ecx
		jz		lb_end
		movl	12(%ebp), %edx
		orl 	%edx, %edx
		jz		lb_end
		movl	$33, %ebx
		subl	%edx, %ebx

		call	check_overlap

lb_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
        ret

		.align 4

###############################################################################
# long overlap_rt(unsigned char *picture1,	long x,  long y,  unsigned char *picture2);
###############################################################################
_overlap_rt:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = picture1
		# 12(%ebp) = x
		# 16(%ebp) = y
        # 20(%ebp) = picture2

		movl	8(%ebp), %esi				# esi=picture1+(33-y)*33
        movl    $33, %eax
        subl    16(%ebp), %eax
        addl    %eax, %esi
        shll    $5, %eax
        addl    %eax, %esi

		movl	20(%ebp), %edi				# edi=picture2+x
		addl	12(%ebp), %edi

		xorl	%eax, %eax					# reset counter

		movl	16(%ebp), %ecx				# check for overlaps
		jcxz	rt_end
		movl	$33, %edx
		movl	12(%ebp), %ebx
		subl	%ebx, %edx
		jz		rt_end

		call	check_overlap

rt_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
        ret

		.align 4

###############################################################################
# long overlap_lt(unsigned char *picture1,	long x,  long y,  unsigned char *picture2);
###############################################################################
_overlap_lt:
		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = picture1
		# 12(%ebp) = x
		# 16(%ebp) = y
        # 20(%ebp) = picture2

		movl	8(%ebp), %esi				#esi=picture1+(33-y)*33+(33-x)
		movl	$33, %eax
		subl	16(%ebp), %eax
		incl	%eax
		addl	%eax, %esi
		shll	$5, %eax
		addl	%eax, %esi
		subl	12(%ebp), %esi

		movl	20(%ebp), %edi				# edi=picture2

		xorl	%eax, %eax					# reset counter

		movl	16(%ebp), %ecx				# check for overlaps
		jcxz	lt_end
		movl	12(%ebp), %edx
		orl 	%edx, %edx
		jz		lt_end
		movl	$33, %ebx
		subl	%edx, %ebx

		call	check_overlap

lt_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp
        ret

		.align 4

###############################################################################
# function: check_overlap
###############################################################################
# esi=pointer to picture1,
# edi=pointer to picture2,
# eax=counter of overlapping pixels,
# ecx=number of rows,
# edx=number of columns,
# ebx=number of bytes to add at end of loop.
###############################################################################
check_overlap:
		pushl	%ecx
		movl	%edx, %ecx

co_x_loop:
		cmpb	$0xff, (%edi)
		je		co_no_overlap
		cmpb	$0xff, (%esi)
		je		co_no_overlap
		incl	%eax

co_no_overlap:
		incl	%esi
		incl	%edi
		loop	co_x_loop

		addl	%ebx, %esi
		addl	%ebx, %edi

		popl	%ecx
		loop	check_overlap

		ret 	

