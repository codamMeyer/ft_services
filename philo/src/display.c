#include "types.h"
#include <display.h>
#include <stdio.h>
#include <unistd.h>
#include <types.h>
#include <stdlib.h>
#include <time_utils.h>

t_display	create_display(void)
{
	t_display	display;

	display.lock = malloc(sizeof(pthread_mutex_t));
	display.is_used = TRUE;
	return (display);
}

void	destroy_display(t_display *display)
{
	if (!display->lock)
		return ;
	pthread_mutex_destroy(display->lock);
	free(display->lock);
}

void	display_usage_message(void)
{
	const char	*usage = "Usage:\t./philo <number_of_philosophers> \
<time_to_die> [number_of_times_each_philosopher_must_eat]\n";

	write(STDERR_FILENO, usage, 98);
}

void	display_action_message(long int time, t_philo *philo, t_action action)
{
	const t_time_ms		wait_time = {.value = 1};
	static const char	*action_str[5] = {
												"is eating           🍝",
												"is sleeping         💤",
												"is thinking         💭",
												"has taken a fork    🍴",
												"died                💀",
										   };

	while (philo->display->is_used && !philo->config->death_event)
		sleep_ms(wait_time);
	if (!philo->display->is_used)
	{
		philo->display->is_used = TRUE;
		pthread_mutex_lock(philo->display->lock);
		printf("%ld %d %s\n", time, philo->id, action_str[action]);
		pthread_mutex_unlock(philo->display->lock);
		if (action != DIED)
			philo->display->is_used = FALSE;
	}
}
