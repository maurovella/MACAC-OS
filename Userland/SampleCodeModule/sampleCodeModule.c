#include <shell.h>

int main() {
	do_print_color("WELCOME! Type \"HELP\" for command list\n", WHITE);
	shell();
	return 0;
}
