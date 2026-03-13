/*
 *  Name: Ty Ahrens
 *  Date: 3/13/2026
 *
 *  alloc.c - Version 0.2.0
 */

#include "libos.h"

#define NUMSTACKS 15

// Global arrays
uint64_t stacks[NUMSTACKS][1024];   // 15 stacks, each 1024 uint64_t (8 bytes each)
PCB_t    pcbs[NUMSTACKS];           // 15 PCBs

// Trackers
int next_stack = 0;
int next_pcb   = 0;

/*
 *        Name: alloc_stack
 *     Purpose: Returns a pointer to the next available stack
 *  Parameters: n/a
 */
uint64_t *alloc_stack() {
    if (next_stack >= NUMSTACKS) return 0;
    return stacks[next_stack++];
}

/*
 *        Name: alloc_pcb
 *     Purpose: Returns a pointer to the next available PCB
 *  Parameters: n/a
 */
PCB_t *alloc_pcb() {
    if (next_pcb >= NUMSTACKS) return 0;
    return &pcbs[next_pcb++];
}

