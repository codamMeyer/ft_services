#include "ctest.h"
#include <philosopher.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

CTEST_DATA(thread_test)
{
    t_philo_config config;
    t_display	display;
	t_fork		*forks;
	t_philo		*philosophers;
};

CTEST_SETUP(thread_test)
{
    data->config.death_event = FALSE;
    data->config.number_of_philosophers = 5;
    data->config.time_to_die.value = 410;
    data->config.time_to_eat.value = 60;
    data->config.time_to_sleep.value = 60;
    data->display.lock = malloc(sizeof(pthread_mutex_t));
	data->display.is_used = FALSE;
    data->forks = create_forks(data->config.number_of_philosophers);
    data->philosophers = create_philosophers(&data->config, data->forks, &data->display);
};

CTEST_TEARDOWN(thread_test)
{
    for (int i = 0; i < data->config.number_of_philosophers; ++i)
    {
        pthread_mutex_destroy(data->forks[i].lock);
        free(data->forks[i].lock);
    }
    free(data->forks);
    free(data->display.lock);
    free(data->philosophers);
};

int create_fake_thread_first(pthread_t *thread, const pthread_attr_t * attr, t_start_routine routine, void * ptr)
{
    static int i = 0;

    if (i++ == 0)
        return 1;
    else
        return pthread_create(thread, attr, routine, ptr);
}

CTEST2(thread_test, first_philo_theard_fails)
{
    ASSERT_EQUAL(ERROR, create_philosophers_threads(data->philosophers, create_fake_thread_first));
}

int create_fake_thread_second(pthread_t *thread, const pthread_attr_t * attr, t_start_routine routine, void * ptr)
{
    static int i = 0;

    if (i++ == 1)
        return 1;
    else
        return pthread_create(thread, attr, routine, ptr);
}

CTEST2(thread_test, second_philo_theard_fails)
{
    ASSERT_EQUAL(ERROR, create_philosophers_threads(data->philosophers, create_fake_thread_second));
	int	i;

	i = 0;
	while (i < data->config.initialized_threads)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		++i;
	}
}

int create_fake_thread_last(pthread_t *thread, const pthread_attr_t * attr, t_start_routine routine, void * ptr)
{
    static int i = 0;

    if (i++ == 4)
        return 1;
    else
        return pthread_create(thread, attr, routine, ptr);
}

CTEST2(thread_test, last_philo_theard_fails)
{
    ASSERT_EQUAL(ERROR, create_philosophers_threads(data->philosophers, create_fake_thread_last));
	int	i;

	i = 0;
	while (i < data->config.initialized_threads)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		++i;
	}
}