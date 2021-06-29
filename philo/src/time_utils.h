#ifndef TIME_UTILS_H
# define TIME_UTILS_H
# include <time.h>
# include <types.h>

unsigned long int	get_cur_time(const struct timeval	*start);
t_bool				is_dead(t_philo *philo);
t_bool				is_dinner_over(t_philo *philo);
void				sleep_ms(t_time_ms sleep_ms);
void				sleep_one_ms(void);

#endif