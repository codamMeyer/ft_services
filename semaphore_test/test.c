#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define WCONTINUED 8
#define NUM_PHILO 4
const char *semName = "testForks";

typedef struct s_config
{
    int num_philo;
    sem_t *forks_sem_id;
}   t_config;

typedef struct s_philo
{
    int         id;
    pid_t       pid_id;
	pthread_t   thread_id;
    t_config    *config;
}   t_philo;

void get_forks(int id, sem_t *sem_id)
{
    sem_wait(sem_id);
    sem_wait(sem_id);
    printf("    %d Got forks\n", id);
    printf("    %d eating\n", id);
    usleep(900000);
    sem_post(sem_id);
    sem_post(sem_id);
    printf("%d Dropped forks\n", id);
}

void child(int id, sem_t *sem_id)
{
    if (sem_id == SEM_FAILED){
        perror("Child   : [sem_open] Failed\n"); return;        
    }
    printf("Child %d : started \n", id);
    get_forks(id, sem_id);
}

int main()
{
    t_config config = {
        .num_philo = NUM_PHILO,
        .forks_sem_id = sem_open(semName, O_CREAT, 0600, NUM_PHILO),
    };

    sem_unlink(semName);

    for (int i = 0; i < 3; i++)
    {
        pid_t pid;
        pid = fork();
        if (pid < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0){
            child(i, config.forks_sem_id);
            printf("Child %d : Done\n", i);
            break ;
        }
        else{
            int status;
            waitpid(pid, &status, WUNTRACED | WCONTINUED);
        }
    }
    sem_close(config.forks_sem_id);
    return 0;
}