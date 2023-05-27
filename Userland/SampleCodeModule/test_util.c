#include <test_util.h>
#include <inout.h>
#include <stdint.h>
#include <syscalls.h>
#include <stddef.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 1000

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void* setmem(void* destination, int32_t c, uint64_t length) {
    uint8_t chr = (uint8_t)c;
    char* dst = (char*)destination;
    while (length--) {
        dst[length] = chr;
    }
    return destination;
}

void test_mm(){
    sys_mm_init();
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

void endless_loop_print(uint64_t wait) {
  //int64_t pid = my_getpid();
  uint64_t pid = 1;
  while (1) {
    printf("%d ", pid);
    bussy_wait(wait);
  }
}