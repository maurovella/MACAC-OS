#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3


extern void sys_write(uint64_t fd, const char * buffer, uint64_t count);

extern int64_t sys_read(uint64_t fd, const char * buffer, uint64_t count);

uint64_t sys_time();

uint8_t sys_inforeg(uint64_t regVec[17]);

void sys_change_font_size(uint64_t level);

void sys_print_color(const char * buffer, uint64_t color);

void sys_get_screen_data(uint16_t * screen);

void sys_paint_rect(uint16_t from_x, uint16_t from_y, uint16_t width, uint16_t height, uint64_t color);

uint64_t sys_get_ticks();
 
void sys_clear_screen(uint64_t color);

void sys_beeper(uint64_t frequency, uint64_t seconds);

uint64_t sys_malloc(uint64_t len);

void sys_free(void * ptr);

void sys_mm_init();

int32_t sys_create_process(char ** params, uint8_t priority, uint8_t input, uint8_t output, uint64_t entry_point);

int32_t sys_kill_process(uint32_t pid);

int32_t sys_block_or_unblock_process(uint32_t pid);

int32_t sys_create_child_process(char ** params, uint8_t priority, uint8_t input, uint8_t output, uint64_t entry_point);

void sys_wait_for_children();

int8_t sys_change_priority(uint32_t pid, uint8_t priority);

int32_t sys_get_pid();

#endif