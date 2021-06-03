#ifndef ACTIONS_H
# define ACTIONS_H

typedef enum e_action
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	HAS_FORKS = 3,
	DIED = 4
}	t_action;

void	print_action(long int time, int philo_id, t_action action);

#endif