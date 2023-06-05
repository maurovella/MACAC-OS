#include <stdint.h>
#include <commands.h>
#include <tron.h>
#include <colors.h>
#include <inout.h>
#include <string_util.h>
#include <test_util.h>
#include <stddef.h>
#include <commands_utils.h>

#define is_hexa(a) ( (((a) >= '0' && (a) <= '9') || ((a) >= 'a' && (a) <= 'f') || ((a) >= 'A' && (a) <= 'F')) ? 1 : 0 )

// no params
void help(){
	/*if(argc!=0){
		do_print_color("\tayayay be more careful... ", RED);
		printf("help does not take any parameters\n");
	}*/
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
	"TESTMM               Test memory manager\n"
	"TESTPRIO             Test prioritys\n"
	"TESTSC          	  Test scheduler\n"
	"TESTSYNC			  Test synchronization\n"
	"PS                   Prints all the processes\n"
	"LOOP                 Prints \"in loop\" every five seconds\n"
	"KILL 			   	  Receives an especific pid and kills it\n"
	"NICE 				  Receives an especific pid and a priority and changes the priority of the process\n"
	"BLOCK 				  Receives an especific pid and change it states to block or ready depending on the current state\n"
	"CAT 				  Prints stdin content\n"
	"WC 				  Prints the number of words in stdin\n"
	"FILTER 			  Prints stdin content without vocals\n"
	"PHYLO 				  Prints the phylosophers problem\n";
	printf(help_string);
}

//no params
void divide_by_zero(){
	/*if(argc!=0){
		do_print_color("\tIf you are already going to divide by zero then try to not commit another error... ", RED);
		printf("'divide_by_zero' does not take any parameters\n");
		return;
	}*/
	do_division_by_zero();
}

//no params
void invalid_op_code(){
	/*if(argc!=0){
		printf("\t'invalidopcode' does not take any parameters\n");
		return;
	}*/
	do_invalid_op_code();
}


//no params
void inforeg(){
	/*if(argc!=0){
		printf("\try inforeg without parameters\n");
		return;
	}*/
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
void print_mem(char** params){
	uint64_t len;

	if (!check_mem(params[0], &len)){
		do_print_color("\tRemember that memory addresses go with a ",RED);
		do_print_color("0x ", CYAN);
		do_print_color("in front.\n",RED);
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
		do_print_color("Parameter is not a valid memory address. Try a smaller number\n",RED);
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
void change_font_size(char ** params){
	int everything_ok = 1;
	/*if(argc != 1){
		printf("Try change font size with 1 parameter (setting)\n");
		return;
	}*/
	if (_str_len(params[0]) > 1){
		do_print_color("Try with 1, 2 or 3\n",RED);
		return;
	}
	if (params[0][0] == '1' || params[0][0] == '2' || params[0][0] == '3' ){
		if (params[0][0] == '3'){
			everything_ok = 0;
			do_print_color("\tWarning, font size 3 is way too big!\n",RED);
			do_print_color("\tIf you still want to change the font size type 'y', otherwise type 'n'\n",WHITE);
			char ans ;
			while(ans != 'y' && ans != 'n')
				ans = do_get_char();
			if(ans == 'y')
				everything_ok = 1;
		}
		if (everything_ok) do_change_font_size(params[0][0]-'0');
		
	}
	else{
		do_print_color("\tInvalid parameter, try with 1, 2 or 3\n",RED);
	}
	return;
}

//no params
void time(){
	/*if(argc!=0){
		printf("\tTry time without parameters\n");
		return;
	}*/
	do_print_color("\tThis is the current ",WHITE);
	do_print_color("time ", TURQUOISE);
	do_print_color("in the beautiful city of ",WHITE);
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
void tron(){
	// tron will run even though it has parameters (not a bug, a feature)
	/*if(argc!=0){
		printf("Try tron without parameters\n");
		
	}*/
	play_tron();
	return;
}


void clear_screen() {
	do_clear_screen(BLACK);
	return;
}

void test_memory_manager() {
	printf("Testeando memory manager..\n");
	test_mm();
	return;
}

void test_synchronization() {
	printf("Testeando sincronizacion..\n");
	test_sync();
	return;
}

void ps() {
	printf("ps");
	return;
}



void loop() {
	//imprimo el pid del proceso que ejecuta el loop y luego "in loop"
	printf("I'm the process with pid: ");
	printf("\n");
	while(1) {
		printf("in loop\n");
		wait(50000);
	}

	return;
}

void kill(char ** params) {
	//do_kill(params[0]);
	return;
}

void nice(char ** params) {
	//do_nice(params[0], params[1]);
	return;
}

void block(char ** params) {
	//do_block(params[0]);
	return;
}

void cat() {
	printf("\n");
	return;
}

void wc() {
	//do_wc(params[0]);
	return;
}

void filter() {
	//do_filter(params[0]);
	return;
}

void phylo() {
	//do_phylo();
	return;
}
void test_scheduler() {
	printf("Testeando scheduler..\n");
	test_processes();
	return;
}

void test_priority() {
	printf("Testeando priority..\n");
	test_prio();
	return;
}