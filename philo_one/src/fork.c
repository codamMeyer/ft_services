#include <fork.h>
#include <stdlib.h>

pthread_mutex_t *create_forks(int num_forks)
{
    pthread_mutex_t *forks;
    int i;

    forks = malloc(sizeof(pthread_mutex_t) * num_forks);
    if (!forks)
        return (NULL);
    i = 0;
    while (i < num_forks)
    {
        if (pthread_mutex_init(&forks[i], NULL))
        {
            free(forks);
            return (NULL);
        }
        ++i;
    }
    return (forks);
}