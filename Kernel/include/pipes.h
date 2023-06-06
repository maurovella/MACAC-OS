#ifndef _PIPES_H_
#define _PIPES_H_

#include <stdint.h>
#include <stddef.h>
#include <memory_manager.h>
#include <semaphore.h>

#define INVALID_PIPE_ID -1
#define MAX_PIPES_REACHED -2
#define NO_SPACE_FOR_PIPE -3

#define SUCCESS 0

int8_t create_pipe (unsigned int pipe_id);

int8_t delete_pipe (unsigned int pipe_id);

int create_pipe_available();

int write_pipe(unsigned int pipe_id, const char * buffer, unsigned int count);

int read_pipe (unsigned int pipe_id, char * buffer, unsigned int bytes);

#endif