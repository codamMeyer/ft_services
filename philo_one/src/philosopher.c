#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <actions.h>
#include <unistd.h>
#include <sys/time.h>

t_philo	*create_philosophers(const t_philo_config *config, \
									pthread_mutex_t *forks, \
									pthread_mutex_t *print_action)
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
		philosophers[i].print_action = print_action;
		philosophers[i].config = config;
		++i;
	}
	return (philosophers);
}

void	drop_forks(t_forks_pair *forks)
{
	pthread_mutex_unlock(forks->left);
	pthread_mutex_unlock(forks->right);
}

unsigned long int get_time_diff(const struct timeval	*start)
{
	struct timeval	time_action;

	gettimeofday(&time_action, NULL);
	return ((time_action.tv_sec - start->tv_sec) * 1000 + (time_action.tv_usec - start->tv_usec) * 0.001);
}

void	get_forks(t_philo *philo)
{

	if (!philo->forks.has_left_fork)
		philo->forks.has_left_fork = \
									pthread_mutex_lock(philo->forks.left) == 0;
	if (!philo->forks.has_right_fork)
		philo->forks.has_right_fork = \
									pthread_mutex_lock(philo->forks.right) == 0;
	if (philo->forks.has_left_fork && philo->forks.has_right_fork)
	{
		pthread_mutex_lock(philo->print_action);
		print_action(get_time_diff(&philo->config->time_start), philo->id, HAS_FORKS);
		pthread_mutex_unlock(philo->print_action);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->print_action);
	print_action(get_time_diff(&philo->config->time_start), philo->id, EATING);
	pthread_mutex_unlock(philo->print_action);
	usleep(philo->config->time_to_eat.value * ONE_MILLISEC);
	drop_forks(&philo->forks);
}

void	sleep_(t_philo *philo)
{
	pthread_mutex_lock(philo->print_action);
	print_action(get_time_diff(&philo->config->time_start), philo->id, SLEEPING);
	pthread_mutex_unlock(philo->print_action);
	usleep(philo->config->time_to_sleep.value * ONE_MILLISEC);
	drop_forks(&philo->forks);
}

void	*start_dinner(void *philo)
{
	get_forks((t_philo *)philo);
	eat((t_philo *)philo);
	sleep_((t_philo *)philo);
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
