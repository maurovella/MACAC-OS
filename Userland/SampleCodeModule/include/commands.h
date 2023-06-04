#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_PARAMETERS 5
#define LENGTH_PARAMETERS 256
#define BUFFER_LENGTH 256
#define COMMANDS_LENGTH 18
#define REGISTERS 17

void help();

void invalid_op_code();

void divide_by_zero();

void inforeg();

void print_mem(char** params);

void time();

void change_font_size(char ** params);

void tron();

void clear_screen();

void test_memory_manager();

void test_scheduler();

void test_priority();

void ps();

void kill(char ** params);

void block(char ** params);

void loop();

void nice(char ** params);


void cat();

void wc();

void filter();

void phylo();

#endif