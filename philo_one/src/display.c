#include <display.h>
#include <stdio.h>
#include <types.h>
#include <unistd.h>

void	display_action_message(long int time, t_philo *philo, t_action action)
{
	static const char	*action_strings[5] = {
												"is eating         🍝",
												"is sleeping       💤",
												"is thinking       💭",
												"has taken a fork  🍴",
												"is dead           💀",
										   };
	while (philo->display->is_used)
		usleep(ONE_MILLISEC);
	if (!philo->display->is_used)
	{
		philo->display->is_used = pthread_mutex_lock(philo->display->lock) == 0;
		printf("| %6ldms | philo %4d | %s |\n", \
		time, philo->id, action_strings[action]);
		pthread_mutex_unlock(philo->display->lock);
		if (action != DIED)
			philo->display->is_used = FALSE;
	}
}
