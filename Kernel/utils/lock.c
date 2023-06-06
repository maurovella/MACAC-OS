// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lock.h>

void lock(uint64_t * lock){
	while(_xchg(lock, 1) != 0);
}

void unlock(uint64_t * lock){
	_xchg(lock,0);
}
