// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>

typedef struct child_info {
    uint32_t parent_pid;
    uint32_t child_pid;
    uint8_t state;
} child_info;

child_info children_processes[MAX_CHILDREN_PROCESSES] = {{0}};

uint8_t all_children_finished(uint32_t parent_pid) {
    for (int i = 0; i < MAX_CHILDREN_PROCESSES; i++) {
        if (children_processes[i].state == READY && children_processes[i].parent_pid == parent_pid) {
            return FALSE;
        }
    }
    return TRUE;
}

void delete_children(uint32_t parent_pid) {
    for(int i=0 ; i<MAX_CHILDREN_PROCESSES; i++){
		if(children_processes[i].parent_pid == parent_pid){
			children_processes[i].parent_pid = 0;
			children_processes[i].child_pid = 0;
			children_processes[i].state = DEAD;
		}
	}
}

void set_dead_process(uint32_t pid) {
    for(int i=0 ; i<MAX_CHILDREN_PROCESSES; i++){
		if( children_processes[i].state == READY && children_processes[i].child_pid == pid){
			children_processes[i].state = DEAD;


			// Check if siblings are alive. If they're all dead wake up parent.

			unsigned int father = children_processes[i].parent_pid;
			if(all_children_finished(father)){
				delete_children(father);
				change_process_state(father, READY);
			}

			return;
		}
	}
}

void add_child(uint32_t parent_pid, uint32_t child_pid) {
    for (int i = 0; i < MAX_CHILDREN_PROCESSES; i++) {
        if (children_processes[i].state == DEAD) {
            children_processes[i].parent_pid = parent_pid;
            children_processes[i].child_pid = child_pid;
            children_processes[i].state = READY;
            return;
        }
    }
}

int32_t create_child_process(char ** params, uint8_t priority, uint8_t input, uint8_t output, uint64_t entry_point) {
    int32_t child_pid = create_process(params, priority, input, output, FALSE, entry_point);

    add_child(get_pid(), child_pid);

    return child_pid;
}

uint8_t has_children(unsigned int pid){
	for(int i=0; i<MAX_CHILDREN_PROCESSES; i++){
		if(children_processes[i].parent_pid == pid){
			return 1;
		}
	}
	return 0;
}

void wait_for_children() {
    uint32_t pid = get_pid();
    if (has_children(pid)) {
        change_process_state(pid, PAUSED_BY_CHILD);
        force_timer_tick();
    }
}
