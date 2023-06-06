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
GLOBAL sys_wait
GLOBAL sys_change_priority
GLOBAL sys_get_pid
GLOBAL sys_get_all_processes
GLOBAL sys_get_mem_info
GLOBAL sys_delete_last_char
GLOBAL sys_nice
GLOBAL sys_create_pipe_available
GLOBAL sys_delete_pipe
GLOBAL sys_get_current_output
GLOBAL sys_get_current_input
GLOBAL sys_create_sem
GLOBAL sys_create_sem_by_id
GLOBAL sys_wait_sem
GLOBAL sys_post_sem
GLOBAL sys_close_sem
GLOBAL sys_open_sem

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

sys_wait_for_children:
    mov rax, 0x12
    int 80h
    ret

sys_wait:
    mov rax, 0x13
    int 80h
    ret

sys_change_priority:
    mov rax, 0x14
    int 80h
    ret

sys_get_pid:
    mov rax, 0x15
    int 80h
    ret

sys_get_all_processes:
    mov rax, 0x16
    int 80h
    ret

sys_get_mem_info:
    mov rax, 0x17
    int 80h
    ret

sys_delete_last_char:
    mov rax, 0x18
    int 80h
    ret

sys_nice:
    mov rax, 0x19
    int 80h
    ret

sys_create_pipe_available:
    mov rax, 0x1A
    int 80h
    ret

sys_delete_pipe:
    mov rax, 0x1B
    int 80h
    ret

sys_get_current_output:
    mov rax, 0x1C
    int 80h
    ret

sys_get_current_input:
    mov rax, 0x1D
    int 80h
    ret

sys_create_sem:
    mov rax, 0x1E
    int 80h
    ret

sys_create_sem_by_id:
    mov rax, 0x1F
    int 80h
    ret

sys_open_sem:
    mov rax, 0x20
    int 80h
    ret

sys_wait_sem:
    mov rax, 0x21
    int 80h
    ret

sys_post_sem:
    mov rax, 0x22
    int 80h
    ret

sys_close_sem:
    mov rax, 0x23
    int 80h
    ret
