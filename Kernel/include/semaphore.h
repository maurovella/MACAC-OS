#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <stdint.h>
#include <queue.h>
#include <scheduler.h>

#define EMPTY_SEM 0
#define MAX_SEM 30
#define MAX_PROCESS_WAITING 10

#define SUCCESS 0


#define ERR_MAX_SEM -1
#define ERR_SEM_EXISTS -2
#define ERR_INVALID_SEM_ID -3



/*
    create_sem
    -------------------------------------
    Description: Creates a new semaphore
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        init_val: initial value of the 
                    semaphore
    -------------------------------------
    Returns:
        sem_id if success
        -1 if error
*/
uint8_t create_sem(uint64_t init_val);

/*
    create_sem_by_id
    -------------------------------------
    Description: Creates a new semaphore
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        sem_id: id of the semaphore
        init_val: initial value of the 
                    semaphore
    -------------------------------------
    Returns:
        0 if success
        errorcode if error
*/
uint8_t create_sem_by_id(uint64_t sem_id, uint64_t init_val);

#endif
