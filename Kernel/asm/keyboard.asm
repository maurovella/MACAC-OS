GLOBAL get_key
GLOBAL t_key

section .text

getKey:
    push rbp
    mov rbp, rsp

    mov rax, 0

loop: 
    in al, 0x64
    mov cl, al
    and al, 0x01
    cmp al, 0

    je loop
    in al ,0x60

    mov rsp, rbp
    pop rbp
    ret

t_key:
    push rbp
    mov rbp, rsp

    mov rax, 0
    in al, 0x60
    
    mov rsp, rbp
    pop rbp
    ret