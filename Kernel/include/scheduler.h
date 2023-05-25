#ifndef SCHEDULER_H
#define SCHEDULER_H

#define FLAGS            0x202				
#define STACK_SEGMENT    0x0				
#define CODE_SEGMENT     0x8
#define MAX_PROCESSES    20
#define NO_PROCESS_FOUND -1
#define TRUE             1
#define BLOCK            0
#define READY            1
#define PAUSED           2

void idle_process();

uint8_t get_pid();

uint8_t get_process_idx(uint8_t pid);

uint8_t change_priority(uint8_t pid, uint8_t priority);

uint8_t get_state(uint8_t pid);

uint8_t block_or_unblock(uint8_t pid);

#endif // SCHEDULER_H