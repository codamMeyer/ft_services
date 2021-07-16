#include "types.h"
#include <display.h>
#include <stdio.h>
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
	printf("Usage:\t./philo <number_of_philosophers> <time_to_die> ");
	printf("<time_to_eat> <time_to_sleep>");
	printf(" [number_of_times_each_philosopher_must_eat]\n");
}

void	display_action_message(long int time, t_philo *philo, t_action action)
{
	static t_bool		death = FALSE;
	const t_time_ms		wait_time = {.value = 0.0001};
	static const char	*action_str[5] = {
												"is eating         🍝",
												"is sleeping       💤",
												"is thinking       💭",
												"has taken a fork  🍴",
												"is dead           💀",
										   };

	while (philo->display->is_used && !philo->config->death_event)
		sleep_ms(wait_time);
	if (!philo->display->is_used && !death)
	{
		death = (action == DIED);
		philo->display->is_used = \
								pthread_mutex_lock(philo->display->lock) == 0;
		printf("%6ldms philo %3d %s\n", time, philo->id, action_str[action]);
		pthread_mutex_unlock(philo->display->lock);
		philo->display->is_used = FALSE;
	}
}
