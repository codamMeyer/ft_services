#include <stdio.h>
#include <stdlib.h>
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
	while (i < num_philosophers)
	{
		pthread_mutex_destroy(philosophers[i].forks.right->lock);
		pthread_mutex_destroy(philosophers[i].forks.left->lock);
		free(philosophers[i].forks.right->lock);
		++i;
	}
	pthread_mutex_destroy(display_action_message);
	free(display_action_message);
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

//  implement the part where it stops after eating N meals

t_status	run(const t_philo_config *config)
{
	t_display	display;
	t_fork		*forks;
	t_philo		*philosophers;

	forks = create_forks(config->number_of_philosophers);
	philosophers = create_philosophers(config, forks, &display);
	display.lock = malloc(sizeof(pthread_mutex_t));
	display.is_used = FALSE;
	if (!display.lock || pthread_mutex_init(display.lock, NULL))
	{
		cleanup_forks(forks, config->number_of_philosophers);
		printf("\n mutex init failed\n");
		return (ERROR);
	}
	create_philosophers_threads(philosophers);
	join_threads(philosophers, config->number_of_philosophers);
	destroy_mutexes(philosophers, config->number_of_philosophers, display.lock);
	free(forks);
	free(philosophers);
	return (0);
}

int	main(int argc, const char *argv[])
{
	const t_optional_philo_config	optional_config = \
							parse_config_args(argc, argv);
	t_status						ret;

	if (!optional_config.initialized)
	{
		printf("Usage:\t./philo_one <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	ret = run(&optional_config.config);
	return (ret);
}
