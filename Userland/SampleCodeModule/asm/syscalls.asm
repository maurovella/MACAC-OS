GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_time
GLOBAL sys_inforeg
GLOBAL sys_change_font_size
GLOBAL sys_print_color
GLOBAL sys_clear_screen
GLOBAL sys_get_screen_data
GLOBAL sys_paint_rect
GLOBAL sys_get_ticks
GLOBAL sys_timed_read
GLOBAL sys_beeper
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_mm_init
GLOBAL sys_create_process
GLOBAL sys_kill_process
GLOBAL sys_block_or_unblock_process
GLOBAL sys_create_child_process
GLOBAL sys_wait_for_children

section .text
sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_time:
    mov rax, 0x02
    int 80h
    ret

sys_inforeg:
    mov rax, 0x03
    int 80h
    ret

sys_change_font_size:
    mov rax, 0x04
    int 80h
    ret

sys_print_color:
    mov rax, 0x05
    int 80h
    ret

sys_clear_screen:
    mov rax, 0x06
    int 80h
    ret

sys_get_screen_data:
    mov rax, 0x07
    int 80h
    ret

sys_paint_rect:
    mov rax, 0x08 
    mov r10, rcx
    int 80h
    ret

sys_get_ticks:
    mov rax, 0x09
    int 80h
    ret


sys_beeper:
    mov rax, 0x0A
    int 80h
    ret

sys_malloc:
    mov rax, 0x0B
    int 80h
    ret

sys_free:
    mov rax, 0x0C
    int 80h
    ret

sys_mm_init:
    mov rax, 0x0D
    int 80h
    ret

sys_create_process:
    mov rax, 0x0E
    int 80h
    ret

sys_kill_process:
    mov rax, 0x0F
    int 80h
    ret

sys_block_or_unblock_process:
    mov rax, 0x10
    int 80h
    ret

sys_create_child_process:
    mov rax, 0x11
    int 80h
    ret

sys_wait_for_children
    mov rax, 0x12
    int 80h
    ret