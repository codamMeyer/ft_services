#include <eat.h>
#include <display.h>
#include <time_utils.h>
#include <stdlib.h>

static void	get_forks(t_philo *philo)
{
	if (sem_wait(philo->sem_id) != 0)
		exit(EXIT_FAILURE);
	if (sem_wait(philo->sem_id) != 0)
		exit(EXIT_FAILURE);
	philo->last_meal = get_timestamp_diff(philo->config->time_start);
	display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
	display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
}

static void	release_forks(sem_t *sem_id)
{
	if (sem_post(sem_id) != 0)
		exit(EXIT_FAILURE);
	if (sem_post(sem_id) != 0)
		exit(EXIT_FAILURE);
}

void	start_to_eat(t_philo *philo)
{
	get_forks(philo);
	display_action_message(philo->last_meal.value, philo, EATING);
	sleep_ms(philo->config->time_to_eat);
	philo->finished_eating = get_timestamp_diff(philo->config->time_start);
	release_forks(philo->sem_id);
	++(philo->meals_counter);
}
