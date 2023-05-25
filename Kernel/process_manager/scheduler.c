#include <stdint.h>
#include "scheduler.h"
#include "interrupts.h"



typedef struct process {
    uint8_t pid;                    // ID del proceso
    uint8_t priority;               // Prioridad del proceso
    void (*function)(void *);       // Puntero a la función del proceso
    void **params;                  // Parámetros del proceso (arreglo de cadenas)
    
    void *stack_start;              // Inicio del stack del proceso
    void *stack_end;                // Final del stack del proceso
    void *stack_pointer;            // Puntero al stack actual del proceso
    
    uint8_t state;                  // Estado del proceso (BLOCK, READY, PAUSED)
    uint32_t ticks;                 // Ticks asignados por el scheduler
    uint8_t immortal;               // Indica si el proceso es inmortal
    
    uint8_t *input;                 // Descriptor/identificador entrada asociado al proceso
    uint8_t *output;                // Descriptor/identificador salida asociado al proceso
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
