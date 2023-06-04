#include "semaphore.h"

uint64_t sys_create_sem(uint64_t init_val){
    return create_sem(init_val);
}
