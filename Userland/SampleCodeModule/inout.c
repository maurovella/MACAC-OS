#include <syscalls.h>
#include <stdint.h>
#include <inout.h>
#include <string_util.h>
#include <stdarg.h>
#include <exceptions.h>

#define MAX_BUFFER 255

void do_print_color(const char * buffer, uint64_t color){
	sys_print_color(buffer, color);
}

void do_change_font_size(uint64_t level){
	sys_change_font_size(level);
}

static void put_string(const char *buffer){
    //lamo al syswrite 1=stdout
    sys_write(STDOUT, buffer, _str_len(buffer));
}

void buffer_action(char * buffer, uint64_t length){
	int found_enter = 0;
	if (length == 0){
		return;
	}
	char c;
	int i = 0;
	while(!found_enter){
		
		c = do_get_char();
		//if (c != 0xFF){
			// si se ingresa un enter se termina el string y salimos del loop
			if (c == '\n'){
				buffer[i] = '\0';
				do_put_char(c);
				found_enter = 1; 
			}
			// agregamos el char ingresado al buffer
			else if (c >= 0) {
				do_put_char(c);
				if (c == 0x7F && i >= 0) {
					if (i > 0){
						i--;
					}
				}
				// validacion del maximo de scan
				else if (i < length-1) {
					buffer[i] = c;
					i++;
				}    
			}
		//}
	}
}

static uint64_t get_format(uint64_t n) {
	uint64_t dec = n & 240;
	dec = dec >> 4;
	uint64_t units = n & 15;
	return dec * 10 + units;
}


void do_get_time(char * buffer) {
    char* p = buffer;
	uint64_t time = sys_time();
	itoa(get_format(time & 0xFF), p);
	p[2] = ':';
	itoa(get_format((time >> 8) & 0xFF), &p[3]);
	p[5] = ':';
	itoa(get_format((time >> 16) & 0xFF), &p[6]);
    p[8] = 0;
}

void do_division_by_zero(){
	exc_division_by_zero();
}

void do_invalid_op_code(){
	exc_invalid_op_code();
}

char * get_rest_of_string(char string[]){
	return string;
}

char do_get_char(){
	char out;
    while(sys_read(STDIN, &out, 1) == 0);
    return out;
}

//en fmt nos pasa formato ... nos van a mandar las variables a dnd asignar
void scanf(char * format, ...){
	//lectura del buffer
	char buffer[MAX_BUFFER];
	if(sys_read(KBDIN, buffer, MAX_BUFFER) == -1) { //agarra de mas, habria q hacer q lea hasta q termine ??
		return;
	}

	//parseamos para poder asignarselo a cada param
	//primer caracter si o si %
	if(*format != '%'){
		printf("uso incorrecto de scanf\n");
		return;
	}

	va_list vl;
	va_start(vl, format);
	int buff_idx = 0;
	while (*format != '\0'){
		if(*format != '%'){ //letra o espacio
			if ( *format != ' '){
				printf("uso incorrecto de scanf\n");
				return;
			}
			else{
				(*format)++; //deberia tmbn indicar el inicio d un nuevo param
			}
		}
		else{
			(*format)++;
			switch (*format) { //caso en el q estoy en una letra 
            	case 'd':
				case 'D':
					*(int *)va_arg( vl, int* ) = _str_toi(buffer, &buff_idx);	
					//sys_read(d terminal)
					//asignacion a la direccion d memoria pasada  hola que tal
                	//putInt(va_arg(p_arg, int));
                	break;
            	case 'c':
				case 'C':
					*(char *)va_arg( vl, char* ) = buffer[buff_idx++];
                	//putChar(va_arg(p_arg, int));  // char promociona a int
                	break;
				case ' ':
					printf("uso incorrecto de scanf (Fijese de no dejar espacios luego del porcentaje)\n");
					return;
			}
			
			(*format)++;	
		}
	}
	va_end(vl);
}

void do_print_inforeg(){
	uint64_t reg[17];
	uint8_t screenshot = sys_inforeg(reg);
	if (screenshot){
		//rip, rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15 
		printf("rip = %x\n", reg[0]);
		printf("rax = %x\n", reg[1]);
		printf("rbx = %x\n", reg[2]);
		printf("rcx = %x\n", reg[3]);
		printf("rdx = %x\n", reg[4]);
		printf("rsi = %x\n", reg[5]);
		printf("rdi = %x\n", reg[6]);
		printf("rbp = %x\n", reg[7]);
		printf("rsp = %x\n", reg[8]);
		printf("r8  = %x\n", reg[9]);
		printf("r9  = %x\n", reg[10]);
		printf("r10 = %x\n", reg[11]);
		printf("r11 = %x\n", reg[12]);
		printf("r12 = %x\n", reg[13]);
		printf("r13 = %x\n", reg[14]);
		printf("r14 = %x\n", reg[15]);
		printf("r15 = %x\n", reg[16]);
	} else {
		printf("No se ha hecho un screenshot, presione la tecla CTRL e intente de nuevo.\n");
	}
}


void do_clear_screen(uint64_t color) {
	sys_clear_screen(color);	
}


void do_put_char(char c) {
	sys_write(STDOUT, &c, 1);
}

void put_base(int num, int base){

	int i = 12;
	int j = 0;

	char hex[13];

	put_string("0x");
	do{
		hex[i] = "0123456789ABCDEF"[num % base];
		i--;
		num = num/base;
	}while( i > 0 );

	while( ++i < 13){
		hex[j++] = hex[i];
	}

	hex[j] = 0;

	put_string(hex);
}

void put_int(int num){
	char strnum[MAX_INT];
	itoa(num,strnum);
	put_string(strnum);
}

// para una cantidad de argumentos variable usamos la lib stdarg.h
void printf (const char *format, ...) {
	// point p_arg to first argument
   	va_list p_arg;
   	va_start(p_arg, format); // -> hace apuntar p_arg al primer argumento (no format)


    while (*format != '\0') {
		// loop mientras se impriman char normales
        if (*format != '%') {
            do_put_char(*format);
            format++;
        } 

		// si hay un %, me fijo el siguiente para imprimir especial
		else {

        format++;

        switch (*format) {
            case 'd':
			case 'D':
                put_int(va_arg(p_arg, int));
                break;
            case 'c':
			case 'C':
                do_put_char(va_arg(p_arg, int));  // char promociona a int
                break;
            case 's':
			case 'S':
                put_string(va_arg(p_arg, char *));
                break;
			case 'x':
            case 'X':
                put_base(va_arg(p_arg, int), 16);
        }

        format++;
		}
    }
	
    va_end(p_arg);
}
