#include <test_util.h>
#include <inout.h>
#include <stdint.h>
#include <stddef.h>
#include <colors.h>
#include <string_util.h>
#include <commands.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT       100000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 3  // TODO: Change as required
#define HIGHEST 5 // TODO: Change as required

#define MAX_BLOCKS 128
#define MAX_MEMORY 1000
#define MAX_PROCESSES 20 // shell and idle are occupying 2 positions


#define SEM_ID 1
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
        do_print_color("Test: ", CYAN);
        printf("%d\n", test);
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = (void *)sys_malloc(mm_rqs[rq].size);
            printf("address: %x size: %d\n", mm_rqs[rq].address, mm_rqs[rq].size);
            if(mm_rqs[rq].address == NULL){
                do_print_color("NO MORE MEM AVAILABLE\n", RED);
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
                    do_print_color("test_mm ERROR\n", RED);
                    return;
                }

        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address){
                sys_free(mm_rqs[i].address);
            }
    }
    do_print_color("MEMORY TEST PASSED!!!\n", GREEN);
}

static void test_prio_create_processes(int64_t * pids) {
  pids[0] = sys_create_child_process(NULL, LOWEST, STDIN, STDOUT, (uint64_t) &endless_loop_print);
  pids[1] = sys_create_child_process(NULL, MEDIUM, STDIN, STDOUT, (uint64_t) &endless_loop_print);
  pids[2] = sys_create_child_process(NULL, HIGHEST, STDIN, STDOUT, (uint64_t) &endless_loop_print);
}

static void test_prio_change_priorities(int64_t * pids) {
  int8_t result = 0;
  if ((result = sys_nice(pids[0], MEDIUM)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  if ((result = sys_nice(pids[1], HIGHEST)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  if ((result = sys_nice(pids[2], LOWEST)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  do_print_color("priorities changed...\n", ORANGE);
}

static void test_prio_change_priorities_when_blocked(int64_t * pids) {
  int8_t result = 0;
  if ((result = sys_nice(pids[0], HIGHEST)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  if ((result = sys_nice(pids[1], LOWEST)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  if ((result = sys_nice(pids[2], MEDIUM)) == -1) {
    do_print_color("test_prio: ERROR changing priority\n", RED);
    return;
  }
  do_print_color("priorities changed...\n", ORANGE);
}

static void test_prio_block_unblock_processes(int64_t * pids) {
  sys_block_or_unblock_process(pids[0]);
  sys_block_or_unblock_process(pids[1]);
  sys_block_or_unblock_process(pids[2]);
}

static void test_prio_kill_processes(int64_t * pids) {
  sys_kill_process(pids[0]);
  sys_kill_process(pids[1]);
  sys_kill_process(pids[2]);
}

void test_prio() {
  int64_t pids[3];
  
  test_prio_create_processes(pids);

  bussy_wait(WAIT);
  do_print_color("\nCHANGING PRIORITIES...\n", ORANGE);
  
  test_prio_change_priorities(pids);
  
  bussy_wait(WAIT);
  do_print_color("\nBLOCKING...\n", DARK_BLUE);

  test_prio_block_unblock_processes(pids);

  do_print_color("\nCHANGING PRIORITIES WHILE BLOCKED...\n", ORANGE);
    
  test_prio_change_priorities_when_blocked(pids);

  do_print_color("\nUNBLOCKING...\n", DARK_BLUE);

  test_prio_block_unblock_processes(pids);

  bussy_wait(WAIT);
  do_print_color("\nKILLING...\n", ORANGE);
  
  test_prio_kill_processes(pids);

  do_print_color("PRIORITIES TEST PASSED!!!\n", GREEN);
}

void test_processes() {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = MAX_PROCESSES;
  uint64_t test = 0;
  p_rq p_rqs[MAX_PROCESSES];

  while (test < 5) {
    test++;
    printf("\n\n\n\n");
    do_print_color("TEST ", CYAN);
    printf("%d\n", test);
    printf("\n\n\n");
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      int32_t pid_return = sys_create_child_process(NULL, 1, STDIN, STDOUT, (uint64_t) &endless_loop);
      if (pid_return == -2 || pid_return == -3) {
        do_print_color("test_processes: ERROR creating process\n", RED);
        return ;
      } else {
        p_rqs[rq].pid = pid_return;
        do_print_color("Created process: ", CYBER_GRAPE);
        printf("%d\n", p_rqs[rq].pid);
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
                do_print_color("test_processes: ERROR killing process\n", RED);
                return;
              }
              do_print_color("Killed process: ", ORANGE);
              printf(" %d\n", p_rqs[rq].pid);
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              aux_return = sys_block_or_unblock_process(p_rqs[rq].pid);
              if (aux_return == -1) {
                do_print_color("test_processes: ERROR blocking process\n", RED);
                return ;
              }
              do_print_color("Blocked process ", CYAN);
              printf("%d\n", p_rqs[rq].pid);
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++) {
          if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sys_block_or_unblock_process(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            return ;
          }
          do_print_color("Unblocked process ", CYAN);
          printf("%d\n", p_rqs[rq].pid);
          p_rqs[rq].state = RUNNING;
        }
      }
        
    }
  }
  do_print_color("PROCESSES TEST PASSED!!!\n", GREEN);
}


int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc, uint64_t pid) {
  uint64_t aux = *p;
  //bussy_wait(30000);
  sys_yield(); // This makes the race condition highly probable
  do_print_color("continues after yield. process: ", CYAN);
  printf("%d\n", pid);
  printf("critical section. ACCESSED VARIABLE: %d\n", aux);
  aux += inc;
  printf("critical section. MODIFIED VARIABLE: %d\n", aux);
  *p = aux;
}

uint64_t my_process_inc(char **argv) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  do_print_color("test_sync: ", CYAN);
  printf("starting process %d\n", sys_get_pid());

  if ((n = satoi(argv[0])) <= 0) return -1;
  if ((inc = satoi(argv[1])) == 0) return -1;
  if ((use_sem = satoi(argv[2])) < 0) return -1;

  if (use_sem) {
    if (sys_open_sem(SEM_ID, 1) != 0) {
      printf("test_sync: ERROR creating semaphore\n");
      return -1;
    }
    printf("test_sync: opened/created semaphore %d, with value: %d\n", SEM_ID, 1);
  }

  uint64_t i;
  for (i = 0; i < n; i++) {
    uint64_t curr_pid = sys_get_pid();
    if (use_sem) {
      do_print_color("mutex ", 0x00007f); // red
      printf("process %d ", curr_pid);
      sys_wait_sem(SEM_ID);
      printf("process %d ", curr_pid);
      do_print_color(" -> decremented\n", 0x00007f); // red
    }
    slowInc(&global, inc, curr_pid);
    if (use_sem) {
      do_print_color("mutex ", 0x007f00); // green
      printf("process %d ", curr_pid);
      sys_post_sem(SEM_ID);
      printf("process %d ", curr_pid);
      do_print_color("incremented\n", 0x007f00); // green
    }
  }

  return 0;
}

uint64_t test_sync() { //{n, use_sem, 0}
  uint64_t pids[TOTAL_PAIR_PROCESSES*2];


  char *argvDec[] = {"2", "-1", "1", NULL};
  char *argvInc[] = {"2", "1", "1", NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES*2; i++) {
    if (i % 2 == 0) {
      pids[i] = sys_create_child_process(argvDec, 4, STDIN, STDOUT, (uint64_t) &my_process_inc);
      do_print_color("\nDECREMENT PROCESS\n", CYAN);
      printf("pid created: %d\n", pids[i]);
    }
    else {
      pids[i] = sys_create_child_process(argvInc, MEDIUM, STDIN, STDOUT, (uint64_t) &my_process_inc);
      do_print_color("\nINCREMENT PROCESS\n", CYAN);
      printf("pid created: %d\n", pids[i]);
    }
  }

  sys_wait_for_children();
  sys_close_sem(SEM_ID);
  printf("Final value: %d\n", global);

  return 0;
}

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
  /*
  char * number;
  itoa(pid, number);
  uint8_t color = (pid * 12) %256;
  color <<= 2;
  color += 0x7F;
  color <<= 2;
  color += 0x7F;
  */
  while (1) {
    // do_print_color(number, color);
    printf("%d ", pid);
    bussy_wait(MINOR_WAIT);
  }
}