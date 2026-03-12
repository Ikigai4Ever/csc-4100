/*
 *  Name: Ty Ahrens
 *  Date: 3/11/2026
 *
 *  libos.h - version 0.2.0
 */

#ifndef LIBOS_H
#define LIBOS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ---- Terminal Functions ---- */
int  term_txtheight();
int  term_txtwidth();
void print_to(unsigned int row, unsigned int col, const char *str);
void putc_to(unsigned int row, unsigned int col, const char c);
int  box(unsigned int srow, unsigned int scol,
         unsigned int erow, unsigned int ecol);

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
 	PCB_t *head;
 	PCB_t *tail;
 	
} PCB_Q_t;

/******************* GLOABALS *******************/
extern PCB_t    *Running;
extern PCB_Q_t   Ready_q;
extern uint32_t  next_pid;

/******************* FUNCTION PROTOYPES *******************/

uint64_t *alloc_stack();
PCB_t    *alloc_pcb();

void   q_init(PCB_Q_t *q);
void   q_enqueue(PCB_Q_t *q, PCB_t *pcb);
PCB_t *q_dequeue(PCB_Q_t *q);

int    spawn_process(int (*pfun)());
void   dispatch_select();
void   dispatch(void);
void   restore_context(void);

#endif
