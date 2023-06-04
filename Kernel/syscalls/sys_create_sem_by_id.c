#include "semaphore.h"

uint64_t sys_create_sem_by_id(uint64_t sem_id, uint64_t init_val){
    return create_sem_by_id(sem_id, init_val);
}
