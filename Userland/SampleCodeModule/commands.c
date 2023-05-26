#include <stdint.h>
#include <commands.h>
#include <tron.h>
#include <colors.h>
#include <inout.h>
#include <string_util.h>
#include <test_util.h>

#define is_hexa(a) ( (((a) >= '0' && (a) <= '9') || ((a) >= 'a' && (a) <= 'f') || ((a) >= 'A' && (a) <= 'F')) ? 1 : 0 )

// no params
void help(int argc, char params[MAX_PARAMETERS][LENGTH_PARAMETERS]){
	if(argc!=0){
		do_print_color("\tayayay be more careful... ", RED);
		printf("help does not take any parameters\n");
	}
	// se corre el comando de todas formas
	
	const char* help_string = 
	"HELP                 Provides help information for commands.\n"
    "DIVIDEBYZERO         Command to verify the operation of the exception routine \"Divide by zero\"\n"
	"INVALIDOPCODE        Command to verify the operation of the exception routine \"Invalid Opcode\"\n"
	"INFOREG              Prints on screen the value of all registers.\n"
	"PRINTMEM             Receives as argument a pointer and performs a memory dump of 32 bytes from the\n"
	"                     Address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n"
	"CHANGEFONTSIZE       Changes font size: insert 1 2 3 for the desired level.\n"
	"TRON                 Get ready to play Tron!.\n"
	"CLEAR                Clear screen\n"
	"TEST_MM			  Tests the memory manager\n";
	printf(help_string);
}

//no params
void divide_by_zero(int argc, char  params[][LENGTH_PARAMETERS]){
	if(argc!=0){
		do_print_color("\tIf you are already going to divide by zero then try to not commit another error... ", RED);
		printf("'divide_by_zero' does not take any parameters\n");
		return;
	}
	do_division_by_zero();
}

//no params
void invalid_op_code(int argc, char  params[][LENGTH_PARAMETERS]){
	if(argc!=0){
		printf("\t'invalidopcode' does not take any parameters\n");
		return;
	}
	do_invalid_op_code();
}


//no params
void inforeg(int argc, char params[][LENGTH_PARAMETERS]){
	if(argc!=0){
		printf("\try inforeg without parameters\n");
		return;
	}
	do_print_inforeg();
	return;
}

char byte_hex_to_char(uint8_t value) {
	return value >= 10 ? (value - 10 + 'A') : (value + '0');
}
 
static uint64_t check_mem(char mem[], uint64_t * len){
    *len = _str_len(mem);
    //0x........ -> if it matches ^0x[a-fA-F0-9]{16}
    if (*len < 2 || mem[0] != '0' || mem[1] != 'x'){
        return 0;
    }

    for (int i = 2; i < *len; i++){
        if (!is_hexa(mem[i])){
            return 0;
        }
    }
	return 1;
}

//1 param: memDirec
void print_mem(int argc, char params[][LENGTH_PARAMETERS]){
	
	if(argc != 1){
		printf("\tTry print_mem with one parameter (mem_dir)\n");
		return;
	}

	uint64_t len;

	if (!check_mem(params[0], &len)){
		printf("\tRemember that memory addresses go with a ");
		do_print_color("0x ", CYAN);
		printf("in front.\n");
		return;
	}

	if (len == 2) {
		printf("\tWe'll check for memory: ");
		do_print_color("0x", DARKER_CYAN);
		do_print_color("00\n", CYAN);
	}

	// we store in mem the pointer to the first memory we want to print
	uint8_t * mem;
	if(!hex_str_to_int(params[0],&mem)){
		printf("Parameter is not a valid memory address. Try a smaller number\n");
		return;
	}
	// we print the 32 bytes that follow *mem
	for (int i = 0; i < 32; i++){
		if (i == 16) {
			printf("\n");
		}
		char  buffer[6] = "0x00 ";
		buffer[2] = byte_hex_to_char(mem[i] >> 4);	// first hex value
		buffer[3] = byte_hex_to_char(mem[i] & 0x0F);	
		do_print_color("0x", DARKER_CYAN);
		do_print_color(buffer+2, CYAN);
	}
	printf("\n");
	return;
}

//1 param: setting
void change_font_size(int argc, char params[][LENGTH_PARAMETERS]){
	int everything_ok = 1;
	if(argc != 1){
		printf("Try change font size with 1 parameter (setting)\n");
		return;
	}
	if (_str_len(params[0]) > 1){
		printf("Try with 1, 2 or 3\n");
		return;
	}
	if (params[0][0] == '1' || params[0][0] == '2' || params[0][0] == '3' ){
		if (params[0][0] == '3'){
			everything_ok = 0;
			printf("\tWarning, font size 3 is way too big!\n");
			printf("\tIf you still want to change the font size type 'y', otherwise type 'n'\n");
			char ans ;
			while(ans != 'y' && ans != 'n')
				ans = do_get_char();
			if(ans == 'y')
				everything_ok = 1;
		}
		if (everything_ok) do_change_font_size(params[0][0]-'0');
		
	}
	else{
		printf("\tInvalid parameter, try with 1, 2 or 3\n");
	}
	return;
}

//no params
void time(int argc, char params[][LENGTH_PARAMETERS]){
	if(argc!=0){
		printf("\tTry time without parameters\n");
		return;
	}
	printf("\tThis is the current ");
	do_print_color("time ", TURQUOISE);
	printf("in the beautiful city of ");
	do_print_color("GREENWICH: ", HAPPY_MATRIX);
	do_print_color("(UTC+00)\n", CYBER_GRAPE);
	char timeBuffer[9];
	do_get_time(timeBuffer);
	printf("\t\t\t");
	do_print_color(timeBuffer, ICTERINE);
	printf("\n");
	printf("\tBuenos Aires is UTC-03:00\n");

	return;
}

//no params
void tron(int argc, char params[][LENGTH_PARAMETERS]){
	// tron will run even though it has parameters (not a bug, a feature)
	/*if(argc!=0){
		printf("Try tron without parameters\n");
		
	}*/
	play_tron();
	return;
}


void clear_screen(int argc, char params[][LENGTH_PARAMETERS]) {
	do_clear_screen(BLACK);
	return;
}

void test_memory_manager(int argc, char params[][LENGTH_PARAMETERS]) {
	printf("Testeando memory manager..\n");
	test_mm();
	return;
}