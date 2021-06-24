#ifndef PHILOSOPHERS_ACTIONS_H
# define PHILOSOPHERS_ACTIONS_H
# include <types.h>

void			drop_forks(t_forks_pair *forks);
t_bool			get_forks(t_philo *philo);
void			start_to_eat(t_philo *philo);
t_life_status	start_to_sleep(t_philo *philo);
void			start_to_think(t_philo *philo);

#endif