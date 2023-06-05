#ifndef INOUT_H
#define INOUT_H

#include <stdint.h>

void buffer_action(char * buffer, uint64_t length);

void printf(const char *format, ...);

void scanf(char* format, ...);

void do_print_color(const char * buffer, uint64_t color);

void do_get_time(char * buffer);

void do_clear_screen(uint64_t color);

void do_division_by_zero();

void do_invalid_op_code();

void do_print_inforeg();

void do_change_font_size(uint64_t level);

char do_get_char();

void do_put_char(char c);

#endif