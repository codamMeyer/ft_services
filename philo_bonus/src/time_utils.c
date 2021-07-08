#include <time_utils.h>
#include <unistd.h>
#include <stdio.h>
#include <display.h>
#include <time_utils.h>

t_time_ms	get_timestamp(void)
{
	const uint64_t	milli_factor = 1000;
	struct timeval	time;
	t_time_ms		timestamp;

	gettimeofday(&time, NULL);
	timestamp.value = (time.tv_sec * milli_factor) + \
						(time.tv_usec / milli_factor);
	return (timestamp);
}

t_time_ms	get_timestamp_diff(t_time_ms start)
{
	const t_time_ms	diff = {.value = get_timestamp().value - start.value};

	return (diff);
}

t_bool	is_dead(t_philo *philo)
{
	const t_time_ms	timestamp = get_timestamp_diff(philo->config->time_start);
	const uint64_t	time_since_last_meal = \
					(timestamp.value - philo->last_meal.value);

	if (time_since_last_meal > philo->config->time_to_die.value)
	{
		display_action_message(timestamp.value, philo, DIED);
		philo->config->death_event = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_dinner_over(t_philo *philo)
{
	if (philo->config->min_meals && (philo->meals_counter == philo->config->min_meals))
		return (TRUE);
	return (FALSE);
}

void	sleep_ms(t_time_ms sleep_ms)
{
	const t_time_ms	start = get_timestamp();

	while (TRUE)
	{
		if (get_timestamp().value - start.value >= sleep_ms.value)
			break ;
		usleep(50);
	}
}
