#include <queue.h>S

typedef struct queue_CDT{
    uint64_t  * elements;
    unsigned int read_pos;
    unsigned int write_pos;
    unsigned int length;        // mucho mas entendible con una variable
    unsigned int size;
}queue_CDT;

void create_queue(queue_ADT queue, unsigned int size) {

    if(size <= 0) {
        return;
    }

    queue->elements = memory_alloc(size * sizeof(uint64_t));
    queue->read_pos = 0;
    queue->write_pos = 0;
    queue->length = 0;
    queue->size = size;
}

unsigned int get_queue_length(queue_ADT queue) {
    return queue->length;
}

void enqueue(queue_ADT queue, uint64_t elem) {
    if(queue->length == queue->size) {
        return;
    }

    queue->elements[queue->write_pos] = elem;
    queue->write_pos = (queue->write_pos + 1) % queue->size;
    queue->length++;
}

uint64_t dequeue(queue_ADT queue) {
    if(queue->length == 0) {
        return EMPTY_QUEUE_ERROR;
    }

    uint64_t elem = queue->elements[queue->read_pos];
    queue->read_pos = (queue->read_pos + 1) % queue->size;
    queue->length--;

    return elem;
}

void destroy_queue(queue_ADT queue) {
    memory_free(queue->elements);
}

uint8_t is_in_queue(queue_ADT queue, uint64_t elem) {
    for(int i = 0; i < queue->length; i++) {
        if(queue->elements[(queue->read_pos + i) % queue->size] == elem) {
            return TRUE;
        }
    }

    return FALSE;
}

void create_queue_iterator(queue_ADT queue, unsigned int * iter_pos) {
    *iter_pos = queue->read_pos;
}

uint8_t has_next(queue_ADT queue, unsigned int iter_pos) {
    if(queue->read_pos > queue->write_pos) {
        return (*iter_pos >= queue->read_pos && queue->size > *iter_pos) || (*iter_pos < queue->write_pos);
    }
    return *iter_pos < queue->write_pos && *iter_pos >= queue->read_pos;
}

uint64_t next(queue_ADT queue, unsigned int * iter_pos) {

    if(has_next(queue, *iter_pos) == FALSE) {
        return NO_MORE_ELEMS;
    }

    uint64_t elem = queue->elements[*iter_pos];
    *iter_pos = (*iter_pos + 1) % queue->size;
    return elem;
}