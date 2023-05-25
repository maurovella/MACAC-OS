#include "semaphore.h"

#define MAX_SEM 30

typedef struct sem_info{
    uint64_t sem_id;
    uint64_t sem_val;
    // vamos a necesitar manejar los procesos que esperan
    // 1) uint64_t sem_waiters;
    // 2) o con una cola de procesos esperando
} sem_info;

static uint8_t current_sem_amount = 0;

static sem_info semaphores[MAX_SEM] = {{0}};

uint8_t create_sem(uint64_t sem_id, uint64_t init_val){

    if (current_sem_amount == MAX_SEM) return ERR_MAX_SEM;

    uint8_t pos_found = -1;
    for (int i = 0; i < MAX_SEM && pos_found == -1; ++i) {
        if (semaphores[i].sem_id == sem_id) return ERR_SEM_EXISTS;
        if (pos_found == -1 && semaphores[i].sem_id == 0) pos_found = i;
    }
    semaphores[pos_found].sem_id = sem_id;
    semaphores[pos_found].sem_val = init_val;

    current_sem_amount++;

    return SUCCESS;
}