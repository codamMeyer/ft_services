#ifndef DISPLAY_H
# define DISPLAY_H
# include <pthread.h>
# include <types.h>

typedef enum e_action
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	HAS_TAKEN_A_FORK = 3,
	DIED = 4
}	t_action;

void	display_action_message(long int time, t_philo *philo, t_action action);

#endif