#include <test_util.h>
#include <inout.h>
#include <stdint.h>
#include <stddef.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT       10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 3  // TODO: Change as required
#define HIGHEST 5 // TODO: Change as required

#define MAX_BLOCKS 128
#define MAX_MEMORY 1000
#define MAX_PROCESSES 20 // shell and idle are occupying 2 positions


#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  uint32_t pid;
  enum State state;
} p_rq;




int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void* setmem(void* destination, int32_t c, uint64_t length) {
    uint8_t chr = (uint8_t)c;
    char* dst = (char*)destination;
    while (length--) {
        dst[length] = chr;
    }
    return destination;
}

void test_mm(){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory;

    max_memory = MAX_MEMORY;
    int test = 0;
    while (test < 50) {
        test++;
        printf("Test %d\n", test);
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = (void *)sys_malloc(mm_rqs[rq].size);
            printf("address: %x size: %d\n", mm_rqs[rq].address, mm_rqs[rq].size);
            if(mm_rqs[rq].address == NULL){
                printf("NO MORE MEM AVAILABLE\n");
                return;
            }

            if (mm_rqs[rq].address) {
                total += mm_rqs[rq].size;
                rq++;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                setmem(mm_rqs[i].address, i, mm_rqs[i].size);
            
                

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                    printf("test_mm ERROR\n");
                    return;
                }

        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address){
                sys_free(mm_rqs[i].address);
            }
    }
    printf("TEST PASSED!!!\n");
}

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = sys_create_process(argv, 1, STDIN, STDOUT, (uint64_t) &endless_loop_print);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");
  int8_t changed = 0;
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    changed = sys_change_priority(pids[i], prio[i]);
    if (changed == -1) {
      printf("test_prio: ERROR changing priority\n");
      return;
    }
  }
    

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_block_or_unblock_process(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    changed = sys_change_priority(pids[i], prio[i+1 % TOTAL_PROCESSES]);
    if (changed == -1) {
      printf("test_prio: ERROR changing priority\n");
      return;
    }
  }
    

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_block_or_unblock_process(pids[i]);

  printf("\nKILLING...\n");
  bussy_wait(WAIT);
  

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_kill_process(pids[i]);

  printf("TEST PASSED!!!\n");
}

void test_processes() {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = MAX_PROCESSES;
  uint64_t test = 0;
  p_rq p_rqs[max_processes];

  while (test < 5) {
    test++;
    printf("\n\n\n\n\n");
    printf("Test %d\n", test);
    printf("\n\n\n\n");
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      int32_t pid_return = sys_create_process(NULL, 1, 1, 1, (uint64_t) &endless_loop);
      if (pid_return == -2) {
        printf("test_processes: ERROR creating process\n");
        return ;
      } else {
        p_rqs[rq].pid = pid_return;
        printf("Created process %d\n", p_rqs[rq].pid);
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;
        int32_t aux_return;
        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              aux_return = sys_kill_process(p_rqs[rq].pid);
              if (aux_return == -1 || aux_return == -4) {
                printf("test_processes: ERROR killing process\n");
                return;
              }
              printf("Killed process %d\n", p_rqs[rq].pid);
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              aux_return = sys_block_or_unblock_process(p_rqs[rq].pid);
              if (aux_return == -1) {
                printf("test_processes: ERROR blocking process\n");
                return ;
              }
              printf("Blocked process %d\n", p_rqs[rq].pid);
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sys_block_or_unblock_process(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            return ;
          }
          printf("Unblocked process %d\n", p_rqs[rq].pid);
          p_rqs[rq].state = RUNNING;
        }
    }
  }
  printf("TEST PASSED!!!\n");
}

/*
int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!my_sem_open(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  if (use_sem)
    my_sem_close(SEM_ID);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process("my_process_inc", 3, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process("my_process_inc", 3, argvInc);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d\n", global);

  return 0;
}


*/













// Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

// Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

// Parameters
int64_t satoi(char *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}

// Dummies
void bussy_wait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endless_loop() {
  while (1)
    ;
}

void endless_loop_print() {
  int32_t pid = sys_get_pid();
  while (1) {
    printf("%d ", pid);
    bussy_wait(MINOR_WAIT);
  }
}