#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <fork.h>
#include <types.h>
#include <philosopher.h>
#include <pthread.h>
#include <unistd.h>

void destroy_mutexes(t_philo *philosophers, int num_philosophers)
{
    for (int i = 0; i < num_philosophers; ++i)
        pthread_mutex_destroy(philosophers[i].forks.right);
}

void join_threads(t_philo *philosophers, int num_philosophers)
{
    for (int i = 0; i < num_philosophers; ++i)
        pthread_join(philosophers[i].thread_id, NULL);
}

t_status run(const t_philo_config *config)
{
    pthread_mutex_t print_action;
    if (pthread_mutex_init(&print_action, NULL))
    {
        printf("\n mutex init failed\n");
        return ERROR;
    }
    pthread_mutex_t *forks = create_forks(config->number_of_philosophers);
    t_philo *philosophers = create_philosophers(config->number_of_philosophers, forks, &print_action);

    create_philosophers_threads(philosophers, config->number_of_philosophers);

    join_threads(philosophers, config->number_of_philosophers);
    destroy_mutexes(philosophers,config->number_of_philosophers);
    
    free(forks);
    free(philosophers);
    return (OK);
}

int main(int argc, const char *argv[])
{
    const t_optional_philo_config optional_config = parse_config_args(argc, argv);
    t_status ret;

    if (!optional_config.initialized)
    {
        printf("Usage:\n\t./philo_one <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }
    ret = run(&optional_config.config);
    return (ret);
}