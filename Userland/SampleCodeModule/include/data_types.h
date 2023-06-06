#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

#include <stdint.h>

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

#endif