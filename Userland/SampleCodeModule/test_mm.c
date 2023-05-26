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
    while (test < 43) {
        test++;
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = (void *)sys_malloc(mm_rqs[rq].size);
        
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
            if (mm_rqs[i].address)
                sys_free(mm_rqs[i].address);
    }
    printf("TEST PASSED!!!\n");
}