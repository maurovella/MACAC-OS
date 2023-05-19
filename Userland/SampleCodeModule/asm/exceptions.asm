GLOBAL exc_invalid_op_code
GLOBAL exc_division_by_zero

section .text
exc_invalid_op_code:
    mov cr6, rax    ; -> InvalidOpCode Exception
    ret

exc_division_by_zero:
    mov rax, 0
    div rax     ; -> DivideByZero Exception
    ret
