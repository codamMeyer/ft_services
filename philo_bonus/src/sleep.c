#include <sleep.h>
#include <time_utils.h>
#include <display.h>

static unsigned int	get_wakeup_time(const t_philo *philo)
{
	return (philo->last_meal.value + \
			philo->config->time_to_eat.value + \
			philo->config->time_to_sleep.value);
}

static unsigned int	get_starvation_time(const t_philo *philo)
{
	return (philo->last_meal.value + philo->config->time_to_die.value);
}

static t_sleep_config	create_sleep_config(const t_philo *philo)
{
	const unsigned int	wakeup_time = get_wakeup_time(philo);
	const unsigned int	starvation_time = get_starvation_time(philo);
	const t_time_ms		timestamp = \
						get_timestamp_diff(philo->config->time_start);
	t_sleep_config		sleep_config;

	sleep_config.will_die = wakeup_time > starvation_time;
	sleep_config.time_to_sleep.value = starvation_time - timestamp.value;
	return (sleep_config);
}

t_life_status	start_to_sleep(t_philo *philo)
{
	const t_sleep_config	sleep_config = create_sleep_config(philo);

	if (philo->config->death_event)
		return (DEAD);
	display_action_message(philo->finished_eating.value, philo, SLEEPING);
	if (sleep_config.will_die)
	{
		sleep_ms(sleep_config.time_to_sleep);
		philo->config->death_event = TRUE;
		return (DEAD);
	}
	sleep_ms(philo->config->time_to_sleep);
	return (ALIVE);
}

void	start_to_think(t_philo *philo)
{
	const t_time_ms	timestamp = get_timestamp_diff(philo->config->time_start);
	t_time_ms		time;

	time.value = 5;
	display_action_message(timestamp.value, philo, THINKING);
	sleep_ms(time);
}
