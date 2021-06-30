#ifndef TIME_UTILS_H
# define TIME_UTILS_H
# include <time.h>
# include <types.h>

t_time_ms	get_timestamp(void);
t_time_ms	get_timestamp_diff(t_time_ms start);
t_bool		is_dead(t_philo *philo);
t_bool		is_dinner_over(t_philo *philo);
void		sleep_ms(t_time_ms sleep_ms);

#endif