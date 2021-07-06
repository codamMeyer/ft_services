#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SEM_NAME "/semaphore_example"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SEM_FLAGS ( O_CREAT | O_EXCL)
#define INITIAL_VALUE 3
#define ITERS 2
#define NUM_PHILO 4

typedef struct s_philosopher
{
	int    		id;
	pid_t  		pid_id;
	sem_t   	*sem;
	pthread_t	thread_id;
}   t_philosopher;

void *start_dinner(void *philosopher)
{
	int i = 0;
	t_philosopher *philo;

	philo = (t_philosopher *)philosopher;
	while (i++ < 5) {
		if (sem_wait(philo->sem) < 0) {
			perror("child sem_wait(3) failed on child");
			continue;
		}

		printf("Philo %d got a fork\n", philo->id);
		usleep(2000);
		if (sem_post(philo->sem) < 0) {
			perror("child sem_post(3) error on child");
		}
		printf("Philo %d started sleeping\n", philo->id);
		usleep(500000);
	}
	if (sem_close(philo->sem) < 0)
		perror("child sem_close(3) failed");

	return (NULL);
}

int create_philo_thread(t_philosopher *philo)
{
	int ret;
	philo->sem = sem_open(SEM_NAME, O_RDWR);
	if (philo->sem == SEM_FAILED) {
		perror("child sem_open(3) failed");
		exit(EXIT_FAILURE);
	}
	ret = pthread_create(&(philo->thread_id), NULL, &start_dinner, philo);
	printf("created thread philo %d\n", philo->id);
	if (ret)
		printf("Error\n");
	pthread_join(philo->thread_id, NULL);
	exit(0);
	return 0;
}

int main(void)
{
	if (sem_unlink(SEM_NAME) < 0)
		perror("sem_unlink(3) failed");
	sem_t *semaphore = sem_open(SEM_NAME, SEM_FLAGS, SEM_PERMS, INITIAL_VALUE);

	if (semaphore == SEM_FAILED) {
		perror("sem_open(3) error");
		exit(EXIT_FAILURE);
	}

	if (sem_close(semaphore) < 0) {
		perror("sem_close(3) failed");
		sem_unlink(SEM_NAME);
		exit(EXIT_FAILURE);
	}

	t_philosopher philosophers[NUM_PHILO];
	size_t i;

	for (i = 0; i < NUM_PHILO; i++) {
		if ((philosophers[i].pid_id = fork()) < 0) {
			perror("fork(2) failed");
			exit(EXIT_FAILURE);
		}

		if (philosophers[i].pid_id == 0) {
			philosophers[i].id = i + 1;
			if (create_philo_thread(&philosophers[i]) < 0) {
				perror("execl(2) failed");
				exit(EXIT_FAILURE);
			}
			break ;
		}
	}

	for (int j = 0; j < NUM_PHILO; j++) {
		if (waitpid(-1, NULL, 0) < 0)
		{
			printf("waitpid PID %ld\n", (long) getpid());
			perror("waitpid(2) failed");
		}
	}
	return 0;
}