#include <time_utils.h>
#include <display.h>

unsigned long int	get_cur_time(const struct timeval	*start)
{
	struct timeval	time_action;

	gettimeofday(&time_action, NULL);
	return ((time_action.tv_sec - start->tv_sec) * ONE_MILLISEC \
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
