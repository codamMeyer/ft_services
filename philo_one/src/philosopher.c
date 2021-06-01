#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <actions.h>

t_philo *create_philosophers(int num_philosophers, pthread_mutex_t *forks, pthread_mutex_t *print_action)
{
    t_philo *philosophers = malloc(sizeof(t_philo) *num_philosophers);

    philosophers[0].id = 1;
    philosophers[0].forks.right = &forks[0];
    philosophers[0].forks.left = &forks[num_philosophers - 1];
    philosophers[0].print_action = print_action;
    for (int i = 1; i < num_philosophers; ++i)
    {
        philosophers[i].id = i + 1;
        philosophers[i].forks.right = &forks[i];
        philosophers[i].forks.left = &forks[i - 1];
        philosophers[i].print_action = print_action;
    }
    return (philosophers);
}

void drop_forks(t_forks_pair *forks)
{
    pthread_mutex_unlock(forks->left);
    pthread_mutex_unlock(forks->right);
}

t_bool get_forks(t_forks_pair *forks)
{
    const t_bool has_left_fork = pthread_mutex_lock(forks->left) == 0;
    const t_bool has_right_fork = pthread_mutex_lock(forks->right) == 0;

    return (has_right_fork && has_left_fork);
}

void *eat(void *philo)
{
    const t_bool has_forks_pair = get_forks(&((t_philo *)philo)->forks);

    if (has_forks_pair)
    {
        pthread_mutex_lock(((t_philo *)philo)->print_action);
        print_action(123456, ((t_philo *)philo)->id, HAS_FORKS);
        pthread_mutex_unlock(((t_philo *)philo)->print_action);
    }    
    
    pthread_mutex_lock(((t_philo *)philo)->print_action);
    print_action(123456, ((t_philo *)philo)->id, EATING);
    pthread_mutex_unlock(((t_philo *)philo)->print_action);
    int i = 10;
    while (i >= 0)
        --i;
    pthread_mutex_lock(((t_philo *)philo)->print_action);
    print_action(123456, ((t_philo *)philo)->id, DONE_EATING);
    pthread_mutex_unlock(((t_philo *)philo)->print_action);
    drop_forks(&((t_philo *)philo)->forks);
    return NULL;
}


t_status create_philosophers_threads(t_philo *philosophers, t_dinner_rules *dinner_rules)
{
    const int num_philosophers = dinner_rules->config->number_of_philosophers;
    int ret;

    for (int i = 0; i < num_philosophers; ++i)
    {
        ret = pthread_create(&(philosophers[i].thread_id), NULL, &eat, &philosophers[i]);
        if (ret)
        {
            free(philosophers);
            printf("\ncan't create thread :[%d]", ret);
            return ERROR;
        }
    }
    return (OK);
}