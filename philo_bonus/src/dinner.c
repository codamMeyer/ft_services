#include <stdlib.h>
#include <stdio.h>
#include <display.h>
#include <dinner.h>
#include <time_utils.h>
#include <sys/wait.h>
#include <signal.h>

static void	cleanup(t_philo *philosophers)
{
	free(philosophers);
}

t_philo	*create_philosophers(t_philo_config *config)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < num_philosophers)
	{
		philosophers[i].config = config;
		philosophers[i].id = i + 1;
		philosophers[i].meals_counter = 0;
		philosophers[i].last_meal.value = 0;
		
		++i;
	}
	return (philosophers);
}

t_bool	get_forks(t_philo *philo)
{
	sem_wait(philo->sem_id);
	sem_wait(philo->sem_id);
	philo->last_meal = get_timestamp_diff(philo->config->time_start);
	display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
	display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
	return (TRUE);
}

void	start_to_eat(t_philo *philo)
{
	display_action_message(philo->last_meal.value, philo, EATING);
	sleep_ms(philo->config->time_to_eat);
	philo->finished_eating = get_timestamp_diff(philo->config->time_start);
	sem_post(philo->sem_id);
	sem_post(philo->sem_id);
	if (philo->config->min_meals)
	{
		++(philo->meals_counter);
		if (philo->meals_counter == philo->config->min_meals)
			--(philo->config->need_to_finish_meals);
	}
}

typedef struct s_sleep_config
{
	t_bool		will_die;
	t_time_ms	time_to_sleep;
}	t_sleep_config;

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
	const t_time_ms		timestamp = get_timestamp_diff(philo->config->time_start);
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

void start_to_think(t_philo *philo)
{
	const t_time_ms	timestamp = get_timestamp_diff(philo->config->time_start);
	
	display_action_message(timestamp.value, philo, THINKING);
}

static t_bool finished_min_meals(const t_philo *philo)
{
	return (philo->meals_counter >= philo->config->min_meals && philo->config->min_meals != NOT_SET);
}

void	*check_for_death(void *void_philosopher)
{
	t_philo	*philo;
	t_time_ms timestamp;

	philo = (t_philo *)void_philosopher;
	while (TRUE)
	{
        timestamp = get_timestamp_diff(philo->config->time_start);
		if ((timestamp.value - philo->last_meal.value) > philo->config->time_to_die.value)
		{
            display_action_message(timestamp.value, philo, DIED);
            philo->config->death_event = TRUE;
			exit(1);
		}
		usleep(1000);
		if (finished_min_meals(philo))
			break ;
	}
	return (NULL);
}

static void open_semaphores(t_philo *philo)
{
	philo->sem_id = sem_open(SEM_NAME, O_RDWR);
	philo->display.sem = sem_open(DISPLAY_NAME, O_RDWR);
	if (philo->sem_id == SEM_FAILED || philo->display.sem == SEM_FAILED)
	{
		printf("sem_open(3) failed");
		exit(EXIT_FAILURE);
	}
}

static void start_dinner(t_philo *philo)
{
	if (philo->id % 2)
		usleep(15000);
	while (!is_dinner_over(philo))
	{
		get_forks(philo);
        start_to_eat(philo);
        start_to_sleep(philo);
        start_to_think(philo);
    }
}

int create_philo_thread(t_philo *philo)
{
	int ret;

	open_semaphores(philo);
	pthread_create(&(philo->thread_id), NULL, &check_for_death, philo);
    start_dinner(philo);
    pthread_join(philo->thread_id, NULL);
	ret = philo->config->death_event;
	exit(ret);
}

static void	create_processes(t_philo *philosophers)
{
	int		i;

	i = 0;
	while (i < philosophers->config->number_of_philosophers)
	{
        philosophers[i].pid = fork();
		if (philosophers[i].pid < 0)
		{
			printf("fork(2) failed");
			exit(EXIT_FAILURE);
		}

		if (philosophers[i].pid == 0)
        {
        	if (create_philo_thread(&philosophers[i]) < 0) {
				exit(EXIT_FAILURE);
			}
			break ;
		}
		++i;
	}
}

static void kill_all_processes(t_philo *philosophers, int num_philo)
{
	int i;

	i = 0;
	while (i < num_philo)
	{
		kill(philosophers[i].pid, SIGTERM);
		++i;
	}
}

static	void wait_processes(t_philo *philosophers, int num_philo)
{
	int i;
	int ret;

	i = 0;
	while (i < num_philo)
	{
		waitpid(ANY_CHILD, &ret, 0);
		
		if (ret != 0)
		{
			kill_all_processes(philosophers, num_philo);
			break ;
		}
		++i;
	}
}

static t_status	malloc_resources(t_philo_config *config,
								t_philo **philosophers)
{
	*philosophers = create_philosophers(config);
	if (!(*philosophers))
		return (ERROR);
	return (SUCCESS);
}

t_status	run(t_philo_config *config)
{
	t_philo		*philosophers;

	philosophers = NULL;
	if (malloc_resources(config, &philosophers) == ERROR)
		return (ERROR);
	philosophers->config->time_start = get_timestamp();
	create_processes(philosophers);
	wait_processes(philosophers, config->number_of_philosophers);
	cleanup(philosophers);
	return (SUCCESS);
}
