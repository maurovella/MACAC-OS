#ifndef	_QUEUE_H
#define _QUEUE_H

#include <stdint.h>

#define EMPTY_QUEUE_ERROR -1
#define NO_MORE_ELEMS -2

#define TRUE 1
#define FALSE 0

typedef struct queue_CDT * queue_ADT;

void create_queue(queue_ADT queue, unsigned int size); 

void enqueue(queue_ADT queue, uint64_t elem);
uint64_t dequeue(queue_ADT queue);
void destroy_queue(queue_ADT queue);

uint8_t is_in_queue(queue_ADT queue, uint64_t elem);
unsigned int get_queue_current_size(queue_ADT queue); 

void create_queue_iterator(queue_ADT queue, unsigned int * iter_pos);
uint64_t next(queue_ADT queue, unsigned int * iter_pos);
uint8_t has_next(queue_ADT queue, unsigned int * iter_pos);

#endif