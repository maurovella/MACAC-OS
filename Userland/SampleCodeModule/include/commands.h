#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_PARAMETERS 5
#define LENGTH_PARAMETERS 256
#define BUFFER_LENGTH 256
#define COMMANDS_LENGTH 25
#define REGISTERS 17
#define MAX_BUFFER 256

#define IS_ALLOWED_CHAR(c) ((c >= 'a' && c <= 'z') || (c == '|') || (c == ' ') || (c == '\0') || (c == '\n') || (c == '\t'))
#define IS_NOT_VOWEL(c) (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') 

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

void test_synchronization();

void ps();

void kill(char ** params);

void block(char ** params);

void loop();

void nice(char ** params);


void cat();

void wc();

void filter();

void philo();

void mem();

void pipe_commands();

void thanks();
#endif