#include <stdio.h>
#include <parser.h>
#include <dinner.h>
#include <types.h>
#include <display.h>
#include <semaphore.h>
#include <stdlib.h>

t_bool create_semaphore(int num_forks)
{
	if (sem_unlink(SEM_NAME) < 0)
		perror("sem_unlink(3) failed");
	sem_t *semaphore = sem_open(SEM_NAME, SEM_FLAGS, SEM_PERMS, num_forks);

	if (semaphore == SEM_FAILED) {
		perror("sem_open(3) error");
		return (FALSE);
	}

	if (sem_close(semaphore) < 0) {
		perror("sem_close(3) failed");
		sem_unlink(SEM_NAME);
		return (FALSE);
	}
	create_display();
	return (TRUE);
}

int	main(int argc, const char *argv[])
{
	t_optional_philo_config	optional;
	t_status				ret;

	optional = parse_config_args(argc, argv);
	if (!optional.initialized)
	{
		display_usage_message();
		return (ERROR);
	}
	if (optional.config.number_of_philosophers < 2)
	{
		printf("Invalid Number of Philosophers\n");
		return (ERROR);
	}
	if (!create_semaphore(optional.config.number_of_philosophers))
		return (ERROR);

	optional.config.forks_available = optional.config.number_of_philosophers;
	ret = run(&optional.config);
	return (ret);
}
