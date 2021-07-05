#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SEM_NAME "/semaphore_example"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 3
#define ITERS 5

int execl_child()
{
    sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        perror("child sem_open(3) failed");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < ITERS; i++) {
        if (sem_wait(semaphore) < 0) {
            perror("child sem_wait(3) failed on child");
            continue;
        }

        printf("child PID %ld acquired semaphore\n", (long) getpid());
        usleep(5000);
        if (sem_post(semaphore) < 0) {
            perror("child sem_post(3) error on child");
        }
        printf("child PID %ld released semaphore\n", (long) getpid());

        sleep(1);
    }

    if (sem_close(semaphore) < 0)
        perror("child sem_close(3) failed");

    return 0;
}


int main(void) {

    /* We initialize the semaphore counter to 1 (INITIAL_VALUE) */
    if (sem_unlink(SEM_NAME) < 0)
        perror("sem_unlink(3) failed");
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE);

    if (semaphore == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }

    /* Close the semaphore as we won't be using it in the parent process */
    if (sem_close(semaphore) < 0) {
        perror("sem_close(3) failed");
        /* We ignore possible sem_unlink(3) errors here */
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }

    pid_t pids[2];
    size_t i;

    for (i = 0; i < sizeof(pids)/sizeof(pids[0]); i++) {
        if ((pids[i] = fork()) < 0) {
            perror("fork(2) failed");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            if (execl_child() < 0) {
                perror("execl(2) failed");
                exit(EXIT_FAILURE);
            }
        }
        else if (pids[i] > 0)
        {
            if (waitpid(-1, NULL, 0) < 0)
            {
                printf("waitpid PID %ld\n", (long) getpid());
                perror("waitpid(2) failed");
            }    
        }
    }

    return 0;
}