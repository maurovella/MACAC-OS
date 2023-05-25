#ifndef SCHEDULER_H
#define SCHEDULER_H

#define FLAGS                0x202				
#define STACK_SEGMENT        0x0				
#define CODE_SEGMENT         0x8
#define MAX_PROCESSES        20
#define NO_PROCESS_FOUND     -1
#define TRUE                 1
#define DEAD                 0
#define BLOCK                1
#define READY                2
#define PAUSED               3
#define NO_SPACE_FOR_PROCESS -2
#define PROCESSES_LIMIT_REACHED    -3
#define STACK_SIZE           4096 // 4KB

// Stack structure 
#define STACK_POINT_OF_ENTRY (21*8)   	/*  	|	RAX, RBX  |		*/
										/*  	|	RCX, etc  |		*/   
#define RDI_POS   (12*8)				/*		---------------		*/ 
#define IP_POS    (6*8)					/*  	|	  RIP	  |		*/				
#define CS_POS 	  (5*8)					/*  	|	  CS	  |		*/
#define FLAGS_POS (4*8)					/*  	|	  RFLAGS  |		*/
#define SP_POS 	  (3*8)					/*  	|	  RSP	  |		*/
#define SS_POS	  (2*8)					/*  	|	  SS	  |		*/
#define RET_POS   8 					/*  	|	  RET	  |		*/
#define ALINGMENT 8		 				/*   	 -------------		*/

#define STACK_POSITION(stack_start, pos) (uint64_t *) ((stack_start) - (pos))
#define CALCULATE_TICKS(priority) (priority) * 2

void idle_process();

uint8_t get_pid();

uint8_t get_process_idx(uint8_t pid);

uint8_t change_priority(uint8_t pid, uint8_t priority);

uint8_t get_state(uint8_t pid);

uint8_t block_or_unblock(uint8_t pid);

#endif // SCHEDULER_H