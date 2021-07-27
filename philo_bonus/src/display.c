#include <display.h>
#include <stdio.h>
#include <types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time_utils.h>

t_bool	create_display_semaphore(void)
{
	sem_t	*semaphore;

	sem_unlink(DISPLAY_NAME);
	semaphore = sem_open(DISPLAY_NAME, (O_CREAT | O_EXCL), \
				(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP), 1);
	if (semaphore == SEM_FAILED)
		return (FALSE);
	if (sem_close(semaphore) < 0)
	{
		sem_unlink(DISPLAY_NAME);
		return (FALSE);
	}
	return (TRUE);
}

void	display_usage_message(void)
{
	const char	*usage = "Usage:\t./philo <number_of_philosophers> \
<time_to_die> [number_of_times_each_philosopher_must_eat]\n";

	write(STDERR_FILENO, usage, 98);
}

void	display_action_message(long int time, t_philo *philo, t_action action)
{
	static const char	*action_str[5] = {
												"is eating           🍝",
												"is sleeping         💤",
												"is thinking         💭",
												"has taken a fork    🍴",
												"died                💀",
										   };

	sem_wait(philo->display.sem);
	printf("%ld %d %s\n", time, philo->id, action_str[action]);
	if (action != DIED)
		sem_post(philo->display.sem);
}
