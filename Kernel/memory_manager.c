#include "memory_manager.h"

static struct memory_info mem_info = { 0 };

void memory_init() {
    mem_info.free_bytes = HEAP_SIZE;
    struct header_info *first_block = (header_info *)HEAP_START;
    first_block->size = 0;
    first_block->allocated = TRUE;
}

void memory_free(void * ptr) {
    if (ptr == NULL || ptr < HEAP_START || ptr >= HEAP_END) {
        return;
    }
    struct header_info *block = ((header_info *) (ptr - sizeof(header_info)));
    uint64_t size = SIZE(block->size);
    mem_info.allocated_bytes -= size;
    mem_info.free_bytes += size;
    mem_info.allocated_blocks--;
    free_block(block);
}

void free_block(header_info * ptr) {
    ptr->allocated = FALSE;
    struct header_info *next = (header_info *) ((void *) ptr + SIZE(ptr->size));
    if (!next->allocated) {
        ptr->size += next->size;
    }
}

void * memory_alloc(uint64_t _size) {
    if (_size <= 0 || _size > MAX_SIZE) {
        return NULL;
    }

    uint64_t true_size = _size + sizeof(struct header_info);
    struct header_info * found_block = find_free_block(true_size);
    if (found_block == NULL) {
        return NULL;
    }

    add_block(found_block, true_size);
    return (void *) ((void *) found_block + sizeof(header_info));
}

struct header_info * find_free_block(uint64_t _size) {
    struct header_info * ptr = (struct header_info *) HEAP_START;

    // iterates till the end of list or finds a free block of _size
    while (ptr < HEAP_END && (ptr->allocated || SIZE(ptr->size) < _size)) {
        ptr = (header_info *) ((void *) ptr + SIZE(ptr->size));
    }

    // We have reached the end of list and found no free block
    if (ptr->size <= 0 && ptr->allocated) {
        // only if the last block at HEAP_END is allocated we return NULL
        if ((void *) ptr + _size + sizeof(struct header_info) > HEAP_END) {
            return NULL;
        }
    }

    // We have found free space
    return ptr;
}

void add_block(header_info * ptr, uint64_t _true_size) {
    uint64_t old_size = SIZE(ptr->size);
   
    if (_true_size + MIN_SIZE <= old_size) { // Split the block
        ptr->size = _true_size; 
        ptr->allocated = TRUE;
        mem_info.allocated_bytes += _true_size;
        mem_info.free_bytes -= _true_size;

        ptr = (header_info *) ((void *) ptr + _true_size);
        ptr->size = old_size - _true_size;
        ptr->allocated = FALSE;
    }
    else if (ptr->size <= 0 && ptr->allocated) { // It is the last block
        ptr->size = _true_size;
        ptr->allocated = TRUE;
        mem_info.allocated_bytes += _true_size;
        mem_info.free_bytes -= _true_size;

        // Create a new end of list block
        ptr = (header_info *) ((void *) ptr + _true_size);
        ptr->size = 0;
        ptr->allocated = TRUE;

    }
    else { // No need to split the block aka normal case
        ptr->size = old_size; 
        ptr->allocated = TRUE;
        mem_info.allocated_bytes += old_size;
        mem_info.free_bytes -= old_size;
    }
    
    mem_info.allocated_blocks++;
}
