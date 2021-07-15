#include "types.h"
#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <display.h>
#include <time_utils.h>

static void	setup_philosopher(t_philo *philosopher, \
							int id, \
							t_display *display, \
							t_philo_config *config)
{
	philosopher->id = id;
	philosopher->last_meal.value = 0;
	philosopher->meals_counter = 0;
	philosopher->display = display;
	philosopher->config = config;
}

static void	assign_forks(t_philo *philosopher, \
						t_fork *forks, \
						int i, \
						int last_philo)
{
	philosopher->forks.right = &forks[i];
	if (i == 0)
		philosopher->forks.left = &forks[last_philo];
	else
		philosopher->forks.left = &forks[i - 1];
}

t_philo	*create_philosophers(t_philo_config *config, \
							t_fork *forks, \
							t_display *display)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	if (!forks || !philosophers)
		return (NULL);
	i = 0;
	while (i < num_philosophers)
	{
		setup_philosopher(&philosophers[i], i + 1, display, config);
		assign_forks(&philosophers[i], forks, i, num_philosophers - 1);
		++i;
	}
	return (philosophers);
}

void	*start_dinner(void *philo)
{
	const t_time_ms	wait_time = {.value = 1500};
	t_time_ms		timestamp;
	t_philo			*philosopher;

	philosopher = (t_philo *)philo;
	if (philosopher->id % 2)
		sleep_ms(wait_time);
	while (!is_dinner_over(philosopher))
	{
		if (get_forks(philosopher))
		{
			start_to_eat(philosopher);
			if (start_to_sleep(philosopher) == DEAD)
			{
				timestamp = \
					get_timestamp_diff((philosopher)->config->time_start);
				display_action_message(timestamp.value, (philosopher), DIED);
				break ;
			}
			start_to_think(philosopher);
		}
	}
	return (NULL);
}

t_status	create_philosophers_threads(t_philo *philosophers, \
										t_pthread_create create_thread)
{
	const int	num_philosophers = philosophers->config->number_of_philosophers;
	int			ret;
	int			i;

	i = 0;
	philosophers->config->time_start = get_timestamp();
	while (i < num_philosophers)
	{
		ret = create_thread(&(philosophers[i].thread_id), \
							NULL, &start_dinner, &philosophers[i]);
		if (ret)
		{
			philosophers->config->death_event = TRUE;
			philosophers->config->initialized_threads = i;
			return (ERROR);
		}
		++i;
	}
	philosophers->config->initialized_threads = num_philosophers;
	philosophers->display->is_used = FALSE;
	return (SUCCESS);
}
