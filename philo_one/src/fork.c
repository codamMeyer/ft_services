#include <fork.h>
#include <stdlib.h>

void	cleanup_forks(t_fork *forks, int num_forks)
{
	int	i;

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
		forks->is_taken = FALSE;
		if (pthread_mutex_init(forks[i].lock, NULL))
		{
			cleanup_forks(forks, num_forks);
			return (NULL);
		}
		++i;
	}
	return (forks);
}
