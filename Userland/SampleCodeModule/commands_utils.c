#include <commands_utils.h>

void wait(uint64_t interval) {
	for (int i=0; i<interval*30000;i++){
		;
	}
}