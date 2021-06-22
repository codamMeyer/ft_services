#include <fork.h>
#include <stdlib.h>

void	cleanup_forks(t_fork *forks, int num_forks)
{
	int	i;

	if (!forks)
		return ;
	i = 0;
	while (i < num_forks)
	{
		free(forks[i].lock);
		++i;
	}
	free(forks);
}

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
			cleanup_forks(forks, i);
			return (NULL);
		}
		++i;
	}
	return (forks);
}
