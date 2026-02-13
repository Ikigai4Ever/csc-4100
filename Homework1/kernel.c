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
 *	Date: 2/12/2026
 *	
 *	kernel.c - version 0.1.2
 *
 */
 
#include <stdio.h>
#include <string.h>
#include "libos.h"

/******************* VARIABLES *******************/

//clear_scr passes
const int SROW = 21;
const int SCOL = 49;
const int EROW = 27;
const int ECOL = 79;

//print_to passes
const int ROW = 24;
const int COLUMN = 59;
const char str[20] = "Hello World";

/******************* PROTOTYPES *******************/
int box(unsigned int srow, unsigned int scol, unsigned int, unsigned int);
/*
int print_to(unsigned int, unsigned int, char *);
int clear_scr(int, int, int, int); 
*/

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


int main(){
	clear_scr(SROW, SCOL, EROW, ECOL);
	box(SROW, SCOL, EROW, ECOL);
	print_to(ROW, COLUMN, str);
}





