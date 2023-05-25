#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <stdint.h>

#define SUCCESS 0

#define ERR_MAX_SEM 1
#define ERR_SEM_EXISTS 2

uint8_t create_sem(uint64_t sem_id, uint64_t init_val);

#endif
