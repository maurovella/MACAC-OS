#ifndef NAIVE_GRAPHICS_CONSOLE_H
#define NAIVE_GRAPHICS_CONSOLE_H

#include <stdint.h>
#define CHAR_WIDTH 9 
#define CHAR_HEIGHT 16 

enum Dir {DOWN = 0, UP = 1};

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
} Color;


void ngc_print_new_line(void);

void ngc_print_char(char c);

void ngc_print(const char * s);

uint16_t ngc_get_height(); 

uint16_t ngc_get_width(); 

void ngc_print_color(const char * buffer, uint64_t color);

void change_font_size(uint64_t new_level);

void ngc_paint_screen(uint64_t bg_color);

void ngc_print_pixels(uint64_t from_x, uint64_t from_y, uint16_t width, uint16_t height, uint64_t color);

void delete_last_char();

#endif