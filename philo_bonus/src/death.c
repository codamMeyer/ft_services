#include <death.h>
#include <types.h>
#include <display.h>
#include <time_utils.h>
#include <stdlib.h>
#include <eat.h>
#include <sleep.h>
#include <philosopher.h>

static void	*check_for_death(void *void_philosopher)
{
	t_philo		*philo;
	t_time_ms	timestamp;
	uint64_t	time_since_last_meal;

	philo = (t_philo *)void_philosopher;
	while (TRUE)
	{
		timestamp = get_timestamp_diff(philo->config->time_start);
		time_since_last_meal = (timestamp.value - philo->last_meal.value);
		if (time_since_last_meal > philo->config->time_to_die.value)
		{
			display_action_message(timestamp.value, philo, DIED);
			philo->config->death_event = TRUE;
			exit(EXIT_FAILURE);
		}
		usleep(1000);
		if (has_finished_min_meals(philo))
			break ;
	}
	return (NULL);
}

static void	open_semaphores(t_philo *philo)
{
	philo->sem_id = sem_open(SEM_NAME, O_RDWR);
	philo->display.sem = sem_open(DISPLAY_NAME, O_RDWR);
	if (philo->sem_id == SEM_FAILED || philo->display.sem == SEM_FAILED)
		exit(EXIT_FAILURE);
}

static void	start_dinner(t_philo *philo)
{
	if (philo->id % 2)
		usleep(15000);
	while (!is_dinner_over(philo))
	{
		start_to_eat(philo);
		start_to_sleep(philo);
		start_to_think(philo);
	}
}

void	create_philo_thread(t_philo *philo)
{
	open_semaphores(philo);
	pthread_create(&(philo->thread_id), NULL, &check_for_death, philo);
	start_dinner(philo);
	pthread_join(philo->thread_id, NULL);
	exit(philo->config->death_event);
}
