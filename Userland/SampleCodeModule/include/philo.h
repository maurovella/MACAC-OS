#ifndef _PHILO_H_
#define _PHILO_H_

#include <inout.h>
#include <syscalls.h>
#include <stdint.h>
#include <string_util.h>

#define MAX_PHYLOS 10
#define MIN_PHYLOS 5
#define LEFT    (i + curr_philos - 1) % curr_philos
#define RIGHT   (i + 1) % curr_philos
#define THINKING    0
#define HUNGRY      1
#define EATING      2

#define TRUE 1
#define FALSE (!TRUE)

#define MUTEX 123
#define PRINT_MUTEX 456

void do_philo();

#endif