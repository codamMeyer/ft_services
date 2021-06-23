#include <stdio.h>
#include <stdlib.h>
#include <display.h>
#include <parser.h>
#include <fork.h>
#include <types.h>
#include <philosopher.h>
#include <pthread.h>
#include <unistd.h>

void	destroy_mutexes(t_philo *philosophers,
						int num_philosophers,
						pthread_mutex_t *display_action_message)
{
	int	i;

	i = 0;
	if (!philosophers)
		return ;
	while (i < num_philosophers)
	{
		pthread_mutex_destroy(philosophers[i].forks.right->lock);
		++i;
	}
	pthread_mutex_destroy(display_action_message);
}

void cleanup(t_fork *forks, t_philo *philosophers, t_display *display)
{
	const int num_philosophers = philosophers->config->number_of_philosophers;

	destroy_mutexes(philosophers, num_philosophers, display->lock);
	cleanup_forks(forks, num_philosophers);
	free(philosophers);
	free(display->lock);
}

void	join_threads(t_philo *philosophers, int num_philosophers)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		pthread_join(philosophers[i].thread_id, NULL);
		++i;
	}
}

t_status	malloc_resources(t_philo_config *config, t_fork **forks, t_philo **philosophers, t_display *display)
{
	display->lock = malloc(sizeof(pthread_mutex_t));
	display->is_used = FALSE;
	if (!display->lock)
		return (ERROR);
	*forks = create_forks(config->number_of_philosophers);
	if (!(*forks))
	{
		free(display->lock);
		return (ERROR);
	}
	*philosophers = create_philosophers(config, *forks, display);
	if (!(*philosophers))
	{
		free(display->lock);
		cleanup_forks(*forks, config->number_of_philosophers);
		return (ERROR);
	}
	return (SUCCESS);
}

t_status	run(t_philo_config *config)
{
	t_display	display;
	t_fork		*forks;
	t_philo		*philosophers;
	t_status	ret;

	ret = SUCCESS;
	forks = NULL;
	philosophers = NULL;
	if (malloc_resources(config, &forks, &philosophers, &display) == ERROR)
		return (ERROR);
	ret = create_philosophers_threads(philosophers, pthread_create);
	join_threads(philosophers, config->initialized_threads);
	if (config->death_event)
		ret = DEATH_EVENT;
	cleanup(forks, philosophers, &display);
	return (ret);
}

int	main(int argc, const char *argv[])
{
	t_optional_philo_config	optional;
	t_status				ret;

	optional = parse_config_args(argc, argv);
	if (!optional.initialized)
	{
		display_usage_message();
		return (ERROR);
	}
	if (optional.config.number_of_philosophers < 2)
	{
		printf("Invalid Number of Philosophers\n");
		return (ERROR);
	}
	ret = run(&optional.config);
	return (ret);
}
