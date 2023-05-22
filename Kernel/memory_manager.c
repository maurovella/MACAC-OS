#include "include/memory_manager.h"

static memory_info mem_info = { 0 };

void memory_init() {
    mem_info.free_bytes = HEAP_SIZE;
    header_info *first_block = (header_info *)HEAP_START;
    first_block->size = 0;
    first_block->allocated = TRUE;
}

void memory_free(void * ptr) {
    if (ptr == NULL || ptr < HEAP_START || ptr >= HEAP_END) {
        return;
    }
    header_info *block = ((header_info *) (ptr - sizeof(header_info)));
    uint64_t size = SIZE(block->size);
    mem_info.allocated_bytes -= size;
    mem_info.free_bytes += size;
    mem_info.allocated_blocks--;
    freeBlock(block);
}

void freeBlock(header_info * ptr) {
    ptr->allocated = FALSE;
    header_info *next = (header_info *) ((void *) ptr + SIZE(ptr->size));
    if (next->allocated == FALSE) {
        ptr->size += SIZE(next->size);
    }
}

