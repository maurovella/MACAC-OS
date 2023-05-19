#include <naive_graphics_console.h>
#include <time.h>
#include <stdint.h>

#define ZERO_EXCEPTION_ID 0
#define INV_OP_EXC 6
#define REGISTERS 18

#define RED 0x0000FF

extern void restart();

static const char* register_names[REGISTERS] = {
    "rip", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8 ", "r9 ", "r10", "r11", "r12", "r13", "r14", "r15", "rflags"
};

static const char* error_msg = "The program aborted due to an exception: ";

static const char* error_names[7] = {"Divide by Zero\n", "","","","","","Invalid Operation Code\n"};

//Convertion from int to hex of 16 chars (para que queden todos los registros de la misma longitud)
static void int_to_hex(uint64_t num, char buffer[17]){
	int i = 16;
	while (i-- != 0){
		int digit = num % 16;
        buffer[i] = (digit < 10 ? '0' : ('A' - 10)) + digit;
        num /= 16;
	}
}

static void print_registers(uint64_t reg[18]){
	char buf[19];
	buf[0] = '0';
	buf[1] = 'x';
	buf[18] = '\0';
	for (int i = 0; i < REGISTERS; i++){
		ngc_print_color(register_names[i], RED);
		ngc_print_color(" : ", RED);
		int_to_hex(reg[i], buf + 2);
		ngc_print_color(buf, RED);
		ngc_print_new_line();
	}
	
}

void exception_dispatcher(uint64_t exception, uint64_t regdata[18]) {
	ngc_print_color(error_msg, RED);
	ngc_print_color(error_names[exception], RED);
	print_registers(regdata);
	restart();
}