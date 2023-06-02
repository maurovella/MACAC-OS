#include <inout.h>
#include <commands.h>
#include <stdint.h>
#include <string_util.h>
#include <tron.h>
#include <syscalls.h>
#include <colors.h>
#include <shell.h>

static char* commands[] = {"help", "invalidopcode", "dividebyzero", "inforeg", "printmem", "time", "changefontsize", "tron", "clear", "testmm","testprio","testsc"};

/* COMMANDS */
static void (*commands_functions[])(int argc, char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) = {
	help, 
	invalid_op_code, 
	divide_by_zero, 
	inforeg, 
	print_mem, 
	time, 
	change_font_size, 
	tron, 
	clear_screen,
	test_memory_manager,
	test_priority,
	test_scheduler
};

static int find_idx_command(char *buff);

static int parse_buffer(char command[BUFFER_LENGTH], char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], char read_buffer[BUFFER_LENGTH]);

int 
main() {
	do_print_color("WELCOME! Type \"HELP\" for command list\n", WHITE);
	shell();
	return 0;
}


