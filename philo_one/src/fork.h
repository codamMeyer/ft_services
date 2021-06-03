#ifndef FORK_H
# define FORK_H
# include <pthread.h>
# include <types.h>

typedef struct s_fork
{
	pthread_mutex_t	*lock;
	t_bool			is_taken;
}	t_fork;

t_fork	*create_forks(int num_forks);

#endif