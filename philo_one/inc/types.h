#ifndef TYPES_H
#define TYPES_H
# define TRUE 1
# define FALSE 0
# include <stdint.h>

typedef int t_bool;
typedef struct s_optional_int
{
	int		value;
	t_bool	initialized;
} t_optional_int;

typedef struct s_time_ms
{
	uint64_t	value;
} t_time_ms;

typedef struct s_philosopher
{
	int time_since_last_meal;
} t_philosopher;

typedef struct s_philo_config
{
	t_time_ms number_of_philosophers;
	t_time_ms time_to_die;
	t_time_ms time_to_eat;
	t_time_ms time_to_sleep;
	t_time_ms min_meals;
} t_philo_config;

typedef struct s_optional_philo_config
{
	t_philo_config	config;
	t_bool			initialized;
} t_optional_philo_config;

#endif
