#ifndef SLEEP_H
# define SLEEP_H
# include <types.h>

typedef struct s_sleep_config
{
	t_bool		will_die;
	t_time_ms	time_to_sleep;
}	t_sleep_config;

t_life_status	start_to_sleep(t_philo *philo);
void			start_to_think(t_philo *philo);

#endif