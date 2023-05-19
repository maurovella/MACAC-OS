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

void * mem_set(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
