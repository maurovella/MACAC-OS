#include <stdint.h>
#include <defs.h>
#include <string.h>
#include <keyboard.h>
#include <naiveRTC.h>
#include <naiveGraphicsConsole.h>

extern uint64_t info[17];
extern uint8_t screenshot;


static void sys_write_handler(unsigned int fd, const char * buffer, int bytes){
    if (fd == STDOUT){
        ngc_print(buffer); //deberia recibir los bytes
    }
}


static void sys_read_handler(unsigned int fd,  char * buffer, int bytes){
    if (fd == STDIN || fd == KBDIN){
        uint64_t aux = 0;
        int flag = 0;
		while(!flag && *buffer != '\t' ){
            _hlt();
            if (getLastChar() == '\n'){
                flag = 1;
            }
        }
        uint64_t n = getBufferLength();
        memcpy(buffer,getBuffer(),n);
    }
}

static uint64_t sys_time_handler(){
    return (_NRTCGetHours()) | ((uint64_t)_NRTCGetMins() << 8) | ((uint64_t)_NRTCGetSeconds() << 16);
}

static void sys_clear_screen_handler(Color color) {
    ngc_paint_screen(color);
}

static uint8_t sys_inforeg_handler(uint64_t regVec[17]){
    if (screenshot){
        for (int i = 0; i < 17; i++)
        {
            regVec[i] = info[i];
        }
        
    }
    return screenshot;
}

static void sys_font_handler(uint64_t level){
    changeFontSize(level);
}

static void sys_printColor_handler(const char *buffer, Color color){
    ngc_printColor(buffer, color);
}

// 2 por altura y ancho
static uint64_t * sys_screenData_handler(){
    uint64_t data[2] = {{0};}
    data[0] = getWidth();
    data[1] = getHeight();
    return data;
}

static void (* syscalls[30])(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax) = {sys_read_handler, sys_write_handler, sys_time_handler, sys_clear_screen_handler, sys_inforeg_handler, sys_font_handler, sys_printColor_handler, sys_screenData_handler};

//  paso syscall_id por rax, se come r10 por rcx, y r9 por eax
void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax){
    syscalls[rax](rdi, rsi, rdx, rcx, r8);
}

