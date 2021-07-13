#ifndef TYPES_H
# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <unistd.h>
#include <sys/stat.h>
# include <fcntl.h>
# define TYPES_H
# define TRUE 1
# define FALSE 0
# define FALSE 0
# define NOT_SET 0
# define SEM_NAME "/forks"
# define DISPLAY_NAME "/display"
# define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
# define SEM_FLAGS ( O_CREAT | O_EXCL)
#define ANY_CHILD -1

typedef int	t_bool;

typedef void *(t_start_routine)(void *);
typedef int (t_pthread_create)(pthread_t *, \
							const pthread_attr_t *, \
							t_start_routine, void *);

typedef struct s_time_ms
{
	uint64_t	value;
}	t_time_ms;

typedef struct s_display
{
	sem_t			*sem;
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
}	t_philo_config;

typedef struct s_philo
{
	int						id;
	pthread_t				thread_id;
	sem_t					*sem_id;
	pid_t					pid;
	int						meals_counter;
	t_time_ms				last_meal;
	t_time_ms				finished_eating;
	t_philo_config			*config;
	t_display				display;
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
	ERROR = 1,
	DEATH_EVENT = 2,
}	t_status;

#endif
