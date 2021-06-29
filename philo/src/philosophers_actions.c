#include <philosophers_actions.h>
#include <time_utils.h>
#include <display.h>

t_bool	get_forks(t_philo *philo)
{
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);

	if (!philo->forks.left->is_taken && !philo->forks.right->is_taken)
	{
		philo->forks.left->is_taken = \
			pthread_mutex_lock(philo->forks.left->lock) == 0;
		philo->forks.right->is_taken = \
			pthread_mutex_lock(philo->forks.right->lock) == 0;
		display_action_message(cur_time, philo, HAS_TAKEN_A_FORK);
		display_action_message(cur_time, philo, HAS_TAKEN_A_FORK);
		return (TRUE);
	}
	return (FALSE);
}

static void	drop_forks(t_forks_pair *forks)
{
	pthread_mutex_unlock(forks->left->lock);
	pthread_mutex_unlock(forks->right->lock);
	forks->left->is_taken = FALSE;
	forks->right->is_taken = FALSE;
}

void	start_to_eat(t_philo *philo)
{
	philo->last_meal.value = get_cur_time(&philo->config->time_start);
	display_action_message(philo->last_meal.value, philo, EATING);
	sleep_ms(philo->config->time_to_eat);
	drop_forks(&philo->forks);
	if (philo->config->min_meals)
	{
		++(philo->meals_counter);
		if (philo->meals_counter == philo->config->min_meals)
			--(philo->config->need_to_finish_meals);
	}
}

typedef struct s_sleep_config
{
	t_bool	will_die;
	t_time_ms time_to_sleep;
}	t_sleep_config;

t_sleep_config create_sleep_config(t_philo *philo)
{
	const unsigned int	wakeup_time = philo->last_meal.value + philo->config->time_to_eat.value + philo->config->time_to_sleep.value;
	const unsigned int	will_starve = philo->last_meal.value + philo->config->time_to_die.value;
	t_sleep_config		die_sleeping;

	die_sleeping.will_die = wakeup_time > will_starve;
	die_sleeping.time_to_sleep.value = philo->config->time_to_sleep.value - (wakeup_time - philo->config->time_to_sleep.value);
	return (die_sleeping);
}

t_life_status	start_to_sleep(t_philo *philo)
{
	const unsigned int		cur_time = get_cur_time(&philo->config->time_start);
	const t_sleep_config	sleep_config = create_sleep_config(philo);

	if (philo->config->death_event)
		return (DEAD);
	display_action_message(cur_time, philo, SLEEPING);
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
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);

	display_action_message(cur_time, philo, THINKING);
	sleep_one_ms();
}
