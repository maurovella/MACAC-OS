#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_PARAMETERS 5
#define LENGTH_PARAMETERS 256
#define BUFFER_LENGTH 256
#define COMMANDS_LENGTH 10
#define REGISTERS 17

void help(int argc, char params[MAX_PARAMETERS][LENGTH_PARAMETERS]);

void invalid_op_code(int argc, char  params[][LENGTH_PARAMETERS]);

void divide_by_zero(int argc, char  params[][LENGTH_PARAMETERS]);

void inforeg(int argc, char params[][LENGTH_PARAMETERS]);

void print_mem(int argc, char params[][LENGTH_PARAMETERS]);

void time(int argc, char params[][LENGTH_PARAMETERS]);

void change_font_size(int argc, char params[][LENGTH_PARAMETERS]);

void tron(int argc, char params[][LENGTH_PARAMETERS]);

void clear_screen(int argc, char params[][LENGTH_PARAMETERS]);

void test_memory_manager(int argc, char params[][LENGTH_PARAMETERS]);

#endif