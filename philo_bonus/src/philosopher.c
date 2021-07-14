#include <philosopher.h>
#include <stdlib.h>
#include <time_utils.h>
#include <death.h>

t_philo	*create_philosophers(t_philo_config *config)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	if (!philosophers)
		return (NULL);
	config->time_start = get_timestamp();
	i = 0;
	while (i < num_philosophers)
	{
		setup_philosopher(&(philosophers[i]), config, i + 1);
		++i;
	}
	return (philosophers);
}

void	destroy(t_philo *philosophers)
{
	free(philosophers);
}

void	setup_philosopher(t_philo *philosopher, \
								t_philo_config *config, \
								int id)
{
	philosopher->config = config;
	philosopher->id = id;
	philosopher->meals_counter = 0;
	philosopher->last_meal.value = 0;
}

t_bool	has_finished_min_meals(const t_philo *philo)
{
	return (philo->meals_counter >= philo->config->min_meals \
			&& philo->config->min_meals != NOT_SET);
}
