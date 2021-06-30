#ifndef TYPES_H
# define TYPES_H
# define TRUE 1
# define FALSE 0
# define NOT_SET 0
# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>

typedef int	t_bool;

typedef void *(t_start_routine)(void *);
typedef int (t_pthread_create)(pthread_t *, \
							const pthread_attr_t *, \
							t_start_routine, void *);

typedef struct s_time_ms
{
	uint64_t	value;
}	t_time_ms;

typedef struct s_fork
{
	pthread_mutex_t	*lock;
	t_bool			is_taken;
}	t_fork;

typedef struct s_forks_pair
{
	t_fork	*right;
	t_fork	*left;
}	t_forks_pair;

typedef struct s_display
{
	pthread_mutex_t	*lock;
	t_bool			is_used;
}	t_display;

typedef struct s_philo_config
{
	int				min_meals;
	int				need_to_finish_meals;
	int				number_of_philosophers;
	t_time_ms		time_start;
	t_time_ms		time_to_die;
	t_time_ms		time_to_eat;
	t_time_ms		time_to_sleep;
	t_bool			death_event;
	int				initialized_threads;
}	t_philo_config;

typedef struct s_philo
{
	int						id;
	int						meals_counter;
	t_time_ms				last_meal;
	t_time_ms				finished_eating;
	pthread_t				thread_id;
	t_forks_pair			forks;
	t_display				*display;
	t_philo_config			*config;
}	t_philo;

typedef struct s_optional_int
{
	int		value;
	t_bool	initialized;
}	t_optional_int;

typedef struct s_optional_philo_config
{
	t_philo_config	config;
	t_bool			initialized;
}	t_optional_philo_config;

typedef enum e_life_status
{
	DEAD = 0,
	ALIVE = 1,
}	t_life_status;

typedef enum e_status
{
	SUCCESS = 0,
	DEATH_EVENT = 2,
	ERROR = 3,
}	t_status;

#endif
