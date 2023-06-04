//#ifdef WITH_BUDDY_MM_

#include "include/memory_manager.h"
#include "include/buddy_mm.h"

buddy_node_t * buddy_tree = (buddy_node_t *) HEAP_START + sizeof(memory_info);
uint64_t max_order;

void * user_memory_start;
uint64_t buddy_size;

static uint64_t get_fit_order(uint64_t size) {
    uint64_t order = 0;
    while (size > 0) {
        size >>= 1;     // size = size / 2
        ++order;
    }
    return order < MIN_ALLOC_LOG2 ? MIN_ALLOC_LOG2 : order;
}

static memory_info * get_mem_info() {
    return (memory_info *) HEAP_START;
}

void memory_init() {
    max_order = get_fit_power(HEAP_SIZE);

    uint64_t max_nodes = POW_2(max_order - MIN_ALLOC_LOG2 + 1) - 1;
    
    // check that the size of the buddy_tree is less than HEAP_SIZE
    if (max_nodes * sizeof(buddy_node_t) > HEAP_SIZE) return -1;

    // we declare where the buddy tree nodes start and the buddy_tree whole size
    user_memory_start = buddy_tree + max_nodes * sizeof(buddy_node_t);
    buddy_size = (uint64_t) ((uint64_t) HEAP_END - (uint64_t) user_memory_start);

    // we initialize the memory_info status
    memory_info * memory_info_status = get_mem_info();
    memory_info_status->free_bytes = buddy_size;

    return 0;
}

static get_left_child(uint64_t _buddy_idx) {
    return _buddy_idx * 2 + 1;
}

static get_right_child(uint64_t _buddy_idx) {
    return _buddy_idx * 2 + 2;
}

static uint64_t get_smallest_available_block_rec(uint64_t _order, uint64_t _current_order, uint64_t _buddy_idx) {
    // return -1 when there is no available block
    // return the index of the block otherwise
    
    if (buddy_tree[_buddy_idx].is_used) return -1;
    
    if (_order > _current_order) return -1;

    if (_order == _current_order) {
        if (buddy_tree[_buddy_idx].is_splitted) return -1;

        buddy_tree[_buddy_idx].is_used = TRUE;
        buddy_tree[_buddy_idx].order = _order;
        return _buddy_idx;
    }

    if (buddy_tree[_buddy_idx].is_splitted) return -1;

    // _order < _current_order
    // recursive step.
    // check if left child is available
    uint64_t left_buddy_idx = get_smallest_available_block_rec(_order, _current_order - 1, get_left_child(_buddy_idx));
    if (left_buddy_idx != -1) {
        buddy_tree[_buddy_idx].is_splitted = TRUE;
        return left_buddy_idx;
    }
    uint64_t right_buddy_idx = get_smallest_available_block_rec(_order, _current_order - 1, get_right_child(_buddy_idx));
    if (right_buddy_idx != -1) {
        buddy_tree[_buddy_idx].is_splitted = TRUE;
        return right_buddy_idx;
    }

    return -1;  // no available block
}

static uint64_t get_smallest_available_block(uint64_t _order) {
    return get_smallest_available_block_rec(_order, max_order, 0);
}

uint64_t get_order_start_idx(uint64_t _order) {
    return POW_2(max_order - _order) - 1;
}

static void * get_node_pointer(uint64_t _buddy_idx, uint64_t _order) {
    return (void *) ((uint64_t) HEAP_START + (_buddy_idx - get_class_start_idx(_order)) * POW_2(_order));
}

static uint64_t get_buddy_idx(void * _block_pointer) {
    return *(uint64_t *) _block_pointer;
}

static uint64_t get_sibling_idx(uint64_t _buddy_idx) {
    // return the index of the buddy sibling
    return _buddy_idx % 2 == 0 ? _buddy_idx - 1 : _buddy_idx + 1;
}

void * memory_alloc(uint64_t request) {
    if (request <= 0 || request + HEADER_SIZE > buddy_size) return NULL;

    uint64_t order = get_fit_power(request + HEADER_SIZE);

    if (order > max_order) return NULL;
    
    uint64_t block_index = get_smallest_available_block(order);

    if (block_index == -1) return NULL;

    memory_info * memory_info_status = get_mem_info();

    memory_info_status->allocated_bytes += POW_2(order);
    memory_info_status->free_bytes -= memory_info_status->allocated_bytes;
    memory_info_status->allocated_blocks++;

    void * block_address = get_node_pointer(block_index, order);
    // set block index at the beginning of the block (to identify it easier for memory_free)
    *(uint64_t *) block_address = block_index;   

    return (void *) ((uint64_t) block_address + HEADER_SIZE);
}

static void free_block_rec(uint64_t _buddy_idx) {
    buddy_tree[_buddy_idx].is_used = FALSE;

    uint64_t buddy_sibling_idx = get_sibling_idx(_buddy_idx);
    if (!buddy_tree[buddy_sibling_idx].is_used && !buddy_tree[buddy_sibling_idx].is_splitted) {
        uint64_t parent_idx = get_parent_idx(_buddy_idx);
        buddy_tree[parent_idx].is_splitted = FALSE;
        free_block_rec(parent_idx);
    }
}

void memory_free(void * ptr) {
    if (ptr == NULL) return;

    void * block_pointer = (void *) ((uint64_t) ptr - HEADER_SIZE);

    uint64_t buddy_idx = get_buddy_idx(block_pointer);
    
    // check if it is already freed
    if (!buddy_tree[buddy_idx].is_used) return;

    free_block_rec(buddy_idx);

    memory_info * memory_info_status = get_mem_info();

    memory_info_status->allocated_bytes -= POW_2(buddy_tree[buddy_idx].order);
    memory_info_status->free_bytes += memory_info_status->allocated_bytes;
    memory_info_status->allocated_blocks--;
}



//#endif