/*
 *	Name: Ty Ahrens
 *	Date: 4/19/2026
 *	
 *	process_asm.S - version 0.3.0
 *
 */
 
.global dispatch
.global restore_context
.global setup_EVT
.global init_timer
.global clear_timer
.global unsupported
.global vector_table

/*
 *	      Name: unsupported
 *	   Purpose: Handler for every exception type we do not support.
 *	Parameters: n/a
 */
unsupported:
	/* print_to(row=0, col=0, str) uses x0=row, x1=col, x2=str */
    adr     x2, unsupported_str     
    mov     x1, #0                  
    mov     x0, #0                  
    bl      print_to                // call libos print function
 
unsupported_loop:
    b       unsupported_loop        // spin here forever
  
unsupported_str:
    .asciz  "Error: unsupported exception!"
.balign 4
 	
/*
 *	      Name: setup_EVT
 *	   Purpose: Tell the hardware about the exception vector table.
 *	Parameters: n/a
 */
setup_EVT:
	// set up the exception vector table
    adr     x0, vector_table        
    msr     VBAR_EL1, x0        // Set the vector base address        
    ret

/*
 *	      Name: clear_timer
 *	   Purpose: Reset the time to so that it will fire in 1/20 of a second.
 *	Parameters: n/a
 */
clear_timer:
	// read count frequency
    mrs     x0, CNTFRQ_EL0
    mov     x1, #20
    udiv    x0, x0, x1              
    
    // clear cntv interrupt and set next 1 sec timer.
    msr     CNTV_TVAL_EL0, x0
    ret
    
/*
 *	      Name: init_timer
 *	   Purpose: Initialize the timer.
 *	Parameters: n/a
 */
init_timer:
    stp     x29, x30, [sp, #-16]!   
    bl      clear_timer             
 
    //route core 0 counter to core 0 irq
    ldr     x0, =0x40000040         
    mov     x1, #0x08               
    str     w1, [x0]
 
    // enable timer
    mov     x0, #1
    msr     CNTV_CTL_EL0, x0        // bit 0 = ENABLE
    ldp     x29, x30, [sp], #16     
    ret

/*
 *	      Name: vector_table
 *	   Purpose: ARMv8 requires VBAR to be aligned to 2 KB (2048 bytes).
 *  			Each of the 16 entries is exactly 128 bytes apart.
 *	Parameters: n/a
 */

.balign 2048	//2KB for VBAR

vector_table:
	.balign 128
		b unsupported
	.balign 128
		b dispatch
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b dispatch    // yes
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b dispatch
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b unsupported
	.balign 128
		b dispatch
	.balign 128
		b unsupported
	.balign 128
		b unsupported

/*
 *	      Name: dispatch
 *	   Purpose: Saves current process context, enqueues it, then branch to restore_context
 *				to run next process
 *	Parameters: n/a
 *		
 *      Stack layout:
 *      x0
 *      x2
 *      ...
 *      x28
 *      x29
 *      x30
 *      PSTATE
 *      Exception return address
 *
 */
dispatch:
	
	// Disable interrupts 
	msr     DAIFSet, #0x2
	
	// Save x1 Temporarily so we can use it to reference the process stack
	str		x1, [sp, #-16]!
	
	// Load the process stack pointer
	mrs		x1, SP_EL0
	
  	// Now, X1 has the process stack pointer
  	// Make room for the process context on the process stack by subtracting 16*17 from x1
  	sub		x1, x1, #(16*17)

	// Now Save the proces general purpose registers
	str		x0, [x1, #0]

	// Store the process's x0 and x1 onto process stack
	ldr		x0, [sp], #16
	str		x0, [x1, #8]
	
	// Store the register x2...x30 on the stack
	stp     x2,  x3,  [x1, #16]
    stp     x4,  x5,  [x1, #32]
    stp     x6,  x7,  [x1, #48]
    stp     x8,  x9,  [x1, #64]
    stp     x10, x11, [x1, #80]
    stp     x12, x13, [x1, #96]
    stp     x14, x15, [x1, #112]
    stp     x16, x17, [x1, #128]
    stp     x18, x19, [x1, #144]
    stp     x20, x21, [x1, #160]
    stp     x22, x23, [x1, #176]
    stp     x24, x25, [x1, #192]
    stp     x26, x27, [x1, #208]
    stp     x28, x29, [x1, #224]    // x28 at [x1,224], x29 at [x1,232]
    str     x30,      [x1, #240]    // x30 at [x1, 16*15]
	
	// Save the process PSTATE
	mrs     x0, SPSR_EL1
    str     x0, [x1, #248]          // [x1, 16*15+8]
	
	// Save the interrupt return address
	mrs     x0, ELR_EL1
    str     x0, [x1, #256]          // [x1, 16*16]
	
	// Save the value of x1 into the process's PCB
	adr     x0, Running             // x0 = address of "Running" global
    ldr     x0, [x0]                // x0 = pointer to the Running PCB
    str     x1, [x0]                // Running->sp = x1 (our new frame top)
	
	// Enqueue the current process to the ready Q (call enqeue)
	adr     x0, Ready_q             // x0 = &Ready_q  (first argument)
    adr     x2, Running
    ldr     x1, [x2]                // x1 = Running   (second argument)
    bl      q_enqueue
    
    b restore_context
    
    
/*
 *	      Name: restore_context
 *	   Purpose:  Dequeue the next process to run and set global "Running" to its PCB pointer
 *				 Get the stack pointer from the PCB (at which "Running" points) 
 *				 and put it in x1
 *	Parameters: n/a
 *
 */
restore_context:
	bl 		dispatch_select			// select the next process
	
	adr		x1, Running
	ldr 	x1, [x1]
	ldr 	x1, [x1]

 	// Restore the PSTATE
	ldr     x0, [x1, #248]          // [x1, 16*15+8]
    msr     SPSR_EL1, x0
    
	// Restore interrupt return address
	ldr     x0, [x1, #256]          // [x1, 16*16]
    msr     ELR_EL1, x0
	
	// Store the new top of stack into the System exception register for EL0
	mov     x0, x1
    add     x0, x0, #(16*17)        // x0 = x1 + 272 = original SP_EL0
    msr     SP_EL0, x0
	
	// Restore process registers x2...x29
	ldp     x2,  x3,  [x1, #16]
    ldp     x4,  x5,  [x1, #32]
    ldp     x6,  x7,  [x1, #48]
    ldp     x8,  x9,  [x1, #64]
    ldp     x10, x11, [x1, #80]
    ldp     x12, x13, [x1, #96]
    ldp     x14, x15, [x1, #112]
    ldp     x16, x17, [x1, #128]
    ldp     x18, x19, [x1, #144]
    ldp     x20, x21, [x1, #160]
    ldp     x22, x23, [x1, #176]
    ldp     x24, x25, [x1, #192]
    ldp     x26, x27, [x1, #208]
    ldr     x28,      [x1, #224]    // restore x28 only; skip x29 at [x1,232]
	
	// clear counter and reset the timer.
	stp     x0, x1, [sp, #-16]!     // save temps x0, x1 on OS stack
    bl      clear_timer             // reset countdown; returns via x30
    ldp     x0, x1, [sp], #16       // restore: x0 = junk, x1 = frame ptr
    
    // Restore x29 
    ldr     x29, [x1, #232]         // [x1, 16*14+8]
    ldr     x30, [x1, #240]         // [x1, 16*15]
    
    // Restore x0 and x1
    ldr     x0, [x1, #0]
    ldr     x1, [x1, #8]
    
	// Re-enable interrupts
	msr     DAIFClr, #0x2
	
	// Return to EL0
	eret
	
	





