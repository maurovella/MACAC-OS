#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <naive_graphics_console.h>

static void int_20();

//static void int_21();

void irq_dispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		/*case 1:
			int_21();
			break;*/
	}
	return;
}



void int_20() {
	timer_handler();
}
/*
void int_21(){
	keyboard_handler();
}
*/