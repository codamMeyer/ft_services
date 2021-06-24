#ifndef FORK_H
# define FORK_H
# include <pthread.h>
# include <types.h>

t_fork	*create_forks(int num_forks);
void	destroy_forks(t_fork *forks, int num_forks);

#endif