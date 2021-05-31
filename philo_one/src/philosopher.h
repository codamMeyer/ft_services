#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <types.h>
# include <pthread.h>

typedef struct s_forks_pair
{
    pthread_mutex_t *right;
    pthread_mutex_t *left;
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

t_philo *create_philosophers(int num_philosophers, pthread_mutex_t *forks, pthread_mutex_t *print_action);
void *eat(void *philo);
t_status create_philosophers_threads(t_philo *philosophers, int num_philosophers);

#endif