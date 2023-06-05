#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define FLAGS                      	0x202				
#define STACK_SEGMENT              	0x0				
#define CODE_SEGMENT               	0x8
#define MAX_PROCESSES              	100
#define MAX_CHILDREN_PROCESSES 50
#define NO_PROCESS_FOUND           -1
#define TRUE                       	1
#define FALSE                      	0
#define DEAD                       	0
#define BLOCK                      	1
#define READY                      	2
#define RUNNING                    	3
#define PAUSED						4	
#define PAUSED_BY_CHILD				5
#define PAUSED_BY_SEM 			    6
#define NO_SPACE_FOR_PROCESS       -2
#define PROCESSES_LIMIT_REACHED    -3
#define CANT_KILL_IMMORTAL_PROCESS -4
#define STACK_SIZE                 4096 // 4KB

#define STDIN 0
#define STDOUT 1
#define BACKGROUND 4
	

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


typedef struct process_info {
	char * name;
	uint32_t pid;
	uint8_t priority;
	uint8_t state;
	void * stack_start;
	void * stack_end;
	uint64_t stack_pointer;
	uint8_t output;
	uint32_t assigned_ticks;
} process_info;

uint32_t calculate_ticks(uint8_t priority);

void scheduler_init();

void change_process_state(uint32_t pid, uint8_t new_state);

int32_t create_process(char **params, uint8_t priority, uint8_t input, uint8_t output, uint8_t immortal, uint64_t entry_point);

void idle_process();

uint32_t get_pid();

uint8_t get_process_idx(uint32_t pid);

int8_t change_priority(uint32_t pid, uint8_t priority);

uint8_t get_state(uint32_t pid);

int32_t block_or_unblock(uint32_t pid);

void end_process();

void destroy_process(uint8_t idx);

void free_params(char **params);

int32_t kill_process(uint32_t pid);

int32_t create_child_process(char **params, uint8_t priority, uint8_t input, uint8_t output, uint64_t entry_point);

void set_dead_process(uint32_t pid);

void wait_for_children();

extern void force_timer_tick();

uint8_t get_all_processes(process_info * processes);

#endif // SCHEDULER_H