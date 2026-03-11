	//	in main:
	//	call clear_scr with the start row, start column, end row, and end column
	//	call the box function with the start row, start column end row, and end column of the box
	//	call print_to with the row and column, and string to print

	// note: prototype of the following functions are:
	//   int box(unsigned int srow, unsigned int scol, unsigned 
	//           int erow, unsigned int ecol);
	//   int print_to(unsinged int row, unsigned int col, char *string);
	//   int clear_scr(int start_row, int start_col, int width, int height);
	// box() and clear_scr returns 1 on error
	// The values that you should pass for box and clear_scr are srow = 21, 
	//   scol = 49, erow = 27, ecol = 79
	// The values that you should pass for print_to are row = 24, 
	//   column= 59, and string = "Hello world"
	
/*
 *	Name: Ty Ahrens
 *	Date: 3/11/2026
 *	
 *	kernel.c - version 0.2.0
 *
 */
 
#include <stdio.h>
#include <string.h>
#include "libos.h"
#include <stdint.h>

/******************* DATA STRUCTURES *******************/

/*
 *	      Name: PCB_t
 *	   Purpose: Holds different parts that the OS needs to know about a process 
 *	  Elements: sp - pointer to the sp to save last running spot
 *				pid - process id 				
 *				*next - pointer to the PCB next in queue
 */
typedef struct PCB {
	uint64_t sp;
	uint32_t pid;						
	struct PCB *next;		
} PCB_t;

/*
 *	      Name: PCB_Q_t
 *	   Purpose: Linked queue of PCB points for stack 
 *	  Elements: *head - next process to run from stack
 *				*tail - last process added to stack
 */
 typedef struct {
 	PCB_Q_t *head;
 	PCB_Q_t *tail;
 	
} PCB_Q_t;

/******************* VARIABLES *******************/

PCB_t *Running;		//pointer to current running process
PCB_Q_t Ready_q;	//queue of process waiting to run
uint32_t next_pid = 1;

//clear_scr passes
const int SROW = 21;
const int SCOL = 49;
const int EROW = 27;
const int ECOL = 79;

//starting script print_to passes
const int ROW = 0;
const int COLUMN = 0;
const char str[20] = "Running Processes";


/******************* PROTOTYPES *******************/
int box(unsigned int srow, unsigned int scol, unsigned int erow, unsigned int ecol);

//queue.c
void q_init(PCB_Q_t *q);
void q_enqueue(PCB_Q_t *q, PCB_t *pcb);
PCB_t *q_dequeue(PCB_Q_t *q);

//process.c
int spawn_process(int (*pfun)());

//processes
int p1();
int p2();
int p3();
int p4();
/*
int print_to(unsigned int, unsigned int, char *);
int clear_scr(int, int, int, int); 
*/

/******************* FUNCTIONS *******************/

/*
 *	      Name: clear_scr
 *	   Purpose: clear the screen within the parameters sent 
 *	Parameters: start_row - starting row position
 *				start_col - starting column position
 *				width - ending row position
 *				height - ending column position
 */
int clear_scr(int start_row, int start_col, int width, int height){
	for(int i = start_row; i <= width; i++){
		for(int j = start_col; j <= height; j++){
			putc_to(i, j, ' ');
		}
	}
	return 0;
}

/*
 *	      Name: q_init
 *	   Purpose: initialize the ready queue properly (head and tail to NULL)
 *	Parameters: q - pointer to Queue_t
 */
void q_init(PCB_Q_t *q){
	q->head = NULL;
	q->tail = NULL;
	
}

/*
 *	      Name: dispatch_select
 *	   Purpose: Choose what process is to run next based on pulling the head of queue
 *				and setting current running point 
 *	Parameters: n/a
 */
void dispatch_select(){
	Running = q_dequeue(&Ready_q);
}

/******************* MAIN *******************/

int main() {
	int retval;
	
    //clear the screen
 	clear_scr(SROW, SCOL, EROW, ECOL);   
    //print "Running processes" on the first line of the screen
	print_to(ROW, COLUMN, str);
	
	//call q_init(Ready_q)   // initialize Ready queue data structure
                           // The prototype for q_init is: void q_init(Queue_t *)
    q_init(&Ready_q);
    
	//set retval to the return value of spawn_process(p1)
    // The prototype of sqawn_process is: int spawn_process(int (*pfun)())
   	retval = spawn_process(p1);
   
 	//check if error (retval != 0)
 	if (retval != 0){
 		print_to(1, 0, "Error spawning p1");
 		return 1;
 	}
 	
    // repeat the above for process p2, p3, and p4
	
	// p2 spawn checking
	retval = spawn_process(p2);
	if (retval != 0){
 		print_to(2, 0, "Error spawning p2");
 		return 1;
 	}
 	
 	// p3 spawn checking
	retval = spawn_process(p3);
	if (retval != 0){
 		print_to(3, 0, "Error spawning p3");
 		return 1;
 	}
 	
 	// p4 spawn checking
	retval = spawn_process(p4);
	if (retval != 0){
 		print_to(4, 0, "Error spawning p4");
 		return 1;
 	}

    // Now begin running the first process ...
    dispatch_select();
  
	asm volatile("b restore_context");  // dispatch the current Running process
                                        //   This restores the context of the process
                                        //   at which Running points, and then jumps
                                        //   to its next instruction.
                                        // Note: retore_context is a label
                                        //       in assembler.  You must
                                        //       declare it as .global in
                                        //       the assembly file to use 
                                        //       it here.
    // Note that you never return here
}

