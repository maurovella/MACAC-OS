#include "semaphore.h"

uint64_t sys_create_sem(uint64_t sem_id, uint64_t init_val){
    return create_sem(sem_id, init_val);
}
