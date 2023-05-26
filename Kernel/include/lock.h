#ifndef _LOCK_H_
#define _LOCK_H_

#include <stdint.h>

void lock(uint64_t * lock);

void unlock(uint64_t * lock);

#endif
