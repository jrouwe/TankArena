# The keyboard handler used in Tank Arena. The Allegro handler is equally
# good, but it takes too much effort to port the whole thing, so I leave
# it like this.


        .extern __djgpp_base_address
        .extern __djgpp_ds_alias

        .global _init_keyboard
        .global _deinit_keyboard
        .global _scan_key
        .global _wait_key
	.global _clear_key_buffer

        .text
        
        .align 4

locking_region_start:

_keyboard_buffer:    .space 0x40, 0
_keyboard_start:     .long 0
_keyboard_end:	     .long 0
extended:            .byte 0

        .align 4            

handler:
        cli
        pushl    %eax
        pushl    %ebx
        pushl    %edx
        pushw    %ds

        .byte 0x2e                        # setup ds
        movw    ___djgpp_ds_alias, %ax
        movw    %ax, %ds

	xorb	%ah, %ah		  # get/reset extended
        xchgb   %ah, extended
        
        inb     $0x60, %al                # get byte from keyboard

        cmpb    $0xe0, %ah                # check if key is complete
        je        not_extended_code
        cmpb    $0xe0, %al
        jne     not_extended_code

	movb	%al, extended		  # set extended byte
        jmp     acknowledge_keyboard

not_extended_code:                        # this code is sent before
	cmpw	$0xe02a, %ax		  # any arrow key if numlock is on
	je	acknowledge_keyboard	  # so we filter it out

	movl	_keyboard_end, %ebx	  # save the key
        movw    %ax, _keyboard_buffer(,%ebx,2)
        incl    %ebx
        andl    $0x1f, %ebx

	cmpl	%ebx, _keyboard_start	  # don't save the key if the buffer
	je	acknowledge_keyboard	  # is full

        movl    %ebx, _keyboard_end

acknowledge_keyboard:
        popw    %ds
        popl    %edx
        popl    %ebx

        inb     $0x61, %al                # acknowledge kbd
        orb     $0x80, %al
        outb    %al, $0x61
        andb    $0x7f, %al
        outb    %al, $0x61

        movb    $0x20, %al                # acknowledge PIC
        outb    %al, $0x20
        popl    %eax
        sti
        iret
        
locking_region_end:
             
prev_handler_ofs:    .long 0
prev_handler_sel:    .word 0

        .align 4

###############################################################################
# long init_keyboard(void);
###############################################################################
_init_keyboard:
        pushl    %ebx
        pushl    %esi
        pushl    %edi
        
	movl	$0, _keyboard_start	  # clear counters
        movl    $0, _keyboard_end

        leal    locking_region_start, %ecx
        leal    locking_region_end, %edi
        subl    %ecx, %edi
        addl    ___djgpp_base_address, %ecx

	shldl	 $16, %ecx, %ebx	  # ecx -> bx:cx
	shldl	 $16, %edi, %esi	  # edi -> si:di

        movw    $0x0600, %ax
	int	$0x31			  # lock linear region
	jc	init_error

        movw    $0x0204, %ax
        movb    $0x09, %bl
	int	$0x31			  # get pm int vector
        movw    %cx, prev_handler_sel
        movl    %edx, prev_handler_ofs

        movb    $0x09, %bl
        movw    %cs, %cx
        leal    handler, %edx
        movw    $0x0205, %ax
	int	$0x31			  # set pm int vector

init_error:
	cmc				  # eax = -1 if CF is 0
	movl	$0, %eax
        sbbl    $0, %eax
        
        popl    %edi
        popl    %esi
        popl    %ebx
        ret
        
        .align 4

###############################################################################
# void deinit_keyboard(void);
###############################################################################
_deinit_keyboard:
        pushl    %ebx
        pushl    %esi
        pushl    %edi

	cmpw	$0, prev_handler_sel	  # check if keyboard handler was
	je	dont_deinit_keyboard	  # installed
        
        leal    locking_region_start, %ecx
        leal    locking_region_end, %edi
        subl    %ecx, %edi
        addl    ___djgpp_base_address, %ecx
	shldl	$16, %ecx, %ebx
	shldl	$16, %edi, %esi
        movw    $0x0601, %ax
	int	$0x31			  # unlock linear region

        movw    prev_handler_sel, %cx
        movl    prev_handler_ofs, %edx
        movb    $0x09, %bl
	movw	$0x0205, %ax		  # set pm int vector
        int     $0x31

	movw	$0, %ax 		  # indicate keyboard handler
	movw	%ax, prev_handler_sel	  # not installed

dont_deinit_keyboard:
        popl    %edi
        popl    %esi
        popl    %ebx
        ret

        .align 4

###############################################################################
# unsigned scan_key(void);
###############################################################################
_scan_key:
        pushl    %esi
        pushl    %edi

	movl	$0x0000ffff, %eax	  # put error code in eax

	movl	_keyboard_start, %esi	  # check for a key
        movl    _keyboard_end, %edi
        cmpl    %esi, %edi
	je	no_key_in_buffer

        movw    _keyboard_buffer(,%esi,2), %ax    # get it

	incl	%esi			  # remove key from buffer
        andl    $0x1f, %esi
        movl    %esi, _keyboard_start

no_key_in_buffer:
        popl    %edi
        popl    %esi
        ret

        .align 4

###############################################################################
# unsigned wait_key(void);
###############################################################################
_wait_key:
        call    _scan_key                # just scan until a key is
        testw    $0x0080, %ax            # pressed (bit 7 will be 0)
        jnz     _wait_key
        ret

###############################################################################
# void clear_key_buffer(void);
###############################################################################
_clear_key_buffer:
	movl	$0, %eax		 # remove all keys from buffer
	movl	%eax, _keyboard_start
	movl	%eax, _keyboard_end
	ret

