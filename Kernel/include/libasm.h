#ifndef _LIBASM_H_
#define _LIBASM_H_

#include <stdint.h>

extern int _xadd(uint64_t * lock, int value);

extern int _xchg(uint64_t * lock, int value);

#endif