#include <stdint.h>
#include "scheduler.h"
#include "interrupts.h"

#define FLAGS            0x202				
#define STACK_SEGMENT    0x0				
#define CODE_SEGMENT     0x8
#define MAX_PROCESSES    20

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

static uint8_t current_pid = 1;         // Indice del proceso actual

void idle_process() {
    while (1) {
        _hlt();
    }
}
