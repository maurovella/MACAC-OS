#include <pipes.h>

#define MAX_PIPES 20
#define PIPE_SIZE 1024

typedef struct pipe {
    unsigned int pipe_id;

    int read_sem_id;
    int write_sem_id;

    unsigned int pipe_read_id;
    unsigned int pipe_write_id;

    uint8_t * pipe_buffer;
    unsigned int current_pipe_size;

    uint8_t eof;
} pipe;

static pipe pipes[MAX_PIPES];
static unsigned int pipes_count = 0;

/* utilizaremos las funciones memory_alloc para cuando creo el buffer de los pipes y memory_free para liberarla*/

int find_pipe_avaiable() {
    if (pipes_count >= MAX_PIPES) {
        return MAX_PIPES_REACHED;
    }
    
    uint8_t found = 0;
    int pipe_id = 10;

    for (int i = 0; i < MAX_PIPES && !found; i++) {
        if (pipes[i].pipe_id == pipe_id) {
            found = 1;
            pipe_id++;
        }
    }
    return pipe_id;
}

int create_pipe_available() {
    int id = find_pipe_avaiable();
    if (id == MAX_PIPES_REACHED) {
        return MAX_PIPES_REACHED;
    }

    if (create_pipe(id) == NO_SPACE_FOR_PIPE) {
        return NO_SPACE_FOR_PIPE;
    }

    return id;
}

int8_t create_pipe (unsigned int pipe_id) {
    if ( pipe_id <= 0 )
        return INVALID_PIPE_ID;
    if (pipes_count >= MAX_PIPES) {
        return MAX_PIPES_REACHED;
    }
    
    int free_pos = INVALID_PIPE_ID;
    int i = 0;
    while (i < MAX_PIPES && free_pos == INVALID_PIPE_ID) {
        if (pipes[i].pipe_id == 0) {
            free_pos = i;
        }
        if(pipes[i].pipe_id == pipe_id) {
            return INVALID_PIPE_ID;
        }
        i++;
    }

    int reading_sem_id = create_sem(0);
    int writing_sem_id = create_sem(PIPE_SIZE);
    if(reading_sem_id == INVALID_PIPE_ID || writing_sem_id == INVALID_PIPE_ID) {
        destroy_sem(reading_sem_id);
        destroy_sem(writing_sem_id);
        return NO_SPACE_FOR_PIPE;
    }
    // creo el buffer para el pipe asignado al pipe correspondiente
    pipes[free_pos].pipe_buffer = memory_alloc(PIPE_SIZE);
    if (pipes[free_pos].pipe_buffer == NULL) {
        return NO_SPACE_FOR_PIPE;
    }

    pipes[free_pos].pipe_id = pipe_id;
    pipes[free_pos].pipe_read_id = 0;
    pipes[free_pos].pipe_write_id = 0;
    pipes[free_pos].current_pipe_size = 0;
    pipes[free_pos].eof = 0;
    pipes[free_pos].read_sem_id = reading_sem_id;
    pipes[free_pos].write_sem_id = writing_sem_id;

    pipes_count++;

    return SUCCESS;
}

static find_pipe(unsigned int pipe_id) {
    int i = 0;
    while (i < MAX_PIPES) {
        if (pipes[i].pipe_id == pipe_id) {
            return i;
        }
        i++;
    }
    return INVALID_PIPE_ID;
}

int8_t delete_pipe(unsigned int pipe_id) {
    if (pipe_id <= 0) {
        return INVALID_PIPE_ID;
    }
    int pos = find_pipe(pipe_id);

    if (pos == INVALID_PIPE_ID) {
        return INVALID_PIPE_ID;
    }

    destroy_sem(pipes[pos].read_sem_id);
    destroy_sem(pipes[pos].write_sem_id);
    memory_free(pipes[pos].pipe_buffer);

    pipes[pos].pipe_id = 0;
    pipes[pos].pipe_read_id = 0;
    pipes[pos].pipe_write_id = 0;
    pipes[pos].current_pipe_size = 0;
    pipes[pos].eof = 0;
    pipes[pos].read_sem_id = 0;
    pipes[pos].write_sem_id = 0;

    return INVALID_PIPE_ID;
}

void set_eof(unsigned int pipe_id) {
    int pos = find_pipe(pipe_id);

    if(pos == INVALID_PIPE_ID)
        return;

    pipes[pos].eof = 1;
}

int write_pipe(unsigned int pipe_id, const char * buffer, unsigned int count) {
    int pos = find_pipe(pipe_id);
    if(pos == INVALID_PIPE_ID)
        return INVALID_PIPE_ID;

    int i = 0;
    while(i < count){
        wait_sem(pipes[pos].write_sem_id);
        pipes[pos].pipe_buffer[pipes[pos].pipe_write_id] = buffer[i];
        pipes[pos].pipe_write_id = (pipes[pos].pipe_write_id + 1) % PIPE_SIZE;
        pipes[pos].current_pipe_size++;
        i++;
        post_sem(pipes[pos].read_sem_id);
    }
    return i;
}

int read_pipe(unsigned int pipe_id, char * buffer, unsigned int count) {
    int pos = find_pipe(pipe_id);
    if(pos == INVALID_PIPE_ID)
        return INVALID_PIPE_ID;
    
    if(pipes[pos].eof && pipes[pos].current_pipe_size == 0)
        return 0;

    int i = 0;
    while(i < count && pipes[pos].current_pipe_size > 0){
        wait_sem(pipes[pos].read_sem_id);
        buffer[i] = pipes[pos].pipe_buffer[pipes[pos].pipe_read_id];
        pipes[pos].pipe_read_id = (pipes[pos].pipe_read_id + 1) % PIPE_SIZE;
        pipes[pos].current_pipe_size--;
        i++;
        post_sem(pipes[pos].write_sem_id);
    }
    return i;
}