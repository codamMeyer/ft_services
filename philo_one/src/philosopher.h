#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <types.h>
# include <pthread.h>

typedef struct s_forks_pair
{
    pthread_mutex_t *right;
    pthread_mutex_t *left;
    t_bool has_left_fork;
    t_bool has_right_fork;
} t_forks_pair;

typedef struct s_philo
{
    int id;
    int meals_counter;
    t_time_ms time_since_last_meal;
    pthread_t thread_id;
    t_forks_pair forks;
    pthread_mutex_t *print_action;
} t_philo;

typedef struct s_dinner_rules
{
    t_philo *philo;
    const t_philo_config *config;
}   t_dinner_rules;

t_philo *create_philosophers(int num_philosophers, pthread_mutex_t *forks, pthread_mutex_t *print_action);
void *start_dinner(void *dinner_rules);
t_status create_philosophers_threads(t_philo *philosophers, t_dinner_rules *dinner_rules);

#endif