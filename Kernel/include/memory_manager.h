#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#include <stddef.h>

#define HEAP_SIZE 0x100000 // 1MB
#define HEAP_START ((void *)0x100000) // arbitrary
#define HEAP_END ((void*)(HEAP_START + HEAP_SIZE)) // check if legal
#define TRUE 1
#define FALSE 0
#define SIZE(size) ((size) & ~0x1) // masks the last bit of size, gets just the size

typedef struct memory_info {
    uint64_t allocated_bytes;   
    uint64_t free_bytes;       
    uint64_t allocated_blocks;  
} memory_info;

typedef struct header_info {
    uint64_t size;
    uint8_t allocated; // 1 if allocated 0 if free
} header_info;

void memory_init();

void memory_free(void * ptr);

void freeBlock(header_info * ptr);

#endif
