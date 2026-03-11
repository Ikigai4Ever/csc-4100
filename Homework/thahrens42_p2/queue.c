/*
 *	Name: Ty Ahrens
 *	Date: 3/11/2026
 *	
 *	queue.c - version 0.2.0
 *
 */
 
#include <stdint.h>
#include "libos.h"

/******************* FUNCTIONS *******************/

/*
 *	      Name: q_init
 *	   Purpose: initialize the ready queue properly (head and tail to NULL)
 *	Parameters: q - pointer to the queue
 */
void q_init(PCB_Q_t *q){
	q->head = NULL;
	q->tail = NULL;
	
}

/*
 *	      Name: q_enqueue
 *	   Purpose: Add a PCB to the tail of ready queue
 *	Parameters: q - pointer to the queue
 *				pcb - pointer to the PCB in order to add
 */
void q_enqueue(PCB_Q_t *q, PCB_t *pcb){
	pcb->next = NULL;
	
	if (q->tail == NULL){
		q->head = pcb;
		q->tail = pcb;
	}
	else{
		q->tail->next = pcb;	// old tail -> new PCB
		q->tail = pcb;			// update tail to new PCB
	}
}

/*
 *	      Name: q_deqeue
 *	   Purpose: Remove and return the PCB at the head of ready queue
 *	Parameters: q - pointer to the queue
 */
PCB_t *q_dequeue(PCB_Q_t *q){
	
	//nothing to dequeue
	if (q->head == NULL){
		return NULL;
	}	
		
	PCB_t *pcb = q->head;
	q->head = q->head->next;	//advance to next PCB
	
	//check to see if queue is empty
	if (q->head == NULL){
		q->tail = NULL;
	}
	
	pcb->next = NULL;
	return pcb;
	
		

}


