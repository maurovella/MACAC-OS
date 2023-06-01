#include <pipes.h>

#define MAX_PIPES 20
#define PIPE_SIZE 1024

typedef struct pipe {
    unsigned int pipe_id;

    unsigned int pipe_read_idx;
    unsigned int pipe_write_idx;

    uint8_t * pipe_buffer;
    unsigned int current_pipe_size;

    uint8_t eof;
} pipe;

static pipe pipes[MAX_PIPES];
static unsigned int pipes_count = 0;

/* utilizaremos las funciones memory_alloc para cuando creo el buffer de los pipes y memory_free para liberarla*/

create_pipe (unsigned int pipe_id) {
    if ( pipe_id <= 0 )
        return INVALID_PIPE_ID;
    if (pipe_count >= MAX_PIPES) {
        return MAX_PIPES_REACHED;
    }
    
    int free_pos = INVALID_PIPE_ID;
    int i = 0;
    while (i < MAX_PIPES && free_pos == INVALID_PIPE_ID) {
        if (pipe_info[i].pipe_id == 0) {
            free_pos = i;
        }
        if(pipe_info[i].pipe_id == pipe_id) {
            return INVALID_PIPE_ID;
        }
        i++;
    }

    // creo el buffer para el pipe asignado al pipe correspondiente
    pipes[free_pos].pipe_buffer = memory_alloc(PIPE_SIZE);
    if (pipes[free_pos].pipe_buffer == NULL) {
        return NO_SPACE_FOR_PIPE;
    }

    pipes[free_pos].pipe_id = pipe_id;
    pipes[free_pos].pipe_read_idx = 0;
    pipes[free_pos].pipe_write_idx = 0;
    pipes[free_pos].current_pipe_size = 0;
    pipes[free_pos].eof = 0;

    pipes_count++;

    return SUCCES;
}

delete_pipe(unsigned int pipe_id) {
    if (pipe_id <= 0) {
        return INVALID_PIPE_ID;
    }

    int pos = 0;
    while (i < MAX_PIPES) {
        if (pipe_info[pos].pipe_id == pipe_id) {
            memory_free(pipe_info[pos].pipe_buffer);
            pipe_info[pos].pipe_id = 0;
            pipe_info[pos].pipe_read_idx = 0;
            pipe_info[pos].pipe_write_idx = 0;
            pipe_info[pos].current_pipe_size = 0;
            pipe_info[pos].eof = 0;
            pipes_count--;
            return SUCCES;
        }
        pos++;
    }
    return INVALID_PIPE_ID;
}