GLOBAL pic_master_mask
GLOBAL pic_slave_mask
GLOBAL halt_cpu
GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti

GLOBAL _irq00_handler
GLOBAL _irq01_handler
GLOBAL _irq02_handler
GLOBAL _irq03_handler
GLOBAL _irq04_handler
GLOBAL _irq05_handler
GLOBAL _irq10_handler
GLOBAL _irq80_handler


GLOBAL _invalid_op_code_interruption
GLOBAL _division_by_zero_interruption

GLOBAL force_timer_tick
GLOBAL force_current_process

EXTERN irq_dispatcher
EXTERN exception_dispatcher
EXTERN syscall_dispatcher
EXTERN keyboard_handler
EXTERN get_RSP
EXTERN consume_tick
EXTERN next_process

GLOBAL info
GLOBAL screenshot


SECTION .text

%macro push_state 0
	push 	rax
	push 	rbx
	push 	rcx
	push 	rdx
	push 	rbp
	push 	rdi
	push 	rsi
	push 	r8
	push 	r9
	push 	r10
	push 	r11
	push 	r12
	push 	r13
	push 	r14
	push 	r15
%endmacro

%macro pop_state 0
	pop 	r15
	pop 	r14
	pop 	r13
	pop 	r12
	pop 	r11
	pop 	r10
	pop 	r9
	pop 	r8
	pop 	rsi
	pop 	rdi
	pop 	rbp
	pop 	rdx
	pop 	rcx
	pop 	rbx
	pop 	rax
%endmacro

%macro push_state_no_rax 0
	push 	rbx
	push 	rcx
	push 	rdx
	push 	rbp
	push 	rdi
	push 	rsi
	push 	r8
	push 	r9
	push 	r10
	push 	r11
	push 	r12
	push 	r13
	push 	r14
	push 	r15
%endmacro

%macro pop_state_no_rax 0
	pop 	r15
	pop 	r14
	pop 	r13
	pop 	r12
	pop 	r11
	pop 	r10
	pop 	r9
	pop 	r8
	pop 	rsi
	pop 	rdi
	pop 	rbp
	pop 	rdx
	pop 	rcx
	pop 	rbx
%endmacro

%macro irq_handler_master 1
	push_state

	mov 	rdi, %1 ; pasaje de parametro
	call 	irq_dispatcher

	; signal pic EOI (End of Interrupt)
	mov 	al, 20h
	out 	20h, al

	pop_state
	iretq
%endmacro



%macro exception_handler 1
	; opcion que trae el valor de RIP cuando ocurre la excepcion tomando la direccion de retorno de la interrupcion
	mov 	[regdata + (1*8)], rax	;rax
	mov 	rax, [rsp]
	mov 	[regdata], rax			;rip
	push_state
	
	;me guardo los registros para imprimir
	;Guardo: rip, rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rflags

	; opcion que ven�amos haciendo
	; mov rax, $

	
	
	mov 	[regdata + (2*8)], rbx	;rbx
	mov 	[regdata + (3*8)], rcx	;rbx
	mov 	[regdata + (4*8)], rdx	;rdx
	mov 	[regdata + (5*8)], rsi	;rsi
	mov 	[regdata + (6*8)], rdi	;rdi
	mov 	[regdata + (7*8)], rbp	;rbp
	mov 	rax, rsp ; We get the value of RSP 
	add 	rax, 0x28 ; We add bytes in order to compensate for the values pushed since the exception occurred and called the exception handler
	mov 	[regdata + (8*8)], rax	;rsp
	mov 	[regdata + (9*8)], r8	;r8
	mov 	[regdata + (10*8)], r9	;r9
	mov 	[regdata + (11*8)], r10	;r10
	mov 	[regdata + (12*8)], r11	;r11
	mov 	[regdata + (13*8)], r12	;r12
	mov 	[regdata + (14*8)], r13	;r13
	mov 	[regdata + (15*8)], r14	;r14
	mov 	[regdata + (16*8)], r15	;r15
	mov 	rax, [rsp+8] ; We get the value of RFLAGS (it is pushed when an interrupt occurs).
	mov 	[regdata + (17*8)], rax	;rflags

	mov 	rdi, %1 ; pasaje de parametro
	mov 	rsi, regdata
	call 	exception_dispatcher

	pop_state
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

pic_master_mask:
	push	rbp
    mov 	rbp, rsp   
    mov 	ax, di
    out		21h,al
    pop		rbp
    retn

pic_slave_mask:
	push	rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out		0A1h, al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00_handler: 
	push_state

	cmp 	BYTE [scheduler_enabled], 1
	jne 	enable_scheduler

	call 	consume_tick
	cmp 	eax, 1
	je 		handle_timer_tick

	mov 	rdi, rsp
	mov 	rsi, ss
	call 	next_process
	mov 	rsp, rax

	handle_timer_tick:
	mov 	rdi, 0				
	call 	irq_dispatcher

	mov 	al, 20h	
	out 	20h, al 	
	pop_state
	iretq

;Keyboard
_irq01_handler:
	;irq_handler_master 1
	push_state
 	mov 	rax, 0
    in 		al, 0x60
	cmp 	al, 0x1D ;me fijo si la tecla es un ctrl
	jne 	.continue
	pop_state
	push_state
.save_regs:
	;Guardo: rip, rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15 
	mov 	[info+(1*8)], rax	;rax
	mov 	rax, [rsp+(15*8)]
	mov 	[info], rax			;rip
	mov 	[info+(2*8)], rbx
	mov 	[info+(3*8)], rcx
	mov 	[info+(4*8)], rdx
	mov 	[info+(5*8)], rsi
	mov 	[info+(6*8)], rdi
	mov 	[info+(7*8)], rbp
	mov 	[info+(8*8)], rsp
	mov 	[info+(9*8)], r8
	mov 	[info+(10*8)], r9
	mov 	[info+(11*8)], r10
	mov 	[info+(12*8)], r11
	mov 	[info+(13*8)], r12
	mov 	[info+(14*8)], r13
	mov 	[info+(15*8)], r14
	mov 	[info+(16*8)], r15
	mov 	byte[screenshot], 1
	jmp 	.end
.continue:
	cmp 	al, 0x9D	;me fijo si la tecla es un ctrl release
	je 		.end

	mov 	rdi, rax
	call 	keyboard_handler
.end:
	mov 	al, 20h
	out 	20h, al

	pop_state
	iretq
	

;Cascade pic never called
_irq02Handler: irq_handler_master 2

;Serial Port 2 and 4
_irq03Handler: irq_handler_master 3

;Serial Port 1 and 3
_irq04Handler: irq_handler_master 4

;USB
_irq05Handler: irq_handler_master 5

;Syscall
_irq80_handler:
	push_state_no_rax
	;Syscall params: 	rdi rsi rdx r10 r8 r9
	;C params: 			rdi rsi rdx rcx r8 r9

	mov 	rcx, r10
	mov 	r9, rax
	call 	syscall_dispatcher
	
	pop_state_no_rax
	iretq

;Zero Division Exception
_division_by_zero_interruption:
	exception_handler 00h

;Invalid Op Code Exception
_invalid_op_code_interruption:
	exception_handler 06h


halt_cpu:
	cli
	hlt
	ret

force_timer_tick:
	int 20h
	ret

force_current_process:
	call 	get_RSP
	mov 	rsp, rax
	pop_state
	iretq

enable_scheduler:
	mov 	BYTE [scheduler_enabled], 1
	jmp 	handle_timer_tick	

SECTION .bss
	aux 		resq 	1
	info 		resq 	17
	regdata 	resq	18
	screenshot 	resb 	1 ;reservo un bit para poner en 1 si hubo un screenshot

SECTION .data
	scheduler_enabled db 0