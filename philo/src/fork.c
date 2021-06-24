#include <fork.h>
#include <stdlib.h>

t_fork	*create_forks(int num_forks)
{
	t_fork	*forks;
	int		i;

	forks = malloc(sizeof(t_fork) * num_forks);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < num_forks)
	{
		forks[i].lock = malloc(sizeof(pthread_mutex_t));
		forks[i].is_taken = FALSE;

		if (!forks[i].lock || pthread_mutex_init(forks[i].lock, NULL) != SUCCESS)
		{
			destroy_forks(forks, i);
			return (NULL);
		}
		++i;
	}
	return (forks);
}

static void	destroy_forks_mutexes(t_fork *forks, int num_forks)
{
	int	i;

	i = 0;
	if (!forks)
		return ;
	while (i < num_forks)
	{
		pthread_mutex_destroy(forks[i].lock);
		++i;
	}
}

void	destroy_forks(t_fork *forks, int num_forks)
{
	int	i;

	i = 0;
	if (!forks)
		return ;
	destroy_forks_mutexes(forks, num_forks);
	while (i < num_forks)
	{
		free(forks[i].lock);
		++i;
	}
	free(forks);
}
