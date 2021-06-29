#include <time_utils.h>
#include <display.h>
#include <unistd.h>

unsigned long int	get_cur_time(const struct timeval	*start)
{
	const uint64_t	mult_value = 1000;
	struct timeval	time_action;

	gettimeofday(&time_action, NULL);
	return ((time_action.tv_sec - start->tv_sec) * mult_value \
		+ (time_action.tv_usec - start->tv_usec) * 0.001);
}

t_bool	is_dead(t_philo *philo)
{
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);

	if ((cur_time - philo->last_meal.value) > philo->config->time_to_die.value)
	{
		display_action_message(cur_time, philo, DIED);
		philo->config->death_event = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_dinner_over(t_philo *philo)
{
	if (philo->config->min_meals && philo->config->need_to_finish_meals == 0)
		return (TRUE);
	if (is_dead(philo) || philo->config->death_event)
		return (TRUE);
	return (FALSE);
}

void sleep_one_ms(void)
{
	const t_time_ms one_ms = {.value = 0.1};

	sleep_ms(one_ms);
}

void sleep_ms(t_time_ms sleep_ms)
{
	const uint64_t	mult_value = 1000;

	usleep(sleep_ms.value * mult_value);
}
