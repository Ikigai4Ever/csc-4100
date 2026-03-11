/*
 *	Name: Ty Ahrens
 *	Date: 3/11/2026
 *	
 *	process_asm.S - version 0.2.0
 *
 */
 
.global dispatch
.global restore_context

/*
 *	      Name: dispatch
 *	   Purpose: Saves current process context, enqueues it, then falls to restore_context
 *				to run next process
 *	  Elements: n/a
 */
 dispatch:
 	//1)	subtract 16*16 from the stack
 	 sub     sp, sp, #16*16
 	
    //2)	copy all registers to the stack the stack
	//		(in the order x0,x1,...,x30)
	str     x0,  [sp, #16*0]
    str     x1,  [sp, #16*0+8]
    str     x2,  [sp, #16*1]
    str     x3,  [sp, #16*1+8]
    str     x4,  [sp, #16*2]
    str     x5,  [sp, #16*2+8]
    str     x6,  [sp, #16*3]
    str     x7,  [sp, #16*3+8]
    str     x8,  [sp, #16*4]
    str     x9,  [sp, #16*4+8]
    str     x10, [sp, #16*5]
    str     x11, [sp, #16*5+8]
    str     x12, [sp, #16*6]
    str     x13, [sp, #16*6+8]
    str     x14, [sp, #16*7]
    str     x15, [sp, #16*7+8]
    str     x16, [sp, #16*8]
    str     x17, [sp, #16*8+8]
    str     x18, [sp, #16*9]
    str     x19, [sp, #16*9+8]
    str     x20, [sp, #16*10]
    str     x21, [sp, #16*10+8]
    str     x22, [sp, #16*11]
    str     x23, [sp, #16*11+8]
    str     x24, [sp, #16*12]
    str     x25, [sp, #16*12+8]
    str     x26, [sp, #16*13]
    str     x27, [sp, #16*13+8]
    str     x28, [sp, #16*14]
    str     x29, [sp, #16*14+8]
    str     x30, [sp, #16*15]
	
    //3)	put the PSTATE into the stack with the
	//		following code:
	//		   mrs     x0, SPSR_EL1      // Save PSTATE
	//		   str     x0, [sp, 16*15+8]
	mrs     x0, SPSR_EL1
    str     x0, [sp, #16*15+8]
	
	//4)	store the new sp value into the pcb where Running points
	//		(note that you cannot move sp into memory directly,
    //        you have to do something like:
    //         adr     x1, Running
    //         ldr     x1, [x1]  // put the first value in the PCB (sp) into x1
    //		 mov     x4, sp    // We can't str sp directly, so put sp into x4...
    //		 str     x4, [x1]  //    Then store the new sp in x4 to the PCB 
    adr		x1, Running
    ldr		x1, [x1]
    mov		x4, sp
    str		x4, [x1]
    
	//5)	call enqueue passing it address of ReadyQ and the value of
	//		Running
	adr		x0, Ready_q
	adr		x1, Running		// get address of Running
	ldr		x1, [x1]		// load the address of Running in x1
	bl 		q_enqueue		
restore_context:   // Note that this is where the inline assembly in main jumps
    
    
    //6)	call dispatch_select()   // The function written in C/C++
    bl		dispatch_select
    
    //7)  	read the stack pointer value out of the pcb at which Running points and put
    //    		it into the sp register. The code should look
    //        	like the following:
    //        	adr		x1, Running
    //          str		x0, [x1]   // store the pointer to the PCB into the Running variable
    //			ldr		x2, [x0]   // x2 now has the value of the stack ptr (we cant ldr sp directly)
    //			mov		sp, x2     // sp now has the top of stack for the chosen process
    adr		x1, Running
    str		x0, [x1]
    ldr		x2, [x0]
    mov		sp, x2
    
    //8)	restore the PSTATE stored in the stack
    //      ldr     x0, [sp, 16*15+8]   // Restore PSTATE
    //		msr     SPSR_EL1, x0
    ldr		x0, [sp, #16*15+8]
	msr		SPSR_EL1, x0 		
    
    //9) copy all register values for x0-x30 from the stack at
    //        which sp points into those registers.
    ldr     x0,  [sp, #16*0]
    ldr     x1,  [sp, #16*0+8]
    ldr     x2,  [sp, #16*1]
    ldr     x3,  [sp, #16*1+8]
    ldr     x4,  [sp, #16*2]
    ldr     x5,  [sp, #16*2+8]
    ldr     x6,  [sp, #16*3]
    ldr     x7,  [sp, #16*3+8]
    ldr     x8,  [sp, #16*4]
    ldr     x9,  [sp, #16*4+8]
    ldr     x10, [sp, #16*5]
    ldr     x11, [sp, #16*5+8]
    ldr     x12, [sp, #16*6]
    ldr     x13, [sp, #16*6+8]
    ldr     x14, [sp, #16*7]
    ldr     x15, [sp, #16*7+8]
    ldr     x16, [sp, #16*8]
    ldr     x17, [sp, #16*8+8]
    ldr     x18, [sp, #16*9]
    ldr     x19, [sp, #16*9+8]
    ldr     x20, [sp, #16*10]
    ldr     x21, [sp, #16*10+8]
    ldr     x22, [sp, #16*11]
    ldr     x23, [sp, #16*11+8]
    ldr     x24, [sp, #16*12]
    ldr     x25, [sp, #16*12+8]
    ldr     x26, [sp, #16*13]
    ldr     x27, [sp, #16*13+8]
    ldr     x28, [sp, #16*14]
    ldr     x29, [sp, #16*14+8]
    ldr     x30, [sp, #16*15]

    //10) add 16*16 to sp
    add		sp, sp, #16*16
    
    //11) return
    ret

