// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <module_loader.h>
#include <naive_RTC.h>
#include <idt_loader.h>
#include <time.h>
#include <defs.h>
#include <naive_graphics_console.h>
#include <interrupts.h>
#include <speaker.h>
#include <memory_manager.h>
#include <scheduler.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t end_of_kernel_binary;
extern uint8_t end_of_kernel;

static const uint64_t page_size = 0x1000;

static void * const sample_code_module_address = (void*)0x400000;
static void * const sample_data_module_address = (void*)0x500000;

static char * shell_args[] = {"shell", NULL};

typedef int (*entry_point)();

void clear_BSS(void * bss_address, uint64_t bss_size)
{
	mem_set(bss_address, 0, bss_size);
	return;
}

void * get_stack_base()
{
	return (void*)(
		(uint64_t)&end_of_kernel
		+ page_size * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initialize_kernel_binary()
{
	
	void * module_addresses[] = {
		sample_code_module_address,
		sample_data_module_address
	};

	load_modules(&end_of_kernel_binary, module_addresses);

	clear_BSS(&bss, &end_of_kernel - &bss);

	return get_stack_base();
}

int main()
{	
	// load 
	load_idt();

	memory_init();
	
	// Create shell before idle
	create_process(shell_args, 1, STDIN, STDOUT, TRUE, (uint64_t) sample_code_module_address);
	
	// Create idle process and force to run shell
	scheduler_init();
	
	
	//((entry_point)sample_code_module_address)();
	while(1);
	return 0;
}
