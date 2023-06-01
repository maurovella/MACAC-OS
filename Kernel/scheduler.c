#include <stdint.h>
#include <scheduler.h>
#include <interrupts.h>
#include <memory_manager.h>
#define STDIN 1
#define STDOUT 0
#define BACKGROUND 4

typedef struct process {
    uint8_t pid;                    // process ID
    uint8_t priority;               // priority of the process (1-5)

    char **params;                  // parameters of the process
    
    void *stack_start;              // start of the stack of the process
    void *stack_end;                // end of the stack of the process
    uint64_t stack_pointer;         // pointer to the stack of the process
    uint64_t stack_segment;         // segment of the stack of the process
    
    uint8_t state;                  // state of the process 
    uint32_t assigned_ticks;        // asigned ticks to the process
    uint32_t remaining_ticks;       // remaining ticks to the process    
    uint8_t immortal;               // indicates if the process is immortal (1) or not (0)
    
    uint8_t input;                 // direction of the input
    uint8_t output;                // direction of the output
} process;

process process_list[MAX_PROCESSES];    // Lista de procesos

static uint8_t pid_value = 0;         // ID de los procesos (va incrementando)
//static uint8_t iter = 0;
static uint8_t dim = 0;
static uint8_t current_process_idx = 0;
static uint8_t idle_pid = 1;
static uint8_t current_remaining_ticks = 0;

static char * idleArg[] = {"idle", NULL};

void scheduler_init() {
    idle_pid = create_process(idleArg, 1, STDIN, BACKGROUND, TRUE, (uint64_t) &idle_process);
    
    change_process_state(idle_pid, PAUSED);

    force_current_process(); //TODO ver si es necesario
}

void change_process_state(uint8_t pid, uint8_t new_state){
	uint8_t pos = get_process_idx(pid);
	if (pos == NO_PROCESS_FOUND) {
        return;
    }
	
	process_list[pos].state = new_state;
}

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
    *(STACK_POSITION(stack_start, RET_POS)) = (uint64_t)&end_process;
    return stack_start;
    
}

uint8_t create_process(char ** params, uint8_t priority, uint8_t input, uint8_t output, uint8_t immortal, uint64_t entry_point) {
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
    
    uint8_t *stack_start = (uint8_t *) build_stack(entry_point, (uint64_t)stack_end, params);

    process_list[pos].pid = pid_value++;
    process_list[pos].priority = priority;
    process_list[pos].params = params;
    process_list[pos].stack_start = stack_start;
    process_list[pos].stack_end = stack_end;
    process_list[pos].stack_pointer = (uint64_t)stack_start;
    process_list[pos].stack_segment = STACK_SEGMENT;
    process_list[pos].state = READY;
    process_list[pos].assigned_ticks = CALCULATE_TICKS(priority);
    process_list[pos].remaining_ticks = process_list[pos].assigned_ticks;
    process_list[pos].immortal = immortal;
    process_list[pos].input = input;
    process_list[pos].output = output;

    dim++;
    return process_list[pos].pid;
}

void end_process() {
    _cli();
    // seteamos el proceso como DEAD y liberamos la memoria del stack y los parametros (free_params) y cambiamos al siguiente proceso
    destroy_process();
    force_timer_tick();
}

void destroy_process() {
    process_list[current_process_idx].state = DEAD;
    free_params(process_list[current_process_idx].params);
    memory_free(process_list[current_process_idx].stack_end);
    dim--;
}

void free_params(char **params) {
    for (int i = 0; params[i] != NULL; i++) {
        memory_free(params[i]);
    }
    memory_free(params);
}

uint8_t kill_process(uint8_t pid) {
    uint8_t idx = get_process_idx(pid);
    if (idx == NO_PROCESS_FOUND) {
        return NO_PROCESS_FOUND;
    }
    if (process_list[idx].immortal) {
        return -1/*CANT_KILL_IMMORTAL_PROCESS*/;
    }
    destroy_process();
    if (current_process_idx == idx) {
        force_timer_tick();
    }
    return TRUE;
}
    
uint64_t get_RSP() {
    return process_list[current_process_idx].stack_pointer;
}

uint64_t next_process(uint64_t stack_pointer, uint64_t stack_segment) {
    process_list[current_process_idx].stack_pointer = stack_pointer;
    process_list[current_process_idx].stack_segment = stack_segment;

    uint8_t found = FALSE;
    uint8_t inactive_processes = 0;
    
    while (!found && (inactive_processes < dim)) {
        current_process_idx = (current_process_idx + 1) % dim;
        if (process_list[current_process_idx].state == READY) {
            found = TRUE;
        } else {
            inactive_processes++;
        }
    }

    if (inactive_processes == dim) {
        current_process_idx = get_process_idx(idle_pid);
        change_process_state(idle_pid, RUNNING);
    } else if (process_list[current_process_idx].pid != idle_pid) {
        change_process_state(idle_pid, PAUSED);
    }
    return process_list[current_process_idx].stack_pointer;
}

uint8_t consume_tick() {
    if (process_list[current_process_idx].remaining_ticks > 0) {
        process_list[current_process_idx].remaining_ticks--;
        return TRUE;
    } 
    return FALSE;
}