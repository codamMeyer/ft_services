#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <display.h>
#include <unistd.h>
#include <sys/time.h>

t_philo	*create_philosophers(const t_philo_config *config, \
									t_fork *forks, \
									t_display *display)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	i = 0;
	while (i < num_philosophers)
	{
		philosophers[i].id = i + 1;
		philosophers[i].last_meal.value = 0;
		philosophers[i].forks.right = &forks[i];
		if (i == 0)
			philosophers[i].forks.left = &forks[num_philosophers - 1];
		else
			philosophers[i].forks.left = &forks[i - 1];
		philosophers[i].display = display;
		philosophers[i].config = config;
		++i;
	}
	return (philosophers);
}

void	drop_forks(t_forks_pair *forks)
{
	pthread_mutex_unlock(forks->left->lock);
	pthread_mutex_unlock(forks->right->lock);
	forks->left->is_taken = FALSE;
	forks->right->is_taken = FALSE;
}

unsigned long int	get_cur_time(const struct timeval	*start)
{
	struct timeval	time_action;

	gettimeofday(&time_action, NULL);
	return ((time_action.tv_sec - start->tv_sec) * ONE_MILLISEC \
		+ (time_action.tv_usec - start->tv_usec) * 0.001);
}

void	get_forks(t_philo *philo)
{
	unsigned int	cur_time;

	if (!philo->forks.left->is_taken && !philo->forks.right->is_taken)
	{
		philo->forks.left->is_taken = \
			pthread_mutex_lock(philo->forks.left->lock) == 0;
		philo->forks.right->is_taken = \
			pthread_mutex_lock(philo->forks.right->lock) == 0;
	}
	if (philo->forks.left->is_taken && philo->forks.right->is_taken)
	{
		cur_time = get_cur_time(&philo->config->time_start);
		display_action_message(cur_time, philo, HAS_FORKS);
	}
}

t_bool	is_dead(t_philo *philo)
{
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);

	if ((cur_time - philo->last_meal.value) > philo->config->time_to_die.value)
	{
		printf("philo %d is dead\n", philo->id);
		return (TRUE);
	}
	return (FALSE);
}

void	start_to_eat(t_philo *philo)
{
	philo->last_meal.value = get_cur_time(&philo->config->time_start);
	display_action_message(philo->last_meal.value, philo, EATING);
	usleep(philo->config->time_to_eat.value * ONE_MILLISEC);
	drop_forks(&philo->forks);
}

t_status	start_to_sleep(t_philo *philo)
{
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);
	const unsigned int	wakeup_time = \
			philo->last_meal.value + philo->config->time_to_eat.value + \
			philo->config->time_to_sleep.value ;
	const unsigned int	will_starve = \
			philo->last_meal.value + philo->config->time_to_die.value;
	unsigned int		sleep_and_die_time;

	display_action_message(cur_time, philo, SLEEPING);
	if (wakeup_time > will_starve)
	{
		sleep_and_die_time = philo->config->time_to_sleep.value - \
						(wakeup_time - philo->config->time_to_sleep.value);
		usleep(sleep_and_die_time * ONE_MILLISEC);
		return (DEAD);
	}
	usleep(philo->config->time_to_sleep.value * ONE_MILLISEC);
	return (ALIVE);
}

void	start_to_think(t_philo *philo)
{
	const unsigned int	cur_time = get_cur_time(&philo->config->time_start);

	display_action_message(cur_time, philo, THINKING);
	usleep(ONE_MILLISEC);
}

void	*start_dinner(void *philo)
{
	unsigned int	cur_time;
	t_philo			*philosopher;

	philosopher = (t_philo *)philo;
	while (!is_dead(philosopher))
	{
		get_forks(philosopher);
		start_to_eat(philosopher);
		if (start_to_sleep(philosopher) == DEAD)
		{
			cur_time = get_cur_time(&(philosopher)->config->time_start);
			display_action_message(cur_time, (philosopher), DIED);
			break ;
		}
		start_to_think(philosopher);
	}
	return (NULL);
}

t_status	create_philosophers_threads(t_philo *philosophers)
{
	const int	num_philosophers = philosophers->config->number_of_philosophers;
	int			ret;
	int			i;

	i = 0;
	while (i < num_philosophers)
	{
		ret = pthread_create(&(philosophers[i].thread_id), \
							NULL, &start_dinner, &philosophers[i]);
		if (ret)
		{
			free(philosophers);
			printf("\ncan't create thread :[%d]", ret);
			return (ERROR);
		}
		++i;
	}
	return (OK);
}
