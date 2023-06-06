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

/*
    open_sem
    -------------------------------------
    Description: Opens a semaphore
        if it doesn't exist, it creates it
        with initial value = 0
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        sem_id: id of the semaphore
    -------------------------------------
    Returns:
        sem_id if success
        -1 if error
*/
uint8_t open_sem(uint64_t sem_id, uint64_t init_val);

/*
    wait_sem
    -------------------------------------
    Description: Decrements the semaphore
        if it's 0, it blocks the process
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        sem_id: id of the semaphore
    -------------------------------------
    Returns:
        0 if success
        errorcode if error
*/
uint8_t wait_sem(uint64_t sem_id);

/*
    signal_sem
    -------------------------------------
    Description: Increments the semaphore
        if there are processes waiting, it
        unblocks one
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        sem_id: id of the semaphore
    -------------------------------------
    Returns:
        0 if success
        errorcode if error
*/
uint8_t post_sem(uint64_t sem_id);

/*
    close_sem
    -------------------------------------
    Description: Destroys a semaphore
    ** reuturns ERROR **
    -------------------------------------
    Parameters:
        sem_id: id of the semaphore
    -------------------------------------
    Returns:
        0 if success
        errorcode if error
*/
uint8_t close_sem(uint64_t sem_id);

#endif
