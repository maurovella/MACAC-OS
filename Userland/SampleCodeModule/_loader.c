// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* _loader.c */
#include <stdint.h>

extern char bss;
extern char end_of_binary;

int main();

void * mem_set(void * destiny, int32_t c, uint64_t length);

int _start() {
	//Clean BSS
	mem_set(&bss, 0, &end_of_binary - &bss);

	return main();

}

void * mem_set(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}
