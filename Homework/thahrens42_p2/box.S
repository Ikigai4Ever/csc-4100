//
//	Name: Ty Ahrens
//	Date: 2/12/2026
//	
//	box.S - version 0.1.1
//

.global     box
box:
 
    // standard entry: save link and stack frame registers
    //    load the stack frame with the pointer to current frame
    stp     x29, x30, [sp, -16]!

    // Save the arguments.  Called functions may overwrite x0-x3
    sub     sp, sp, 32
    stp     x0, x1, [sp]
    stp     x2, x3, [sp, 16]

    // sr in w0, sc in w1, er int w2, ec in w3
    
    // Make sure the argument are valid

    bl      _box_check_box_
    cmp     w0, 0
    bne     _box_error_

 
    // first, draw the corners:
    // +        +
    // +        +
    ldp     x0, x1, [sp]
    ldp     x2, x3, [sp, 16]
    bl      _box_draw_corners_

    // Now, draw the top and bottom:
    // +------+
    // +------+

    // You figure out what goes here
    ldp		x0, x1, [sp]
    ldp		x2, x3, [sp, 16]
    bl		_box_draw_top_bottom_

    // draw left and right sides
    // +------+
    // |	  |
    // +------+

    // You figure out what goes here
    ldp		x0, x1, [sp]
    ldp		x2, x3, [sp, 16]
    bl		_box_draw_right_left_

    mov     w0, 0
    b       _box_exit_


    
//------------------------------------------------------//

_box_check_box_:
	// sr from w0 to w19
	// sc from w1 to w20
	// er from w2 to w21
	// ec from w3 to w22
	
	sub		sp, sp, 48
	str		x30, [sp]
	stp		x19, x20, [sp, 16]
	stp		x21, x22, [sp, 32]
	
	//	save values in w0-w3 because of calls to other functions		
	mov		w19, w0		//	sr
	mov		w20, w1		//	sc
	mov		w21, w2		//	er
	mov		w22, w3 	//	ec		
	
	bl		term_txtheight		//	w0 now holds max_height
	
	//	er >= mr
	cmp		w21, w0
	bge		_check_fail_
	
	bl		term_txtwidth		//	w0 now holds max_width
	
	//	ec >= mc
	cmp		w22, w0
	bge		_check_fail_
	
	//	sr >= er
	cmp		w19, w21
	bge		_check_fail_
	
	//	sc >= ec
	cmp		w20, w22
	bge		_check_fail_ 
	
	mov w0, 0		//	return value of 0 for success
	
	//	cleanup
	ldr		x30, [sp]
	ldp		x21, x22, [sp, 32]
	ldp		x19, x20, [sp, 16]	
	add		sp, sp, 48
	ret 
	
_check_fail_:
	mov w0, 1		//	return value of 1 for failure
	ldr		x30, [sp]
	ldp		x21, x22, [sp, 32]
	ldp		x19, x20, [sp, 16]	
	add		sp, sp, 48
	ret 
	
//------------------------------------------------------//

_box_draw_corners_:	
	// sr from w0 to w19
	// sc from w1 to w20
	// er from w2 to w21
	// ec from w3 to w22
	
	sub		sp, sp, 48
	str		x30, [sp]
	stp		x19, x20, [sp, 16]
	stp		x21, x22, [sp, 32]
	
	//	save values in w0-w3 because of calls to other functions		
	mov		w19, w0		//	sr
	mov		w20, w1		//	sc
	mov		w21, w2		//	er
	mov		w22, w3 	//	ec
	
	//	top left '+' 
	//	pass w0 = sr, w1 = sc, w2 = +
	mov		w0, w19
	mov		w1, w20
	mov 	w2, '+'
	bl		putc_to
	
	//	top right '+'
	//	pass w0 = sr, w1 = ec, w2 = +
	mov		w0, w19
	mov		w1, w22
	mov		w2, '+'
	bl		putc_to
	
	//	bottom left '+'
	//	pass w0 = er, w1 = sc, w2 = +
	mov		w0, w21
	mov		w1, w20
	mov		w2, '+'
	bl		putc_to
	
	//	bottom right '+'		
	//	pass w0 = er, w1 = ec, w2 = +
	mov		w0, w21
	mov		w1, w22
	mov		w2, '+'
	bl		putc_to
		
	//	cleanup
	ldr		x30, [sp]
	ldp		x21, x22, [sp, 32]
	ldp		x19, x20, [sp, 16]	
	add		sp, sp, 48
	ret 

//------------------------------------------------------//

_box_draw_top_bottom_:
	// sr from w0 to w19
	// sc from w1 to w20
	// er from w2 to w21
	// ec from w3 to w22
	
	sub		sp, sp, 48
	str		x30, [sp]
	stp		x19, x20, [sp, 16]
	stp		x21, x22, [sp, 32]
	
	//	save values in w0-w3 because of calls to other functions		
	mov		w19, w0		//	sr
	mov		w20, w1		//	sc
	mov		w21, w2		//	er
	mov		w22, w3 	//	ec
	
	add		w9, w20, 1		//	w9 is starting column of top row
	
_top_loop_:
	//	check if current position equals the top right corner
	cmp 	w9, w22
	bge		_top_loop_done_
	
	//	draw line in current column
	mov		w0, w19
	mov		w1, w9
	mov		w2, '-'
	bl		putc_to
	
	add		w9, w9, 1		//	increment current column by 1
	b		_top_loop_		//	loop again
	
_top_loop_done_:
	add		w9, w20, 1		//	reset column to starting position
	
_bottom_loop_:
	cmp		w9, w22
	bge		_bottom_loop_done_
	
	//	draw line in current column
	mov		w0, w21
	mov		w1, w9
	mov		w2, '-'
	bl		putc_to
	
	add		w9, w9, 1			//	increment current column by 1
	b		_bottom_loop_		//	loop again
	
	
_bottom_loop_done_:
	//	cleanup
	ldr		x30, [sp]
	ldp		x21, x22, [sp, 32]
	ldp		x19, x20, [sp, 16]	
	add		sp, sp, 48
	ret 

//------------------------------------------------------//
_box_draw_right_left_:
	// sr from w0 to w19
	// sc from w1 to w20
	// er from w2 to w21
	// ec from w3 to w22
	
	sub		sp, sp, 48
	str		x30, [sp]
	stp		x19, x20, [sp, 16]
	stp		x21, x22, [sp, 32]
	
	//	save values in w0-w3 because of calls to other functions		
	mov		w19, w0		//	sr
	mov		w20, w1		//	sc
	mov		w21, w2		//	er
	mov		w22, w3 	//	ec

	add		w9, w19, 1		//	add 1 to starting row
				
_left_loop_:
	cmp		w9, w21
	bge		_left_loop_done_
	
	mov		w0, w9
	mov		w1, w20
	mov		w2, '|'
	bl		putc_to
	
	add		w9, w9, 1		//	increment row number by 1
	b		_left_loop_

_left_loop_done_:
	add		w9, w19, 1		//	reset w9 to starting row + 1

_right_loop_:
	cmp		w9, w21
	bge		_right_loop_done_
	
	mov		w0, w9
	mov		w1, w22
	mov		w2, '|'
	bl		putc_to
	
	add		w9, w9, 1		//	increment row number by 1
	b		_right_loop_


_right_loop_done_:
	//	cleanup
	ldr		x30, [sp]
	ldp		x21, x22, [sp, 32]
	ldp		x19, x20, [sp, 16]	
	add		sp, sp, 48
	ret 

//------------------------------------------------------//

_box_error_:
    mov     w0, 1

_box_exit_:
    add     sp, sp, 32   // pop save arg regs

    ldp     x29, x30, [sp], 16
    ret
