#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

extern uint64_t get_key();

extern uint64_t t_key();

void keyboard_handler(uint64_t tecla_hex);

char get_first_char();

uint64_t get_buffer_length();

#endif
