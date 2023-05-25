#include <stdint.h>
#include "scheduler.h"
#include "interrupts.h"
#include "memory_manager.h"

typedef struct process {
    uint8_t pid;                    // process ID
    uint8_t priority;               // priority of the process (1-5)

    char **params;                  // parameters of the process
    
    void *stack_start;              // start of the stack of the process
    void *stack_end;                // end of the stack of the process
    void *stack_pointer;            // pointer to the stack of the process
    
    uint8_t state;                  // state of the process 
    uint32_t ticks;                 // asigned ticks to the process
    uint8_t immortal;               // indicates if the process is immortal (1) or not (0)
    
    uint8_t input;                 // direction of the input
    uint8_t output;                // direction of the output
} process;

process process_list[MAX_PROCESSES];    // Lista de procesos

static uint8_t pid_value = 1;         // ID de los procesos (va incrementando)
static uint8_t iter = 0;
static uint8_t dim = 0;
static uint8_t current_process_idx = 0;

void idle_process() {
    while (1) {
        _hlt();
    }
}

uint8_t get_pid() {
    return process_list[current_process_idx].pid;
}

uint8_t get_process_idx(uint8_t pid) {
    for (int i = 0; i < dim; i++) {
        if (process_list[i].pid == pid) {
            return i;
        }
    }
    return NO_PROCESS_FOUND;
}

uint8_t change_priority(uint8_t pid, uint8_t priority) {
    uint8_t idx = get_process_idx(pid);
    if (idx == NO_PROCESS_FOUND) {
        return NO_PROCESS_FOUND;
    }
    if (priority > 5) {
        process_list[idx].priority = 5;
    } else if (priority < 1) {
        process_list[idx].priority = 1;
    } else {
        process_list[idx].priority = priority;
    }
    return TRUE;
}

uint8_t get_state(uint8_t pid) {
    uint8_t idx = get_process_idx(pid);
    if (idx == NO_PROCESS_FOUND) {
        return NO_PROCESS_FOUND;
    }
    return process_list[idx].state;
}

uint8_t block_or_unblock(uint8_t pid) {
    uint8_t idx = get_process_idx(pid);
    if (idx == NO_PROCESS_FOUND) {
        return NO_PROCESS_FOUND;
    }
    process_list[idx].state = (process_list[idx].state == BLOCK) ? READY : BLOCK;
    return TRUE;
}

uint64_t build_stack(uint64_t entry_point, uint64_t stack_end, char **params) {
    uint64_t stack_start = stack_end + STACK_SIZE;
    stack_start -= stack_start % ALINGMENT;

    *(STACK_POSITION(stack_start, RDI_POS)) = (uint64_t) params;

    // TODO: change magic numbers
    for(int i = 7; i < 21; i++){
		if(i != 12)
			*(STACK_POSITION(stack_start, i * 8)) = 0;
	}

    *(STACK_POSITION(stack_start, IP_POS)) = entry_point;
    *(STACK_POSITION(stack_start, CS_POS)) = CODE_SEGMENT;
    *(STACK_POSITION(stack_start, FLAGS_POS)) = FLAGS;
    *(STACK_POSITION(stack_start, SP_POS)) = stack_start - RET_POS;
    *(STACK_POSITION(stack_start, SS_POS)) = STACK_SEGMENT;
    *(STACK_POSITION(stack_start, RET_POS)) = 0;
    return stack_start;
    
}

uint8_t create_process(char **params, uint8_t priority, uint8_t input, uint8_t output, uint8_t immortal, uint64_t entry_point) {
    // check if there is space for a new process
    if(dim == MAX_PROCESSES) {
        return PROCESSES_LIMIT_REACHED;
    }
    // TODO "remember it´s a premature Operative System, please don´t abuse"

    int pos;
	for(pos = 0; process_list[pos].state != DEAD; pos++);	// find a free space

    // Build the stack process
    uint8_t *stack_end = memory_alloc(STACK_SIZE);
    
    if (stack_end == NULL) {
        return NO_SPACE_FOR_PROCESS;
    }
    
    uint8_t *stack_start = (uint8_t *) build_stack(entry_point, stack_end, params);

    process_list[pos].pid = pid_value++;
    process_list[pos].priority = priority;
    process_list[pos].params = params;
    process_list[pos].stack_start = stack_start;
    process_list[pos].stack_end = stack_end;
    process_list[pos].stack_pointer = stack_start;
    process_list[pos].state = READY;
    process_list[pos].ticks = CALCULATE_TICKS(priority);
    process_list[pos].immortal = immortal;
    process_list[pos].input = input;
    process_list[pos].output = output;
    
    dim++;
}

