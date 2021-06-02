#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <actions.h>
#include <unistd.h>
#include <sys/time.h>

t_philo	*create_philosophers(const t_philo_config *config, \
									t_fork *forks, \
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
	pthread_mutex_unlock(forks->left->lock);
	pthread_mutex_unlock(forks->right->lock);
	forks->left->is_taken = FALSE;
	forks->right->is_taken = FALSE;
}

unsigned long int get_time_diff(const struct timeval	*start)
{
	struct timeval	time_action;

	gettimeofday(&time_action, NULL);
	return ((time_action.tv_sec - start->tv_sec) * 1000 + (time_action.tv_usec - start->tv_usec) * 0.001);
}

void	get_forks(t_philo *philo)
{
	unsigned int cur_time;
	if (!philo->forks.left->is_taken && !philo->forks.right->is_taken)
	{
		philo->forks.left->is_taken = pthread_mutex_lock(philo->forks.left->lock) == 0;
		philo->forks.right->is_taken = pthread_mutex_lock(philo->forks.right->lock) == 0;
	}
	if (philo->forks.left->is_taken && philo->forks.right->is_taken)
	{
		cur_time = get_time_diff(&philo->config->time_start);
		pthread_mutex_lock(philo->print_action);
		print_action(cur_time, philo->id, HAS_FORKS);
		pthread_mutex_unlock(philo->print_action);
	}
}

void	start_to_eat(t_philo *philo)
{
	philo->last_meal.value = get_time_diff(&philo->config->time_start);
	pthread_mutex_lock(philo->print_action);
	print_action(philo->last_meal.value, philo->id, EATING);
	pthread_mutex_unlock(philo->print_action);
	usleep(philo->config->time_to_eat.value * ONE_MILLISEC);
	drop_forks(&philo->forks);
}

void	start_to_sleep(t_philo *philo)
{
	unsigned int cur_time = get_time_diff(&philo->config->time_start); 
	pthread_mutex_lock(philo->print_action);
	print_action(cur_time, philo->id, SLEEPING);
	pthread_mutex_unlock(philo->print_action);
	usleep(philo->config->time_to_sleep.value * ONE_MILLISEC);
}

void	start_to_think(t_philo *philo)
{
	unsigned int cur_time = get_time_diff(&philo->config->time_start); 
	pthread_mutex_lock(philo->print_action);
	print_action(cur_time, philo->id, THINKING);
	pthread_mutex_unlock(philo->print_action);
	usleep(philo->config->time_to_sleep.value * ONE_MILLISEC);
}

void	*start_dinner(void *philo)
{
	int i = 2;
	while (i > 0)
	{
		get_forks((t_philo *)philo);
		start_to_eat((t_philo *)philo);
		start_to_sleep((t_philo *)philo);
		start_to_think((t_philo *)philo);
		--i;
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
