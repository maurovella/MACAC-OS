#include <stdint.h>
#include <commands.h>
#include <tron.h>
#include <colors.h>
#include <inout.h>
#include <string_util.h>
#include <test_util.h>
#include <stddef.h>
#include <commands_utils.h>
#include <data_types.h>

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
	"MEM	              Prints HEAP memory status.\n"
	"                     Address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n"
	"CHANGEFONTSIZE       Changes font size: insert 1 2 3 for the desired level.\n"
	"TRON                 Get ready to play Tron!.\n"
	"CLEAR                Clear screen\n"
	"TESTMM               Test memory manager\n"
	"TESTPRIO             Test prioritys\n"
	"TESTSC          	  Test scheduler\n"
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
/*
typedef struct process_info {
	char * name;
	uint32_t pid;
	uint8_t priority;
	uint8_t state;
	void * stack_start;
	void * stack_end;
	uint64_t stack_pointer;
	uint8_t output;
	uint32_t assigned_ticks;
} process_info;*/

int calculate_max_len(process_info * processes, uint8_t size) {
	int max_len = 0;
	for (int i = 0; i < size; i++) {
		int len = _str_len(processes[i].name);
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}

void ps() {
	process_info * processes = sys_malloc(sizeof(process_info *));
	uint8_t size = sys_get_all_processes(processes);
	int max_len = calculate_max_len(processes, size);
	



	printf("NAME\t|\tPID\t|\tPRIORITY\t|\tSTATE\t|\tSTACK START\t|\tSTACK END\t|\tSTACK POINTER\t|\tOUTPUT\t|\tASSIGNED_TICKS\n");
	printf("--------------------------------------------------------------------------------------------------\n");


	for (int i = 0; i < size; i++) {
		int len = _str_len(processes[i].name);
		for (int j = 0; j < max_len - len; j++) {
			processes[i].name[len + j] = ' ';
		}
		processes[i].name[max_len] = '\0';
		char * state_name;
		switch (processes[i].state)
		{
		case 0:
			state_name = "DEAD";
			break;
		case 1:
			state_name = "BLOCK";
			break;
		case 2:
			state_name = "READY";
			break;
		case 3:
			state_name = "RUNNING";
			break;
		case 4:
			state_name = "PAUSED";
			break;
		case 5:
			state_name = "PAUSED_BY_CHILD";
			break;
		case 6:
			state_name = "PAUSED_BY_SEM";
			break;
		default:
			break;
		}

		char * output_name;
		switch (processes[i].output)
		{
		case 1:
			output_name = "SCREEN";
			break;
		case 4:
			output_name = "BACKGROUND";
			break;	
		default: 
			output_name = "ELSE";
			break;
		}
		printf("%s\t|\t%d\t|\t%d\t\t|\t%s\t|\t%x\t|\t%x\t|\t%x\t|\t%s\t|\t%d\n", processes[i].name, processes[i].pid, processes[i].priority, state_name, processes[i].stack_start, processes[i].stack_end, processes[i].stack_pointer, output_name, processes[i].assigned_ticks);
	}

	sys_free(processes);
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
	uint32_t pid = atoi(params[0]);
	uint32_t deleted_pid = sys_kill_process(pid);
	printf("Process with pid %d was deleted\n", deleted_pid);
	return;
}

void nice(char ** params) {
	uint32_t pid = atoi(params[0]);
	uint32_t new_priority = atoi(params[1]);
	sys_nice(pid, new_priority);
	return;
}

void block(char ** params) {
	uint32_t pid = atoi(params[0]);
	uint32_t changed_process = sys_block_or_unblock_process(pid);
	printf("Process with pid %d was blocked\n", changed_process);
	return;
}

void get_buffer(char *buffer, char delim) {
	char c;
	int letter = 0;
	printf("  ");
	while (c != delim) {
		c = do_get_char();
		if (IS_ALLOWED_CHAR(c)) { 
			printf("%c", c);
			buffer[letter++] = c;
		}
		if (c == 0x7F && letter > 0) {
			letter--;
			buffer[letter] = '\0';
			sys_delete_last_char();
		}
		if (delim != '\n' && c == '\n') {
		printf("  ");
	}
	}

	

	buffer[letter] = '\0';
}

void cat() {
	char buffer[MAX_BUFFER];
	get_buffer(buffer,'\n');

	// imprimo contenido de buffer con do_print_color en un formato amigable
	printf("  ");
	do_print_color(buffer, CYAN);
}

void wc() {
	char buffer[MAX_BUFFER];
	get_buffer(buffer, 'U');
	int letter = 0;
	int words = 0;
	int lines = 0;
	int in_word = 0;

	for (int i = 0; buffer[i] != '\0'; i++) {
		// Contar palabras
		if (buffer[i] != ' ' && buffer[i] != '\n') {
			if (!in_word) {
				in_word = 1;
				words++;
			}
		} else {
			in_word = 0;
		}

		// Contar líneas
		if (buffer[i] == '\n') {
			lines++;
		}

		// Contar caracteres
		letter++;
	}

	// Imprimir resultados en formato amigable con do_print_color en color cian
	printf("\n");
	printf("  ");
	do_print_color("Words: ", CYAN);
	printf("%d\n", words);
	printf("  ");
	do_print_color("Lines: ", CYAN);
	printf("%d\n", lines);
	printf("  ");
	do_print_color("Characters: ", CYAN);
	printf("%d\n", letter);
}

void filter() {
	char buffer[MAX_BUFFER];
	get_buffer(buffer, '\n');
	char no_vowels[MAX_BUFFER];
	int letter = 0;
	printf("  ");
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (IS_NOT_VOWEL(buffer[i])) {
			no_vowels[letter++] = buffer[i];
		}
	}
	no_vowels[letter] = '\0';

	// Imprimir contenido de no_vowels con do_print_color en un formato amigable
	printf("  ");
	do_print_color(no_vowels, CYAN);
}

void phylo() {
	
	//do_phylo();
	return;
}

void mem() {
	uint64_t mem_info[4]; // 0: total_bytes 1: allocated_bytes 2: free_bytes 3: allocated_blocks
	sys_get_mem_info(mem_info);
	printf("Total bytes:      %d\n", mem_info[0]);
	printf("Allocated bytes:  %d\n", mem_info[1]);
	printf("Free bytes:       %d\n", mem_info[2]);
	printf("Allocated blocks: %d\n", mem_info[3]);
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

void pipe_commands() {
	printf("List of pipeable commands:\n");
	printf("  cat\n");
	printf("  wc\n");
	printf("  filter\n");
	printf("  phylo\n");
	printf("  mem\n");
	printf("  test_scheduler\n");
	printf("  test_priority\n");
	return;
}

void thanks() {
	do_print_color("You are welcome!\n", CYAN);
}