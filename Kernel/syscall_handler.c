#include <stdint.h>
#include <defs.h>
#include <string.h>
#include <keyboard.h>
#include <naive_RTC.h>
#include <naive_graphics_console.h>
#include <time.h>
#include <speaker.h>
#include <interrupts.h>

extern uint64_t info[17];
extern uint8_t screenshot;

typedef int64_t (*syscall_type) (uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static void sys_write_handler(uint64_t fd, uint64_t buffer, uint64_t bytes){
    if (fd == STDOUT) {
        for (uint64_t i = 0; i < bytes; i++){
            ngc_print_char(((char*)buffer)[i]);
        }
    }
}

static int64_t sys_read_handler(uint64_t fd, char * buffer, uint64_t bytes){
    if (fd != STDIN && fd != KBDIN) return -1;
    int64_t i = 0;
    char c;
    while(i < bytes && (c = get_first_char()) != 0xFF) {
        buffer[i] = c;
        i++;
    }
    return i;
}

static uint64_t sys_time_handler(){
    return (_NRTC_get_hours()) | ((uint64_t)_NRTC_get_mins() << 8) | ((uint64_t)_NRTC_get_seconds() << 16);
}

static uint8_t sys_inforeg_handler(uint64_t reg_vec[17]){
    if (screenshot){
        for (int i = 0; i < 17; i++)
        {
            reg_vec[i] = info[i];
        }
        
    }
    return screenshot;
}

static void sys_font_handler(uint64_t level){
    change_font_size(level);
}

static void sys_print_color_handler(const char *buffer, uint64_t color){
    ngc_print_color(buffer, color);
}

static void sys_clear_screen_handler(uint64_t color) {
    ngc_paint_screen(color);
}

// 2 por altura y ancho
static void sys_screen_data_handler(uint16_t * data){
    data[0] = ngc_get_width();
    data[1] = ngc_get_height();
    return;
}

static void sys_paint_rect_handler(uint64_t from_x, uint64_t from_y, uint16_t width, uint16_t height, uint64_t color) {
    ngc_print_pixels(from_x, from_y, width, height, color);
}

static uint64_t sys_ticks_handler(){
    return ticks_elapsed();
}

static void sys_beeper_handler(uint64_t frequency, uint64_t interval) {
    beep(frequency);
    wait(interval);
    stop_beep();
}

static syscall_type syscalls[]  = {
    (syscall_type) sys_read_handler, 
    (syscall_type) sys_write_handler, 
    (syscall_type) sys_time_handler, 
    (syscall_type) sys_inforeg_handler, 
    (syscall_type) sys_font_handler, 
    (syscall_type) sys_print_color_handler, 
    (syscall_type) sys_clear_screen_handler, 
    (syscall_type) sys_screen_data_handler, 
    (syscall_type) sys_paint_rect_handler, 
    (syscall_type) sys_ticks_handler, 
    (syscall_type) sys_beeper_handler
};

//  paso syscall_id por rax, se come r10 por rcx, y r9 por rax
int64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax){
    return syscalls[rax](rdi, rsi, rdx, rcx, r8);
}

