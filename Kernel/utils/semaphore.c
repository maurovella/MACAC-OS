#include <semaphore.h>



typedef struct sem_info{
    uint64_t sem_id;
    uint64_t sem_val;

    uint8_t is_locked;
    // vamos a necesitar manejar los procesos que esperan
    // 1) uint64_t sem_waiters;
    // 2) o con una cola de procesos esperando
    queue_ADT waiters;
} sem_info;

static uint8_t current_sem_amount = 0;

static sem_info semaphores[MAX_SEM] = {{0}};

static uint8_t find_free_sem() {
    if(current_sem_amount == MAX_SEM) return ERR_MAX_SEM;

    for (int i = 0; i < MAX_SEM; ++i) {
        if (semaphores[i].sem_id == EMPTY_SEM) return i;
    }
    return -1;
}

static uint8_t find_sem(uint64_t sem_id) {
    for (int i = 0; i < MAX_SEM; ++i) {
        if (semaphores[i].sem_id == sem_id) return i;
    }
    return -1;
}

uint8_t create_sem(uint64_t init_val) {
    uint8_t pos_found = find_free_sem();
    if (pos_found == -1) return -1;

    create_queue(&(semaphores[pos_found].waiters), MAX_PROCESS_WAITING);

    semaphores[pos_found].sem_id = pos_found;
    semaphores[pos_found].sem_val = init_val;

    current_sem_amount++;

    return pos_found;
}

uint8_t create_sem_by_id(uint64_t sem_id, uint64_t init_val){

    if (sem_id == EMPTY_SEM) return ERR_INVALID_SEM_ID;
    if (current_sem_amount == MAX_SEM) return ERR_MAX_SEM;

    uint8_t pos_found = -1;
    for (int i = 0; i < MAX_SEM && pos_found == -1; ++i) {

        if (semaphores[i].sem_id == sem_id) return ERR_SEM_EXISTS;
        if (pos_found == -1 && semaphores[i].sem_id == EMPTY_SEM) pos_found = i;
    }

    create_queue(&(semaphores[pos_found].waiters), MAX_PROCESS_WAITING);

    semaphores[pos_found].sem_id = sem_id;
    semaphores[pos_found].sem_val = init_val;

    current_sem_amount++;

    return SUCCESS;
}

uint8_t destroy_sem(uint64_t sem_id) {
    if (sem_id == EMPTY_SEM) return ERR_INVALID_SEM_ID;

    uint8_t pos_found = find_sem(sem_id);

    if (pos_found == -1) return ERR_INVALID_SEM_ID;

    lock(&(semaphores[pos_found].is_locked));
    semaphores[pos_found].sem_id = EMPTY_SEM;
    semaphores[pos_found].sem_val = 0;
    current_sem_amount--;

    destroy_queue(&(semaphores[pos_found].waiters));
    // TODO: destroy queue of waiters
    unlock(&(semaphores[pos_found].is_locked));

    return SUCCESS;    
}

uint8_t wait_sem(uint64_t sem_id) {
    if (sem_id == EMPTY_SEM) return ERR_INVALID_SEM_ID;

    uint8_t pos_found = find_sem(sem_id);

    if (pos_found == -1) return ERR_INVALID_SEM_ID;

    lock(&(semaphores[pos_found].is_locked));
    if (semaphores[pos_found].sem_val == 0) {
        uint64_t current_pid = get_pid();
        change_process_state(current_pid, PAUSED_BY_SEM);
        enqueue(&(semaphores[pos_found].waiters), current_pid);
        unlock(&(semaphores[pos_found].is_locked));
        force_timer_tick();
    }
    else {
        semaphores[pos_found].sem_val--;
        unlock(&(semaphores[pos_found].is_locked));
    }

    return SUCCESS;
}

uint8_t post_sem(uint64_t sem_id) {
    if (sem_id == EMPTY_SEM) return ERR_INVALID_SEM_ID;

    uint8_t pos_found = find_sem(sem_id);

    if (pos_found == -1) return ERR_INVALID_SEM_ID;

    lock(&(semaphores[pos_found].is_locked));
    // TODO
    
    if (get_queue_length(semaphores[pos_found].waiters) > 0) {
        uint64_t pid = dequeue(semaphores[pos_found].waiters);
        change_process_state(pid, READY);
    }
    else {
        semaphores[pos_found].sem_val++;
    }
    unlock(&(semaphores[pos_found].is_locked));

    return SUCCESS;
}


unsigned int get_blocked_process(unsigned int i, unsigned int * blocked_pids) {
    if (i >= MAX_SEM) return 0;

    if(get_queue_length(semaphores[i].waiters) > 0) {
        unsigned int iter_pos;
        create_queue_iterator(&(semaphores[i].waiters), &iter_pos);
        int blocked_procees = 0;
        for(; has_next(&(semaphores[i].waiters),&iter_pos); blocked_procees++) {
            blocked_pids[blocked_procees] = next(&(semaphores[i].waiters),&iter_pos);
        }
        return blocked_procees;
    }
    return 0;
}

unsigned int get_blocked_by_sem_id(unsigned int sem_id, unsigned int * blocked_pids){
	int free_pos = find_sem(sem_id);
	if(free_pos == -1)
		return ERR_INVALID_SEM_ID;

	return get_blocked_process((unsigned int) free_pos, blocked_pids);
}