#ifndef FORK_H
# define FORK_H
# include <pthread.h>

pthread_mutex_t *create_forks(int num_forks);

#endif