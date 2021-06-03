#ifndef TYPES_H
# define TYPES_H
# define TRUE 1
# define FALSE 0
# define NOT_SET -1
# define ONE_MILLISEC 1000
# include <stdint.h>
# include <sys/time.h>

typedef int	t_bool;
typedef struct s_optional_int
{
	int		value;
	t_bool	initialized;
}	t_optional_int;

typedef struct s_time_ms
{
	uint64_t	value;
}	t_time_ms;

typedef struct s_philo_config
{
	int				min_meals;
	int				number_of_philosophers;
	struct timeval	time_start;
	t_time_ms		time_to_die;
	t_time_ms		time_to_eat;
	t_time_ms		time_to_sleep;
}	t_philo_config;

typedef struct s_optional_philo_config
{
	t_philo_config	config;
	t_bool			initialized;
}	t_optional_philo_config;

typedef enum e_status
{
	ERROR = -1,
	DEAD = 0,
	ALIVE = 1,
	OK = 2,
}	t_status;

#endif
