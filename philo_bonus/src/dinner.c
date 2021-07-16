#include <stdlib.h>
#include <stdio.h>
#include <display.h>
#include <dinner.h>
#include <time_utils.h>
#include <sys/wait.h>
#include <signal.h>
#include <philosopher.h>
#include <death.h>

static void	create_processes(t_philo *philosophers)
{
	int		i;

	i = 0;
	while (i < philosophers->config->number_of_philosophers)
	{
		philosophers[i].pid = fork();
		if (philosophers[i].pid < 0)
			exit(EXIT_FAILURE);
		if (philosophers[i].pid == 0)
		{
			create_philo_thread(&philosophers[i]);
			break ;
		}
		++i;
	}
}

static void	kill_all_processes(t_philo *philosophers, int num_philo)
{
	int	i;

	i = 0;
	while (i < num_philo)
	{
		kill(philosophers[i].pid, SIGTERM);
		++i;
	}
}

static t_status	wait_processes(t_philo *philosophers, int num_philo)
{
	int	i;
	int	ret;

	i = 0;
	while (i < num_philo)
	{
		waitpid(ANY_CHILD, &ret, 0);
		if (ret != 0)
		{
			kill_all_processes(philosophers, num_philo);
			ret = DEATH_EVENT;
			break ;
		}
		++i;
	}
	return (ret);
}

t_status	run(t_philo_config *config)
{
	t_philo		*philosophers;
	t_status	ret;

	philosophers = create_philosophers(config);
	if (!philosophers)
		return (ERROR);
	create_processes(philosophers);
	ret = wait_processes(philosophers, config->number_of_philosophers);
	destroy(philosophers);
	return (ret);
}
