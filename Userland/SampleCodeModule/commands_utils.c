// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands_utils.h>

void wait(uint64_t interval) {
	for (int i=0; i<interval*30000;i++){
		;
	}
}