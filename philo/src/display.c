#include <display.h>
#include <stdio.h>
#include <types.h>
#include <unistd.h>
#include <stdlib.h>

t_display create_display()
{
	t_display display;

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
	printf("<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n");
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

	while (philo->display->is_used && !philo->config->death_event)
		usleep(ONE_MILLISEC);
	if (!philo->display->is_used)
	{
		philo->display->is_used = \
								pthread_mutex_lock(philo->display->lock) == 0;
		printf("%6ldms philo %3d %s |\n", time, philo->id, action_str[action]);
		pthread_mutex_unlock(philo->display->lock);
		if (action != DIED)
			philo->display->is_used = FALSE;
	}
}
