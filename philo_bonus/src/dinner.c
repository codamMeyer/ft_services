#include <stdlib.h>
#include <dinner.h>
#include <time_utils.h>

static void	cleanup(t_philo *philosophers)
{
	free(philosophers);
}

t_philo	*create_philosophers(t_philo_config *config)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < num_philosophers)
	{
		philosophers[i].config = config;
		philosophers[i].id = i + 1;
		++i;
	}
	return (philosophers);
}

static t_status	malloc_resources(t_philo_config *config,
								t_philo **philosophers)
{
	*philosophers = create_philosophers(config);
	if (!(*philosophers))
		return (ERROR);
	return (SUCCESS);
}

t_status	run(t_philo_config *config)
{
	t_philo		*philosophers;
	t_status	ret;

	ret = SUCCESS;
	philosophers = NULL;
	if (malloc_resources(config, &philosophers) == ERROR)
		return (ERROR);
	if (config->death_event)
		ret = DEATH_EVENT;
	cleanup(philosophers);
	return (ret);
}
