#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <types.h>

t_philo	*create_philosophers(t_philo_config *config);
void	destroy(t_philo *philosophers);
void	setup_philosopher(t_philo *philosopher, \
								t_philo_config *config, \
								int id);
t_bool	has_finished_min_meals(const t_philo *philo);

#endif
