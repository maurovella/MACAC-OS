#include <shell.h>
#include <string_util.h>
#include <stddef.h>

#define NO_PARMAS_ERROR_MSG "Try without parameters\n"
#define ONE_PARAM_ERROR_MSG "Try with one parameter\n"
#define TWO_PARAM_ERROR_MSG "Try with two parameters\n"
#define BACKGROUND 4

typedef struct program_info_CDT {
    char * name;
    uint64_t function_ptr; 		// function direction
    uint8_t args_qty; 				// number of args
    uint8_t pipeable; 			// 1 if can be piped, 0 if not
    char * params_error_msg ; 	// error msg if params are wrong
	uint8_t base_priority; 			// initial process priority
} program_info_CDT;

static int parse_buffer(char command[BUFFER_LENGTH], char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], char read_buffer[BUFFER_LENGTH]);
static int find_idx_command(char *buff);
void * mem_cpy(void * destination, const void * source, uint64_t length);

static char* commands[] = {"help", "invalidopcode", "dividebyzero", "inforeg", "printmem", "time", "changefontsize", "tron", "clear", "testmm","testprio","testsc","ps","loop","cat","wc","filter","kill","nice","block","phylo","mem","pipecommands","thanks", "testsync", "ps", "kill", "block", "loop", "nice", "cat", "wc", "filter", "phylo"};

static program_info_CDT programs[] = {
    {.name = "help", 			.function_ptr = (uint64_t) &help, 				.args_qty = 0,	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "invalidopcode",   .function_ptr = (uint64_t) &invalid_op_code,    .args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "dividebyzero",    .function_ptr = (uint64_t) &divide_by_zero,     .args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "inforeg", 		.function_ptr = (uint64_t) &inforeg, 			.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "printmem", 		.function_ptr = (uint64_t) &print_mem, 		    .args_qty = 1, 	.pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
    {.name = "time", 			.function_ptr = (uint64_t) &time, 				.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "changefontsize",  .function_ptr = (uint64_t) &change_font_size,  	.args_qty = 1, 	.pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
    {.name = "tron", 			.function_ptr = (uint64_t) &tron, 				.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "clear", 			.function_ptr = (uint64_t) &clear_screen, 		.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "testmm", 			.function_ptr = (uint64_t) &test_memory_manager,.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
    {.name = "testprio", 		.function_ptr = (uint64_t) &test_priority, 		.args_qty = 0, 	.pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 1},
    {.name = "testsc", 			.function_ptr = (uint64_t) &test_scheduler, 	.args_qty = 0, 	.pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 1},
	{.name = "ps",				.function_ptr = (uint64_t) &ps,					.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "loop", 			.function_ptr = (uint64_t) &loop,				.args_qty = 0,  .pipeable = 0 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "cat", 			.function_ptr = (uint64_t) &cat,				.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "wc", 				.function_ptr = (uint64_t) &wc,					.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "filter", 			.function_ptr = (uint64_t) &filter,				.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "kill",			.function_ptr = (uint64_t) &kill,				.args_qty = 1,  .pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
	{.name = "nice",			.function_ptr = (uint64_t) &nice,				.args_qty = 2,  .pipeable = 1 , .params_error_msg = TWO_PARAM_ERROR_MSG , .base_priority = 5},
	{.name = "block",			.function_ptr = (uint64_t) &block,				.args_qty = 1,  .pipeable = 0 , .params_error_msg = ONE_PARAM_ERROR_MSG , .base_priority = 5},
	{.name = "phylo",			.function_ptr = (uint64_t) &phylo,				.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "mem",				.function_ptr = (uint64_t) &mem,				.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "pipecommands",	.function_ptr = (uint64_t) &pipe_commands,		.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "thanks",			.function_ptr = (uint64_t) &thanks,				.args_qty = 0,  .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5},
	{.name = "testsync", 		.function_ptr = (uint64_t) &test_synchronization,.args_qty = 0, .pipeable = 1 , .params_error_msg = NO_PARMAS_ERROR_MSG , .base_priority = 5}
};
static uint8_t validate_num_of_params(uint64_t params_size, program_info_ADT program, char ** params){
	if(params_size == program->args_qty+1 && params[params_size -1][0] == '&' && params[params_size -1][1] == '\0') {
		return BACKGROUND;
	}
	if(params_size != program->args_qty) {
		do_print_color(program->params_error_msg, RED);
		return 0;
	}
	return 1;
}


void * mem_cpy(void * destination, const void * source, uint64_t length)	{
	/*
	* mem_cpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

static char ** prepare_parameters(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], uint64_t size, char* name){
	void * params_alloc = (void *) sys_malloc((size + 2 ) * sizeof(char *));
	if(params_alloc == NULL){
		do_print_color("Error allocating memory for parameters\n", RED);
		return NULL;
	}
	char ** params = (char **) params_alloc;

	void * param_alloc;
	int param_size;

	for(int i = 0; i < size; i++){
		param_size = _str_len(parameters[i]) + 1;
		param_alloc = (void *)sys_malloc(param_size);
		if(param_alloc == NULL){
			do_print_color("Error allocating memory for parameters\n", RED);
			return NULL;
		}

		char * param = (char *) param_alloc;
		mem_cpy(param, parameters[i], param_size);
		params[i] = param;
	}
	void * name_alloc = (void *) sys_malloc(_str_len(name) + 1);

	if(name_alloc == NULL){
		do_print_color("Error allocating memory for parameters\n", RED);
		return NULL;
	}
	char * name_param = (char *) name_alloc;
	mem_cpy(name_param, name, _str_len(name) + 1);
	params[size++] = name_param;
	params[size] = NULL;
	return params;
}

int piped_process_handle(char ** params, unsigned int params_size) {
	if(params_size != 2 || params[0][0] != '|' || params[0][1] != '\0') {
		return 0;
	}
	int program_1 = find_idx_command(params[1]);
	int program_2 = find_idx_command(params[2]);

	if(program_1 < 0 || program_2 < 0) {
		do_print_color("Command not found, write \"help\" for more information\n", RED);
		return -1;
	}
	if(!programs[program_1].pipeable) {
		do_print_color("Command not pipeable, write \"pipcommands\" for more information\n", RED);
		return -1;
	}

	int pipe_id = sys_create_pipe_available();

	do_print_color("Pipe created\n", GREEN);

	if(pipe_id <= 0) {
		do_print_color("Error creating pipe\n", RED);
		return -1;
	}

	sys_create_child_process((char **) params[1], programs[program_1].base_priority, STDIN, pipe_id, programs[program_1].function_ptr);
	do_print_color("First process created\n", GREEN);
	sys_create_child_process((char **) params[2], programs[program_2].base_priority, pipe_id, STDOUT, programs[program_2].function_ptr);
	do_print_color("Second process created\n", GREEN);


	do_print_color("Waiting for children\n", GREEN);
	sys_wait_for_children();
	do_print_color("Children finished\n", GREEN);

	sys_delete_pipe(pipe_id);
	do_print_color("Pipe deleted\n", GREEN);

	return 1;
}

void normal_process_handle(char ** params, unsigned int params_size, int command_idx) {
	int is_valid;
	if( (is_valid = validate_num_of_params(params_size, &programs[command_idx],params)) == 0) {
		return;
	}

	if(is_valid == BACKGROUND) {
		params[params_size - 1] = NULL;
		sys_create_process(params, programs[command_idx].base_priority, STDIN, BACKGROUND, programs[command_idx].function_ptr);
	} else {
		int pid = sys_create_child_process(params, programs[command_idx].base_priority, STDIN, STDOUT, programs[command_idx].function_ptr);
		if(pid < 0) {
			do_print_color("Error creating process\n", RED);
			return;
		}
		sys_wait_for_children();
	}
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
			/*if( params[0][0] == '|' && params[0][1] == '\0' ) {
				do_print_color("Pipeing\n", RED);
			}*/
            //if(validate_size(size, &(programs[command_idx]))) {
				char ** params = prepare_parameters(parameters, size, command);
				if(piped_process_handle(params,size) == 0){
					normal_process_handle(params, size ,command_idx);
				}
				//sys_create_child_process(params, programs[command_idx].base_priority, STDIN, STDOUT, (uint64_t) programs[command_idx].function_ptr);
				//sys_wait_for_children();
			//}
		}
		else if (command_idx == -1){
			do_print_color("Command not found, write \"help\" for more information\n", RED);
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
