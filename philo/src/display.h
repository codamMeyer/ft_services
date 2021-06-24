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

t_display	create_display(void);
void		destroy_display(t_display *display);
void		display_usage_message(void);
void		display_action_message(long int time, t_philo *philo, t_action action);

#endif