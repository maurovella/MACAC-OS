#include "lock.h"

void lock(uint64_t * lock){
	while(_xchg(lock, 1) != 0);
}

void unlock(uint64_t * lock){
	_xchg(lock,0);
}
