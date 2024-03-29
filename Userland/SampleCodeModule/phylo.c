// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/philo.h"

typedef int sem_t;
int end = FALSE;

int state[MAX_PHYLOS] = { 0 };
sem_t s[MAX_PHYLOS] = { 0 };
sem_t safe[MAX_PHYLOS] = { 0 };
int pids[MAX_PHYLOS] = { 0 };
int curr_philos = 0;

static void philosopher(char ** num);
static void take_forks(int i);
static void add_philo();
static void remove_philo();
static void put_forks(int i);
static void test(int i);
static void eat();
static void think();

void do_philo() {
    end = FALSE;
    curr_philos = 0;
    sys_close_sem(MUTEX);
    sys_close_sem(PRINT_MUTEX);
    sys_create_sem_by_id(MUTEX, 1);
    sys_create_sem_by_id(PRINT_MUTEX, 1);

    for(int i = 0 ; i < MIN_PHYLOS; i++){
        add_philo();
    }

    char action_buffer[1];
    while(!end){
        sys_read(STDIN, action_buffer, 1);
        switch(action_buffer[0]){
            case 'a':
                add_philo();
                break;
            case 'r':
                remove_philo();
                break;
            case 'q':
                end = TRUE;
                break;
        }
    }

    for(int i = 0 ; i < curr_philos; i++){
        sys_kill_process(pids[i]);
        sys_close_sem(s[i]);
        sys_close_sem(safe[i]);
    }

    sys_close_sem(MUTEX);
    sys_close_sem(PRINT_MUTEX);
}

static void add_philo() {
    sys_wait_sem(MUTEX);
    if(curr_philos == MAX_PHYLOS){
        printf("Maximun number of phylosofers reached\n");
    } else {
        state[curr_philos] = THINKING;
        s[curr_philos] = sys_create_sem(0);
        safe[curr_philos] = sys_create_sem(1);

        char philosopher_string[11] = { "philosopher" };
        char ** philos = { 0 };

        char ** args = (char **) sys_malloc(3 * sizeof(char *));
        char * arg_buffer = (char *) sys_malloc(8);
        itoa(curr_philos, arg_buffer);

        _str_cpy(args[0], philosopher_string, 12);
        args[1] = arg_buffer;
        philos = args;
        pids[curr_philos] = sys_create_child_process(philos, 3, STDIN, STDOUT, (uint64_t) &philosopher);
        if( pids[curr_philos] <= 0) {
            printf("ERROR: creating philosophers.\n");
            return;
        }

        curr_philos++;
    }
    sys_post_sem(MUTEX);
}

static void remove_philo(){
    if(curr_philos == MIN_PHYLOS){
        printf("Minimun number of phylosofers reached\n");
        return;
    }

    sys_wait_sem(safe[curr_philos-1]);
    sys_wait_sem(MUTEX);

    curr_philos--;
    sys_close_sem(safe[curr_philos]);
    sys_kill_process(pids[curr_philos]);
    sys_close_sem(s[curr_philos]);
    
    sys_post_sem(MUTEX);
}

static void philosopher(char ** num) {
    int i = atoi(num[1]);
    while(!end) {
        sys_wait_sem(safe[i]);
        think();
        take_forks(i);
        eat();
        put_forks(i);
        sys_post_sem(safe[i]);
    }
}

static void take_forks(int i) {
    sys_wait_sem(MUTEX);
    state[i] = HUNGRY;
    test(i);
    sys_post_sem(MUTEX);
    sys_wait_sem(s[i]);
}

static void put_forks(int i) {
    sys_wait_sem(MUTEX);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    sys_post_sem(MUTEX);
}

static void test(int i) {
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        sys_post_sem(s[i]);
    }
}

static void eat() {
    for(int i = 0; i < 5000000; i++)
        ;
    sys_wait_sem(PRINT_MUTEX);
    for(int i = 0; i < curr_philos; i++) {
        printf(state[i] == EATING? "E " : ". ");
    }
    printf("\n");
    sys_post_sem(PRINT_MUTEX);
}

static void think() {
    for(int i = 0; i < 5000000; i++)
        ;
}
