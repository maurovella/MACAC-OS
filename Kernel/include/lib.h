#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * mem_set(void * destination, int32_t character, uint64_t length);
void * mem_cpy(void * destination, const void * source, uint64_t length);

char *cpu_vendor(char *result);

#endif