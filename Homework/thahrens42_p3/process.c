/*
 *	Name: Ty Ahrens
 *	Date: 4/19/2026
 *	
 *	process.c - version 0.3.0
 *
 */
 
#include "libos.h"

/******************* FUNCTIONS *******************/

/*
 *	      Name: spawn_process
 *	   Purpose: Allocate a stack and PCB, set up initial contaxt, and enqueueing a new process 
 *	  Parameters: code_address - pointer to function of process to run
 *				
 */
int spawn_process(int (*code_address)()){
// code_address is the pointer to the function that contains the process code
		
      
    //set stackptr to return value of calling alloc_stack() to allocate a stack
    //      that is 1024 uint64_t in size
    uint64_t *stackptr = alloc_stack();
    
    //check to make sure stackptr is not null
  	//	(if it is, return -1)
  	if (stackptr == NULL){
  		return -1;
  	}
   	

    // First, set up the stack, pretend sp is a uint64_t *
	
    //set sp to stackptr + 1024 to put sp at the
    //     bottom of the stack    // Note that if stackptr is a pointer to a
                                  // uint64_t, then stackptr+1024 will point
                                  // to the right place
	uint64_t *sp = stackptr + 1024;	

    // create context that will set gen purpose regs to 0
    // loop i from 0 to 32 (inclusive)
    //   decrement sp  // sp should be a pointer to a uint64_t
  	//	set the uint64_t at which sp points to 0
  	for (int i = 0; i <= 32; i++){
  		sp--;		
  		*sp = 0;
  	}

	//set the value at which sp+30 points to code_address // type cast code_address  
                                                          // to a uint64_t so 
                                                          // that the compiler does not complain)
    *(sp + 32) = (uint64_t)code_address;
    
    // Note: you will have to create the pcb struct for the following code
    //set pcb to the return value of alloc_pcb() to allocate a pcb for the 
  	//	process
	PCB_t *pcb = alloc_pcb();
	
	//check the pcb and if is null then return -1
	if (pcb == NULL){
		return -1;
	}
	
    //set the pcb's sp member to sp.
    //set the pcb's pid member to next_pid  //Note that you can keep track
                                            // of the next pid using a
                                            // global variable
    pcb->sp = (uint64_t)sp;
    pcb->pid = next_pid++;

    //enqueue the pcb onto the Ready_q  // Ready_q should be a global
	q_enqueue(&Ready_q, pcb);

    return 0; /* no errors occured */
    // Note that if alloc_stack() or alloc_pcb() returns NULL, you should return 
    //  -1 instead to indicate the error
}
