#include <display.h>
#include <stdio.h>
#include <types.h>
#include <stdlib.h>
#include <time_utils.h>

t_display	create_display(void)
{
	t_display	display;

	sem_unlink(SEM_NAME);
	display.sem = sem_open(SEM_NAME, SEM_FLAGS, SEM_PERMS, 1);
	if (display.sem == SEM_FAILED)
	{
		perror("sem_open(3) error");
		exit(1);
	}
	if (sem_close(display.sem) < 0)
	{
		perror("sem_close(3) failed");
		sem_unlink(SEM_NAME);
		exit(1);
	}
	return (display);
}

void	display_usage_message(void)
{
	printf("Usage:\t./philo <number_of_philosophers> <time_to_die> ");
	printf("<time_to_eat> <time_to_sleep>");
	printf(" [number_of_times_each_philosopher_must_eat]\n");
}

void	display_action_message(long int time, t_philo *philo, t_action action)
{
	static const char	*action_str[5] = {
												"is eating         🍝",
												"is sleeping       💤",
												"is thinking       💭",
												"has taken a fork  🍴",
												"is dead           💀",
										   };

	sem_wait(philo->display.sem);
	printf("%6ldms philo %3d %s |\n", time, philo->id, action_str[action]);
	sem_post(philo->display.sem);
}
