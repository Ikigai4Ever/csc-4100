/*
 *	Name: Ty Ahrens
 *	Date: 4/19/2026
 *	
 *	kernel.c - version 0.3.0
 *
 */
 
#include "libos.h"


/******************* VARIABLES *******************/

PCB_t    *Running;
PCB_Q_t   Ready_q;
uint32_t  next_pid = 1;

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
//processes
int p1();
int p2();
int p3();
int p4();

//assembly routines
void setup_EVT(void);
void init_timer(void);

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
 *	      Name: dispatch_select
 *	   Purpose: Choose what process is to run next based on pulling the head of queue
 *				and setting current running point 
 *	Parameters: n/a
 */
void dispatch_select(){
	Running = q_dequeue(&Ready_q);
	
	if (Running == NULL){
		print_to(0, 0, "Error: Ready_q is empty!");
	}
}

/*
 *        Name: is_prime
 *     Purpose: Brute force prime checker using division up to half the number
 *  Parameters: num - number to check
 */
int is_prime(unsigned long long num) {
    if (num < 2) 
    	return 0;
    for (unsigned long long i = 2; i <= num / 2; i++) {
        /* 	count++;
       		if (count >= 1500){
        		dispatch();       // yield CPU during loop so other processes can run
        		count = 0;
        	}
        */
        if (num % i == 0) 
        	return 0;
    }
    return 1;
}

/*
 *	      Name: p1
 *	   Purpose: Counts primes, then displays count in box at row 9-11, col 23-29
 *				and setting current running point 
 *	Parameters: n/a
 */
int p1(){
	char message[] = "Process 1: 0";
	unsigned long long num = 1;
	int count = 0;
	
	box(9,23,11,39);
	print_to(10,25,message);
	
	while(1){
		if (is_prime(num)){
		    count++;
		    char countc = count + '0';  // convert count to character
		    message[11] = countc;
		}
		print_to(10, 25, message);       // print every iteration
		num++;
		if (count > 9)            
			count = 0;
		if (num   > 4000000000ULL) 
			num  = 1;
    }
    return 0;
}

/*
 *	      Name: p2
 *	   Purpose: Counts primes, then displays count in box at row 13-15, col 23-29
 *				and setting current running point 
 *	Parameters: n/a
 */
int p2(){
	char message[] = "Process 2: 0";
	unsigned long long num = 1;
	int count = 0;
	
	box(13, 23, 15, 39);
	print_to(14, 25, message);
	
	while(1){
		if (is_prime(num)){
		    count++;
		    char countc = count + '0';  
		    message[11] = countc;
		}
		print_to(14, 25, message);       
		num++;
		if (count > 9)            
			count = 0;
		if (num   > 4000000000ULL) 
			num  = 1;
    }
    return 0;
}

/*
 *	      Name: p3
 *	   Purpose: Counts primes, then displays count in box at row 9-11, col 49-65
 *				and setting current running point 
 *	Parameters: n/a
 */
int p3(){
	char message[] = "Process 3: 0";
	unsigned long long num = 1;
	int count = 0;
	
	box(9, 49, 11, 65);
	print_to(10, 51, message);
	
	while(1){
		if (is_prime(num)){
		    count++;
		    char countc = count + '0';  
		    message[11] = countc;
		}
		print_to(10, 51, message);       
		num++;
		if (count > 9) 
			count = 0;
		if (num   > 4000000000ULL) 
			num  = 1;
    }
    return 0;
}

/*
 *	      Name: p4
 *	   Purpose: Counts primes, then displays count in box at row 13-15, col 49-65
 *				and setting current running point 
 *	Parameters: n/a
 */
int p4(){
	char message[] = "Process 4: 0";
	unsigned long long num = 1;
	int count = 0;
	
	box(13, 49, 15, 65);
	print_to(14, 51, message);
	
	while(1){
		if (is_prime(num)){
		    count++;
		    char countc = count + '0';  
		    message[11] = countc;
		}
		print_to(14, 51, message);       
		num++;
		if (count > 9)            
			count = 0;
		if (num   > 4000000000ULL) 
			num  = 1;
    }
    return 0;
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
 	
 	setup_EVT();
 	init_timer();

    // Now begin running the first process ...  
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

