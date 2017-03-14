# These are functions that write sprites and shadows in the buffer scr.

		.extern _buf_x
		.extern _buf_y
		.extern _scr
		.extern _gray

		.global _b_first_layer
		.global _b_add_layer
		.global _b_add_shadow
		.global _b_set_layer
		.global _b_add_layer_set_high
		.global _b_add_layer_x
		.global _b_add_layer_y
		.global _b_add_layer_xy
		
		.text

		.align 4

################################################################################
# b_first_layer(long x, long y, unsigned char *layer);
################################################################################
_b_first_layer:
		cmpl	$0, 12(%esp)		# check if there is a layer
		je		fl_abort

		pushl	%ebp
		movl	%esp, %ebp

		# 8(%esp) = x
		# 12(%esp) = y
		# 16(%esp) = layer

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		xorl	%ebx, %ebx			# clear ebx and ecx
		xorl	%ecx, %ecx

		movl	12(%ebp), %eax		# eax=y

		orl 	%eax, %eax			# check y<0
		jns 	fl_not_neg_y

		cmpl	$-33, %eax			# end if y<=-b_size
		jbe 	fl_end

		movl	$33, %ecx			# ecx=b_size-abs(y)
		addl	%eax, %ecx
		xorl	%edi, %edi			# edi=0
		negl	%eax				# esi=b_size*abs(y)
		movl	%eax, %esi
		shll	$5, %eax
		addl	%eax, %esi
		movl	_buf_x, %edx		# edx=buf_x
		jmp 	fl_check_x			# check x coordinates

		.align 4

fl_not_neg_y:
		movl	_buf_y, %ebx		# ebx=buf_y

		cmpl	%ebx, %eax			# end if y>=buf_y
		jae 	fl_end

		movl	_buf_x, %edx		# edx=buf_x

		subl	$33, %ebx			# check y>buf_y-b_size
		cmpl	%ebx, %eax
		jbe 	fl_not_overflow_y

		movl	_buf_y, %ecx		# ecx=buf_y-y
		subl	%eax, %ecx
		imull	%edx, %eax			# edi=buf_x*y
		movl	%eax, %edi
		xorl	%esi, %esi			# esi=0
		jmp 	fl_check_x			# check x coordinates

		.align 4

fl_not_overflow_y:
		movl	$33, %ecx			# ecx=b_size
		imull	%edx, %eax			# edi=buf_x*y
		movl	%eax, %edi
		xorl	%esi, %esi			# esi=0

fl_check_x:
		movl	8(%ebp), %eax		# eax=x

		orl 	%eax, %eax			# check x<0
		jns 	fl_not_neg_x

		cmpl	$-33, %eax			# end if x<=-b_size
		jbe 	fl_end

		movl	$33, %ebx			# ebx=b_size-abs(x)
		addl	%eax, %ebx
		subl	%eax, %esi			# esi+=abs(x)
		jmp 	fl_make_pointer

		.align 4

fl_not_neg_x:
		movl	%edx, %ebx			# end if x>=buf_x
		cmpl	%ebx, %eax
		jae 	fl_end
		subl	$33, %ebx			# check x>buf_x-b_size
		cmpl	%ebx, %eax
		jbe 	fl_not_overflow_x
		movl	%edx, %ebx			# ebx=buf_x-x
		subl	%eax, %ebx
		addl	%eax, %edi			# edi+=x
		jmp 	fl_make_pointer

		.align 4

fl_not_overflow_x:
		movl	$33, %ebx			# ebx=b_size
		addl	%eax, %edi			# edi+=x

fl_make_pointer:
		addl	_scr, %edi			# point edi to screen buffer

		addl	16(%ebp), %esi		# point esi to layer

		subl	%ebx, %edx			# buf_x-ebx in edx

		movl	$33, %ebp			# 33-ebx in ebp
		subl	%ebx, %ebp

		movb	$0xff, %al			# mask code in al

		# esi=pointer to layer,
		# edi=pointer to scr,
		# ebx=number of columns to write,
		# ecx=number of rows to write,
		# edx=buf_x-ebx,
		# ebp=33-ebx,
		# al=0xff

		.align 4, 0x90

fl_y_loop:
		pushl	%ecx
		movl	%ebx, %ecx
		rep
		movsb
		popl	%ecx

		addl	%edx, %edi
		addl	%ebp, %esi

		loop	fl_y_loop

fl_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

fl_abort:
		ret

		.align 4

################################################################################
# b_add_layer(long x, long y, unsigned char *layer);
################################################################################
_b_add_layer:
        cmpl    $0, 12(%esp)        # check if there is a layer
        je      al_abort

        pushl   %ebp
        movl    %esp, %ebp

        # 8(%esp) = x
        # 12(%esp) = y
        # 16(%esp) = layer

        pushl   %ebx
        pushl   %esi
        pushl   %edi

        xorl    %ebx, %ebx          # clear ebx and ecx
        xorl    %ecx, %ecx

        movl    12(%ebp), %eax      # eax=y
        
        orl     %eax, %eax          # check y<0
        jns     al_not_neg_y

        cmpl    $-33, %eax          # end if y<=-b_size
        jbe     al_end

        movl    $33, %ecx           # ecx=b_size-abs(y)
        addl    %eax, %ecx
        xorl    %esi, %esi          # esi=0
        negl    %eax                # edi=b_size*abs(y)
        movl    %eax, %edi
        shll    $5, %eax
        addl    %eax, %edi
        movl    _buf_x, %edx        # edx=buf_x
        jmp     al_check_x          # check x coordinates

        .align 4

al_not_neg_y:
        movl    _buf_y, %ebx        # ebx=buf_y

        cmpl    %ebx, %eax          # end if y>=buf_y
        jae     al_end

        movl    _buf_x, %edx        # edx=buf_x

        subl    $33, %ebx           # check y>buf_y-b_size
        cmpl    %ebx, %eax
        jbe     al_not_overflow_y

        movl    _buf_y, %ecx        # ecx=buf_y-y
        subl    %eax, %ecx
        imull   %edx, %eax          # esi=buf_x*y
        movl    %eax, %esi
        xorl    %edi, %edi          # edi=0
        jmp     al_check_x          # check x coordinates

        .align 4

al_not_overflow_y:
        movl    $33, %ecx           # ecx=b_size
        imull   %edx, %eax          # esi=buf_x*y
        movl    %eax, %esi
        xorl    %edi, %edi          # edi=0

al_check_x:
        movl    8(%ebp), %eax       # eax=x

        orl     %eax, %eax          # check x<0
        jns     al_not_neg_x

        cmpl    $-33, %eax          # end if x<=-b_size
        jbe     al_end

        movl    $33, %ebx           # ebx=b_size-abs(x)
        addl    %eax, %ebx
        subl    %eax, %edi          # edi+=abs(x)
        jmp     al_make_pointer

        .align 4

al_not_neg_x:
        movl    %edx, %ebx          # end if x>=buf_x
        cmpl    %ebx, %eax
        jae     al_end
        subl    $33, %ebx           # check x>buf_x-b_size
        cmpl    %ebx, %eax
        jbe     al_not_overflow_x
        movl    %edx, %ebx          # ebx=buf_x-x
        subl    %eax, %ebx
        addl    %eax, %esi          # esi+=x
        jmp     al_make_pointer

        .align 4

al_not_overflow_x:
        movl    $33, %ebx           # ebx=b_size
        addl    %eax, %esi          # esi+=x

al_make_pointer:
        addl    _scr, %esi          # point esi to screen buffer - 1
        decl    %esi

        addl    16(%ebp), %edi      # point edi to layer

        subl    %ebx, %edx          # buf_x-ebx in edx

        movl    $33, %ebp           # 33-ebx in ebp
        subl    %ebx, %ebp

        movb    $0xff, %al          # mask code in al

        # esi=pointer to scr,
        # edi=pointer to layer,
        # ebx=number of columns to write,
        # ecx=number of rows to write,
        # edx=buf_x-ebx,
        # ebp=33-ebx,
        # al=0xff

		.align 4, 0x90

al_y_loop:
        pushl   %ecx
        movl    %ebx, %ecx

		.align 4, 0x90

al_x_loop:
        addl    %ecx, %esi          # search for next byte != 0xff
        repe
		scasb
        je      al_end_x_loop
        subl    %ecx, %esi

        movb    -1(%edi), %ah       # move byte
        movb    %ah,(%esi)

        orl     %ecx, %ecx
        jnz     al_x_loop

al_end_x_loop:
        addl    %edx, %esi          # next line
        addl    %ebp, %edi

        popl    %ecx
        loop    al_y_loop

al_end:
        popl    %edi
        popl    %esi
        popl    %ebx
        popl    %ebp

al_abort:
        ret

		.align 4

################################################################################
# b_set_layer(long x, long y, unsigned char *layer);
################################################################################
_b_set_layer:
        cmpl    $0, 12(%esp)        # check if there is a layer
		je		sl_abort

        pushl   %ebp
        movl    %esp, %ebp

        # 8(%esp) = x
        # 12(%esp) = y
        # 16(%esp) = layer

        pushl   %ebx
        pushl   %esi
        pushl   %edi

        xorl    %ebx, %ebx          # clear ebx and ecx
        xorl    %ecx, %ecx

        movl    12(%ebp), %eax      # eax=y
        
        orl     %eax, %eax          # check y<0
		jns 	sl_not_neg_y

        cmpl    $-33, %eax          # end if y<=-b_size
		jbe 	sl_end

        movl    $33, %ecx           # ecx=b_size-abs(y)
        addl    %eax, %ecx
        xorl    %esi, %esi          # esi=0
        negl    %eax                # edi=b_size*abs(y)
        movl    %eax, %edi
        shll    $5, %eax
        addl    %eax, %edi
        movl    _buf_x, %edx        # edx=buf_x
		jmp 	sl_check_x			# check x coordinates

        .align 4

sl_not_neg_y:
        movl    _buf_y, %ebx        # ebx=buf_y

        cmpl    %ebx, %eax          # end if y>=buf_y
		jae 	sl_end

        movl    _buf_x, %edx        # edx=buf_x

        subl    $33, %ebx           # check y>buf_y-b_size
        cmpl    %ebx, %eax
		jbe 	sl_not_overflow_y

        movl    _buf_y, %ecx        # ecx=buf_y-y
        subl    %eax, %ecx
        imull   %edx, %eax          # esi=buf_x*y
        movl    %eax, %esi
        xorl    %edi, %edi          # edi=0
		jmp 	sl_check_x			# check x coordinates

        .align 4

sl_not_overflow_y:
        movl    $33, %ecx           # ecx=b_size
        imull   %edx, %eax          # esi=buf_x*y
        movl    %eax, %esi
        xorl    %edi, %edi          # edi=0

sl_check_x:
        movl    8(%ebp), %eax       # eax=x

        orl     %eax, %eax          # check x<0
		jns 	sl_not_neg_x

        cmpl    $-33, %eax          # end if x<=-b_size
		jbe 	sl_end

        movl    $33, %ebx           # ebx=b_size-abs(x)
        addl    %eax, %ebx
        subl    %eax, %edi          # edi+=abs(x)
		jmp 	sl_make_pointer

        .align 4

sl_not_neg_x:
        movl    %edx, %ebx          # end if x>=buf_x
        cmpl    %ebx, %eax
		jae 	sl_end
        subl    $33, %ebx           # check x>buf_x-b_size
        cmpl    %ebx, %eax
		jbe 	sl_not_overflow_x
        movl    %edx, %ebx          # ebx=buf_x-x
        subl    %eax, %ebx
        addl    %eax, %esi          # esi+=x
		jmp 	sl_make_pointer

        .align 4

sl_not_overflow_x:
        movl    $33, %ebx           # ebx=b_size
        addl    %eax, %esi          # esi+=x

sl_make_pointer:
        addl    _scr, %esi          # point esi to screen buffer - 1
        decl    %esi

        addl    16(%ebp), %edi      # point edi to layer

        subl    %ebx, %edx          # buf_x-ebx in edx

        movl    $33, %ebp           # 33-ebx in ebp
        subl    %ebx, %ebp

        movb    $0xff, %al          # mask code in al

        # esi=pointer to scr,
        # edi=pointer to layer,
        # ebx=number of columns to write,
        # ecx=number of rows to write,
        # edx=buf_x-ebx,
        # ebp=33-ebx,
        # al=0xff

		.align 4, 0x90

sl_y_loop:
        pushl   %ecx
        movl    %ebx, %ecx

		.align 4, 0x90

sl_x_loop:
        addl    %ecx, %esi          # search for next byte != 0xff
        repe
		scasb
		je		sl_end_x_loop
        subl    %ecx, %esi

		orb 	$0x80, (%esi)		# set highest bit

        orl     %ecx, %ecx
		jnz 	sl_x_loop

sl_end_x_loop:
        addl    %edx, %esi          # next line
        addl    %ebp, %edi

        popl    %ecx
		loop	sl_y_loop

sl_end:
        popl    %edi
        popl    %esi
        popl    %ebx
        popl    %ebp

sl_abort:
        ret

		.align 4

################################################################################
# b_add_layer_set_high(long x, long y, unsigned char *layer);
################################################################################
_b_add_layer_set_high:
		cmpl	$0, 12(%esp)		# check if there is a layer
		je		alsh_abort

		pushl	%ebp
		movl	%esp, %ebp

		# 8(%esp) = x
		# 12(%esp) = y
		# 16(%esp) = layer

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		xorl	%ebx, %ebx			# clear ebx and ecx
		xorl	%ecx, %ecx

		movl	12(%ebp), %eax		# eax=y

		orl 	%eax, %eax			# check y<0
		jns 	alsh_not_neg_y

		cmpl	$-33, %eax			# end if y<=-b_size
		jbe 	alsh_end

		movl	$33, %ecx			# ecx=b_size-abs(y)
		addl	%eax, %ecx
		xorl	%esi, %esi			# esi=0
		negl	%eax				# edi=b_size*abs(y)
		movl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		movl	_buf_x, %edx		# edx=buf_x
		jmp 	alsh_check_x		# check x coordinates

		.align 4

alsh_not_neg_y:
		movl	_buf_y, %ebx		# ebx=buf_y

		cmpl	%ebx, %eax			# end if y>=buf_y
		jae 	alsh_end

		movl	_buf_x, %edx		# edx=buf_x

		subl	$33, %ebx			# check y>buf_y-b_size
		cmpl	%ebx, %eax
		jbe 	alsh_not_overflow_y

		movl	_buf_y, %ecx		# ecx=buf_y-y
		subl	%eax, %ecx
		imull	%edx, %eax			# esi=buf_x*y
		movl	%eax, %esi
		xorl	%edi, %edi			# edi=0
		jmp 	alsh_check_x		# check x coordinates

		.align 4

alsh_not_overflow_y:
		movl	$33, %ecx			# ecx=b_size
		imull	%edx, %eax			# esi=buf_x*y
		movl	%eax, %esi
		xorl	%edi, %edi			# edi=0

alsh_check_x:
		movl	8(%ebp), %eax		# eax=x

		orl 	%eax, %eax			# check x<0
		jns 	alsh_not_neg_x

		cmpl	$-33, %eax			# end if x<=-b_size
		jbe 	alsh_end

		movl	$33, %ebx			# ebx=b_size-abs(x)
		addl	%eax, %ebx
		subl	%eax, %edi			# edi+=abs(x)
		jmp 	alsh_make_pointer

		.align 4

alsh_not_neg_x:
		movl	%edx, %ebx			# end if x>=buf_x
		cmpl	%ebx, %eax
		jae 	alsh_end
		subl	$33, %ebx			# check x>buf_x-b_size
		cmpl	%ebx, %eax
		jbe 	alsh_not_overflow_x
		movl	%edx, %ebx			# ebx=buf_x-x
		subl	%eax, %ebx
		addl	%eax, %esi			# esi+=x
		jmp 	alsh_make_pointer

		.align 4

alsh_not_overflow_x:
		movl	$33, %ebx			# ebx=b_size
		addl	%eax, %esi			# esi+=x

alsh_make_pointer:
		addl	_scr, %esi			# point esi to screen buffer - 1
		decl	%esi

		addl	16(%ebp), %edi		# point edi to layer

		subl	%ebx, %edx			# buf_x-ebx in edx

		movl	$33, %ebp			# 33-ebx in ebp
		subl	%ebx, %ebp

		movb	$0xff, %al			# mask code in al

		# esi=pointer to scr,
		# edi=pointer to layer,
		# ebx=number of columns to write,
		# ecx=number of rows to write,
		# edx=buf_x-ebx,
		# ebp=33-ebx,
		# al=0xff

		.align 4, 0x90

alsh_y_loop:
		pushl	%ecx
		movl	%ebx, %ecx

		.align 4, 0x90

alsh_x_loop:
		addl	%ecx, %esi			# search for next byte != 0xff
		repe
		scasb
		je		alsh_end_x_loop
		subl	%ecx, %esi

		movb	-1(%edi), %ah		# move byte and set high bit
		orb 	$0x80, %ah
		movb	%ah, (%esi)

		orl 	%ecx, %ecx
		jnz 	alsh_x_loop

alsh_end_x_loop:
		addl	%edx, %esi			# next line
		addl	%ebp, %edi

		popl	%ecx
		loop	alsh_y_loop

alsh_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

alsh_abort:
		ret

		.align 4

################################################################################
# b_add_shadow(long x, long y, unsigned char *layer);
################################################################################
_b_add_shadow:
		cmpl	$0, 12(%esp)		# check if there is a layer
		je		as_abort

		pushl	%ebp
		movl	%esp, %ebp

		# 8(%esp) = x
		# 12(%esp) = y
		# 16(%esp) = layer

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		xorl	%ebx, %ebx			# clear ebx and ecx
		xorl	%ecx, %ecx

		movl	12(%ebp), %eax		# eax=y

		orl 	%eax, %eax			# check y<0
		jns 	as_not_neg_y

		cmpl	$-33, %eax			# end if y<=-b_size
		jbe 	as_end

		movl	$33, %ecx			# ecx=b_size-abs(y)
		addl	%eax, %ecx
		xorl	%esi, %esi			# esi=0
		negl	%eax				# edi=b_size*abs(y)
		movl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		movl	_buf_x, %edx		# edx=buf_x
		jmp 	as_check_x			# check x coordinates

		.align 4

as_not_neg_y:
		movl	_buf_y, %ebx		# ebx=buf_y

		cmpl	%ebx, %eax			# end if y>=buf_y
		jae 	as_end

		movl	_buf_x, %edx		# edx=buf_x

		subl	$33, %ebx			# check y>buf_y-b_size
		cmpl	%ebx, %eax
		jbe 	as_not_overflow_y

		movl	_buf_y, %ecx		# ecx=buf_y-y
		subl	%eax, %ecx
		imull	%edx, %eax			# esi=buf_x*y
		movl	%eax, %esi
		xorl	%edi, %edi			# edi=0
		jmp 	as_check_x			# check x coordinates

		.align 4

as_not_overflow_y:
		movl	$33, %ecx			# ecx=b_size
		imull	%edx, %eax			# esi=buf_x*y
		movl	%eax, %esi
		xorl	%edi, %edi			# edi=0

as_check_x:
		movl	8(%ebp), %eax		# eax=x

		orl 	%eax, %eax			# check x<0
		jns 	as_not_neg_x

		cmpl	$-33, %eax			# end if x<=-b_size
		jbe 	as_end

		movl	$33, %ebx			# ebx=b_size-abs(x)
		addl	%eax, %ebx
		subl	%eax, %edi			# edi+=abs(x)
		jmp 	as_make_pointer

		.align 4

as_not_neg_x:
		movl	%edx, %ebx			# end if x>=buf_x
		cmpl	%ebx, %eax
		jae 	as_end
		subl	$33, %ebx			# check x>buf_x-b_size
		cmpl	%ebx, %eax
		jbe 	as_not_overflow_x
		movl	%edx, %ebx			# ebx=buf_x-x
		subl	%eax, %ebx
		addl	%eax, %esi			# esi+=x
		jmp 	as_make_pointer

		.align 4

as_not_overflow_x:
		movl	$33, %ebx			# ebx=b_size
		addl	%eax, %esi			# esi+=x

as_make_pointer:
		addl	_scr, %esi			# point esi to screen buffer - 1
		decl	%esi

		addl	16(%ebp), %edi		# point edi to layer

		subl	%ebx, %edx			# buf_x-ebx in edx

		movl	%ebx, %ebp			# ebp=ebx

		leal	_gray, %ebx 		# point ebx to shadow

		# esi=pointer to scr,
		# edi=pointer to layer,
		# ebx=shadow map,
		# ecx=number of rows to write,
		# edx=buf_x-ebx,
		# ebp=number of rows to write,

		.align 4, 0x90

as_y_loop:
		pushl	%ecx
		movl	%ebp, %ecx

		.align 4, 0x90

as_x_loop:
		movb	$0xff, %al

		addl	%ecx, %esi			# search for next byte != 0xff
		repe
		scasb
		je		as_end_x_loop
		subl	%ecx, %esi

		movb	(%esi), %al 		# move byte
		xlat
		movb	%al,(%esi)

		orl 	%ecx, %ecx
		jnz 	as_x_loop

as_end_x_loop:
		addl	%edx, %esi			# next line
		addl	$33, %edi
		subl	%ebp, %edi

		popl	%ecx
		loop	as_y_loop

as_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

as_abort:
		ret

		.align 4
		
###############################################################################
# void add_layer_x(long x, long y, unsigned char *layer);
###############################################################################
_b_add_layer_x:
		cmpl	$0, 12(%esp)				# check null pointer
		je		alx_abort

		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = x
		# 12(%ebp) = y
		# 16(%ebp) = layer

		xorl	%ebx, %ebx					# clear ebx and ecx
        xorl    %ecx, %ecx

		movl	_buf_x, %edx				# put buf_x in edx

		movl	12(%ebp), %eax				# ax=y

		orl 	%eax, %eax					# check y<0
		jns 	alx_not_neg_y
		cmpl	$-33, %eax					# end if y<=-33
		jbe 	alx_end
		movl	$33, %ecx					# ecx=33-abs(y)
        addl    %eax, %ecx
		xorl	%esi, %esi					# esi=0
		addl	$33-1, %eax 				# edi=33*(32-abs(y))
		movl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		jmp 	alx_check_x 				# check x coordinates

		.align 4

alx_not_neg_y:
		movl	_buf_y, %ebx				# end if y>=buf_y
        cmpl    %ebx, %eax
		jae 	alx_end
		subl	$33, %ebx					# check y>buf_y-33
        cmpl    %ebx, %eax
		jbe 	alx_not_overflow_y
		movl	_buf_y, %ecx				# ecx=buf_y-y
        subl    %eax, %ecx
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		movl	$(32*33), %edi				# edi=32*33
		jmp 	alx_check_x 				# check x coordinates

		.align 4

alx_not_overflow_y: 						# not at boundary
		movl	$33, %ecx					# ecx=33
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		movl	$(32*33), %edi				# edi=(32)*33

alx_check_x:
		movl	8(%ebp), %eax				# ax=x

		orl 	%eax, %eax					# check x<0
		jns 	alx_not_neg_x
		cmpl	$-33, %eax					# end if x<=-33
		jbe 	alx_end
		movl	$33, %ebx					# ebx=33-abs(x)
        addl    %eax, %ebx
		subl	%eax, %edi					# edi+=abs(x)
		jmp 	alx_make_pointer

		.align 4

alx_not_neg_x:
		movl	%edx, %ebx					# end if x>=buf_x
        cmpl    %ebx, %eax
		jae 	alx_end
		subl	$33, %ebx					# check x>buf_x-33
        cmpl    %ebx, %eax
		jbe 	alx_not_overflow_x
		movl	%edx, %ebx					# bx=buf_x-x
        subl    %eax, %ebx
		addl	%eax, %esi					# esi+=x
		jmp 	alx_make_pointer

		.align 4

alx_not_overflow_x: 						# not at boundary
		movl	$33, %ebx					# bx=33
		addl	%eax, %esi					# esi+=x

alx_make_pointer:
		subl	%ebx, %edx					# buf_x-ebx in edx

		addl	_scr, %esi					# point esi to screen buffer - 1
        decl    %esi

		addl	16(%ebp), %edi				# point edi to layer

		# esi=pointer to buffer,
        # edi=pointer to layer,
        # ebx=number of columns to write,
        # ecx=number of rows to write.
        # edx=buf_x-ebx

		movb	$0x0ff, %al 				# mask out code in al

		movl	$33, %ebp					# ebp=33+ebx
		addl	%ebx, %ebp

		.align 4, 0x90

alx_y_loop: 								# add layer
		pushl	%ecx
		movl	%ebx, %ecx

		.align 4, 0x90

alx_scan_pixels:							# search for next byte != 0ffh
		addl	%ecx, %esi
		repe
		scasb
		je		alx_end_loop
		subl	%ecx, %esi

		movb	-1(%edi), %ah				# copy
		movb	%ah, (%esi)

		orl 	%ecx, %ecx					# loop
		jnz 	alx_scan_pixels

alx_end_loop:
		addl	%edx, %esi
		subl	%ebp, %edi

		popl	%ecx
		loop	alx_y_loop

alx_end:
		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

alx_abort:
		ret

		.align 4

###############################################################################
# void add_layer_y(long x, long y, unsigned char *layer);
###############################################################################
_b_add_layer_y:
		cmp 	$0, 12(%esp)				# check null pointer
		je		aly_abort

		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = x
		# 12(%ebp) = y
        # 16(%ebp) = layer

		xorl	%ebx, %ebx					# clear ebx and ecx
        xorl    %ecx, %ecx

		movl	_buf_x, %edx				# put buf_x in edx

		movl	12(%ebp), %eax				# ax=y

		orl 	%eax, %eax					# check y<0
		jns 	aly_not_neg_y
		cmpl	$-33, %eax					# end if y<=-33
		jbe 	aly_end
		movl	$33, %ecx					# ecx=33-abs(y)
        addl    %eax, %ecx
		xorl	%esi, %esi					# esi=0
		negl	%eax						# edi=33*abs(y)
		movl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		jmp 	aly_check_x 				# check x coordinates

		.align 4

aly_not_neg_y:
		movl	_buf_y, %ebx				# end if y>=buf_y
        cmpl    %ebx, %eax
		jae 	aly_end
		subl	$33, %ebx					# check y>buf_y-33
        cmpl    %ebx, %eax
		jbe 	aly_not_overflow_y
		movl	_buf_y, %ecx				# ecx=buf_y-y
        subl    %eax, %ecx
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		xorl	%edi, %edi					# edi=0
		jmp 	aly_check_x 				# check x coordinates

		.align 4

aly_not_overflow_y: 						# not at boundary
		movl	$33, %ecx					# ecx=33
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		xorl	%edi, %edi					# edi=0

aly_check_x:
		movl	8(%ebp), %eax				# ax=x

		orl 	%eax, %eax					# check x<0
		jns 	aly_not_neg_x
		cmpl	$-33, %eax					# end if x<=-33
		jbe 	aly_end
		movl	$33, %ebx					# ebx=33-abs(x)
        addl    %eax, %ebx
		addl	$32, %eax					# edi+=32-abs(x)
        addl    %eax, %edi
		jmp 	aly_make_pointer

		.align 4

aly_not_neg_x:
		movl	%edx, %ebx					# end if x>=buf_x
        cmpl    %ebx, %eax
		jae 	aly_end
		subl	$33, %ebx					# check x>buf_x-33
        cmpl    %ebx, %eax
		jbe 	aly_not_overflow_x
		movl	%edx, %ebx					# bx=buf_x-x
        subl    %eax, %ebx
		addl	%eax, %esi					# esi+=x
		addl	$32, %edi					# edi+=32
		jmp 	aly_make_pointer

		.align 4

aly_not_overflow_x: 						# not at boundary
		movl	$33, %ebx					# bx=33
		addl	%eax, %esi					# esi+=x
		addl	$32, %edi					# edi+=32

aly_make_pointer:
		subl	%ebx, %edx					# buf_x-ebx in edx

		addl	_scr, %esi					# point esi to screen buffer - 1
        decl    %esi

		addl	16(%ebp), %edi				# point edi to layer

		# esi=pointer to buffer,
        # edi=pointer to layer,
        # ebx=number of columns to write,
        # ecx=number of rows to write.
        # edx=buf_x-ebx

		std 								# scan backwards

		movb	$0xff, %al					# mask out code in al

		movl	$33, %ebp					# ebp=33+ebx
		addl	%ebx, %ebp

		.align 4, 0x90

aly_y_loop: 								# add layer
		pushl	%ecx
		movl	%ebx, %ecx

		.align 4, 0x90

aly_scan_pixels:							# search for next byte != 0ffh
		addl	%ecx, %esi
		repe
		scasb
		je		aly_end_loop
		subl	%ecx, %esi

		movb	1(%edi), %ah				# copy
		movb	%ah, (%esi)

		orl 	%ecx, %ecx					# loop
		jnz 	aly_scan_pixels

aly_end_loop:
		addl	%edx, %esi
		addl	%ebp, %edi

		popl	%ecx
		loop	aly_y_loop

aly_end:
		cld 								# direction down again

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

aly_abort:
        ret
   
		.align 4

###############################################################################
# void add_layer_xy(long x, long y, unsigned char *layer);
###############################################################################
_b_add_layer_xy:
		cmp 	$0, 12(%esp)				# check null pointer
		je		alxy_abort

		pushl	%ebp
		movl	%esp, %ebp

		pushl	%ebx
		pushl	%esi
		pushl	%edi

		# 8(%ebp) = x
		# 12(%ebp) = y
        # 16(%ebp) = layer

		xorl	%ebx, %ebx					# clear ebx and ecx
        xorl    %ecx, %ecx

		movl	_buf_x, %edx				# put buf_x in edx

		movl	12(%ebp), %eax				# ax=y

		orl 	%eax, %eax					# check y<0
		jns 	alxy_not_neg_y
		cmpl	$-33, %eax					# end if y<=-33
		jbe 	alxy_end
		movl	$33, %ecx					# ecx=33-abs(y)
        addl    %eax, %ecx
		xorl	%esi, %esi					# esi=0
		addl	$32, %eax					# edi=33*(32-abs(y))
		movl	%eax, %edi
		shll	$5, %eax
		addl	%eax, %edi
		jmp 	alxy_check_x				# check x coordinates

		.align 4

alxy_not_neg_y:
		movl	_buf_y, %ebx				# end if y>=buf_y
        cmpl    %ebx, %eax
		jae 	alxy_end
		subl	$33, %ebx					# check y>buf_y-33
        cmpl    %ebx, %eax
		jbe 	alxy_not_overflow_y
		movl	_buf_y, %ecx				# ecx=buf_y-y
        subl    %eax, %ecx
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		movl	$(32*33), %edi				# edi=32*33
		jmp 	alxy_check_x				# check x coordinates

		.align 4

alxy_not_overflow_y:						# not at boundary
		movl	$33, %ecx					# ecx=33
		imull	%edx, %eax					# esi=buf_x*y
        movl    %eax, %esi
		movl	$(32*33), %edi				# edi=32*33

alxy_check_x:
		movl	8(%ebp), %eax				# ax=x

		orl 	%eax, %eax					# check x<0
		jns 	alxy_not_neg_x
		cmpl	$-33, %eax					# end if x<=-33
		jbe alxy_end
		movl	$33, %ebx					# ebx=33-abs(x)
        addl    %eax, %ebx
		addl	$32, %eax					# edi+=32-abs(x)
        addl    %eax, %edi
		jmp 	alxy_make_pointer

		.align 4

alxy_not_neg_x:
		movl	%edx, %ebx					# end if x>=buf_x
        cmpl    %ebx, %eax
		jae 	alxy_end
		subl	$33, %ebx					# check x>buf_x-33
        cmpl    %ebx, %eax
		jbe 	alxy_not_overflow_x
		movl	%edx, %ebx					# bx=buf_x-x
        subl    %eax, %ebx
		addl	%eax, %esi					# esi+=x
		addl	$32, %edi					# edi+=32
		jmp 	alxy_make_pointer

		.align 4

alxy_not_overflow_x:						# not at boundary
		movl	$33, %ebx					# bx=33
		addl	%eax, %esi					# esi+=x
		addl	$32, %edi					# edi+=32

alxy_make_pointer:
		subl	%ebx, %edx					# buf_x-ebx in edx

		addl	_scr, %esi					# point esi to screen buffer - 1
        decl    %esi

		addl	16(%ebp), %edi				# point edi to layer

		# esi=pointer to buffer,
        # edi=pointer to layer,
        # ebx=number of columns to write,
        # ecx=number of rows to write.
        # edx=buf_x-ebx

		std 								# scan backwards

		movb	$0xff, %al					# mask out code in al

		movl	$33, %ebp					# ebp=33-ebx
		subl	%ebx, %ebp

		.align 4, 0x90

alxy_y_loop:								# add layer
		pushl	%ecx
		movl	%ebx, %ecx

		.align 4, 0x90

alxy_scan_pixels:							# search for next byte != 0ffh
		addl	%ecx, %esi
		repe
		scasb
		je		alxy_end_loop
		subl	%ecx, %esi

		movb	1(%edi), %ah				# copy
		movb	%ah, (%esi)

		orw 	%ecx, %ecx					# loop
		jnz 	alxy_scan_pixels

alxy_end_loop:
		addl	%edx, %esi
		subl	%ebp, %edi

		popl	%ecx
		loop	alxy_y_loop

alxy_end:
		cld 								# direction down again

		popl	%edi
		popl	%esi
		popl	%ebx
		popl	%ebp

alxy_abort:
		ret


