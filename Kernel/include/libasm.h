#ifndef _LIBASM_H_
#define _LIBASM_H_

int _xadd(unsigned int * lock, int value);

int _xchg(unsigned int * lock, int value);

#endif