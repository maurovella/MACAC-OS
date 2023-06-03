#include <shell.h>

#define NO_PARMAS_ERROR_MSG "Try without parameters\n"
#define ONE_PARAM_ERROR_MSG "Try with one parameter\n"

typedef struct program_info_CDT {
    char * name;
    uint64_t function_ptr; 		// function direction
    uint8_t min_args; 			// min args
    uint8_t max_args; 			// max args
    uint8_t pipeable; 			// 1 if can be piped, 0 if not
    char * params_error_msg ; 	// error msg if params are wrong
	uint8_t base_priority; 			// initial process priority
} program_info_CDT;

static int parse_buffer(char command[BUFFER_LENGTH], char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], char read_buffer[BUFFER_LENGTH]);
static int find_idx_command(char *buff);

static char* commands[] = {"help", "invalidopcode", "dividebyzero", "inforeg", "printmem", "time", "changefontsize", "tron", "clear", "testmm","testprio","testsc"};

static program_info_CDT programs[] = {
    {.name = "help", 			.function_ptr = &help, 				    .min_args = 0, 	.max_args = 0,  .pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "invalidopcode",   .function_ptr = &invalid_op_code,       .min_args = 0,  .max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "dividebyzero",    .function_ptr = &divide_by_zero,        .min_args = 0,  .max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "inforeg", 		.function_ptr = &inforeg, 			    .min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "printmem", 		.function_ptr = &print_mem, 		    .min_args = 2, 	.max_args = 2, 	.pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
    {.name = "time", 			.function_ptr = &time, 				    .min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "changefontsize",  .function_ptr = &change_font_size,  	.min_args = 1, 	.max_args = 1, 	.pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
    {.name = "tron", 			.function_ptr = &tron, 					.min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 0},
    {.name = "clear", 			.function_ptr = &clear_screen, 			.min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "testmm", 			.function_ptr = &test_memory_manager,   .min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "testprio", 		.function_ptr = &test_priority, 		.min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "testsc", 			.function_ptr = &test_scheduler, 		.min_args = 0, 	.max_args = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
};

static uint8_t validate_size(uint64_t size, program_info_ADT program){
	if(size < program->min_args || size > program->max_args ){
		do_print_color(program->params_error_msg, RED);
		return 0;
	}
	return 1;
}

void shell() {
    while(1){
		printf("$>");

		char buff_command[BUFFER_LENGTH] 	= {0};
		char command[BUFFER_LENGTH] 		= {0};
		char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS] = {{0}};

		buffer_action(buff_command, BUFFER_LENGTH); //sys_read de todo
		int size = parse_buffer(command, parameters, buff_command);
		int command_idx = find_idx_command(command);
		if(command_idx >= 0 ){
            if(validate_size(size, &(programs[command_idx]))) {
				sys_create_child_process(parameters, programs[command_idx].base_priority, STDIN, STDOUT, (uint64_t) programs[command_idx].function_ptr);
				sys_wait_for_children();
			}
		}
		else if (command_idx == -1){
			do_print_color("Command not found: try again\n", RED);
		}
	}
}

static int parse_buffer(char command[BUFFER_LENGTH], char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], char read_buffer[BUFFER_LENGTH]) {
	int i=0, j;

	while(i < BUFFER_LENGTH && read_buffer[i] == ' '){
		i++;
	}

	for(j = 0; i < BUFFER_LENGTH && read_buffer[i] != ' ' && read_buffer[i] != '\0'; i++){
		command[j++] = read_buffer[i];
	}

	int k = 0;
	command[j] = 0;
	while(i < BUFFER_LENGTH && read_buffer[i] == ' '){
		i++;
	}
	if (read_buffer[i]=='\0'){
		return k;
	}

	k = 1;
	for( j = 0 ; i < BUFFER_LENGTH; ) {
		if( k >= MAX_PARAMETERS || j >= LENGTH_PARAMETERS) return -1;
		//casos: a: estoy en un espacio => aumento k pues termino un param
		//		 b: estoy en una caracter => o es el ultimo o no
		// => si es el ultimo ++k y si no sigo leyendo
		if(read_buffer[i] != ' ') { //estoy en un caracter y hay un siguiente
			parameters[k-1][j++] = read_buffer[i++];
		}
		else {
			parameters[k-1][j] = 0;
			j=0;
			while(i<BUFFER_LENGTH && read_buffer[i]==' '){
				i++;
			}
			if (read_buffer[i]=='\0'){
				return k;
			}
			k++;
		}
	}
	return k;
}

static int find_idx_command(char *buff) {

	if (_str_len(buff) == 0) { //Me pasan un enter suelto
		return -2;            //Para diferenciar de command not found
	}
	
	for (int i = 0; i < COMMANDS_LENGTH ; i++){
		if (_str_cmp(buff, commands[i]) == 0){
			return i;
		}
	}
	return -1; //command not found
}