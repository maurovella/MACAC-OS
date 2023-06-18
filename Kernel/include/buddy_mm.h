#ifdef WITH_BUDDY_MM_

#include <stdint.h>

#define POW_2(x) ((size_t)1 << (x))

#define MIN_ALLOC_LOG2 6 // 2(bytes)^6 = 64(bytes)
#define MIN_ALLOC (POW_2(MIN_ALLOC_LOG2))

typedef struct buddy_node_t {
    uint8_t is_used: 1;
    uint8_t is_splitted: 1;
    uint64_t order;
} buddy_node_t;

#endif