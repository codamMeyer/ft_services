#include <philosopher.h>
#include <stdlib.h>
#include <stdio.h>
#include <actions.h>

t_philo *create_philosophers(const t_philo_config *config, pthread_mutex_t *forks, pthread_mutex_t *print_action)
{
    const int num_philosophers = config->number_of_philosophers;
    t_philo *philosophers = malloc(sizeof(t_philo) *num_philosophers);

    philosophers[0].id = 1;
    philosophers[0].forks.right = &forks[0];
    philosophers[0].forks.left = &forks[num_philosophers - 1];
    philosophers[0].print_action = print_action;
    philosophers[0].config = config;
    for (int i = 1; i < num_philosophers; ++i)
    {
        philosophers[i].id = i + 1;
        philosophers[i].forks.right = &forks[i];
        philosophers[i].forks.left = &forks[i - 1];
        philosophers[i].print_action = print_action;
        philosophers[i].config = config;
    }
    return (philosophers);
}

void drop_forks(t_forks_pair *forks)
{
    pthread_mutex_unlock(forks->left);
    pthread_mutex_unlock(forks->right);
}

void get_forks(t_philo *philo)
{
    if (!philo->forks.has_left_fork)
        philo->forks.has_left_fork = pthread_mutex_lock(philo->forks.left) == 0;
    if (!philo->forks.has_right_fork)
        philo->forks.has_right_fork = pthread_mutex_lock(philo->forks.right) == 0;
    if (philo->forks.has_left_fork && philo->forks.has_right_fork)
    {
        pthread_mutex_lock(philo->print_action);
        print_action(123456, philo->id, HAS_FORKS);
        pthread_mutex_unlock(philo->print_action);
    }
}

void *start_dinner(void *philo)
{
    get_forks((t_philo *)philo);
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

t_status create_philosophers_threads(t_philo *philosophers)
{
    const int num_philosophers = philosophers->config->number_of_philosophers;
    int ret;

    for (int i = 0; i < num_philosophers; ++i)
    {
        ret = pthread_create(&(philosophers[i].thread_id), NULL, &start_dinner, &philosophers[i]);
        if (ret)
        {
            free(philosophers);
            printf("\ncan't create thread :[%d]", ret);
            return ERROR;
        }
    }
    return (OK);
}