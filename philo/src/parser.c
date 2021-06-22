#include <parser.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>

static t_bool	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static t_optional_int	parse_number(const char *nbr)
{
	int				i;
	t_optional_int	number;

	number.value = 0;
	number.initialized = TRUE;
	i = 0;
	if (!ft_isdigit(nbr[i]))
		number.initialized = FALSE;
	while (ft_isdigit(nbr[i]))
	{
		number.value = (nbr[i] - '0') + (number.value * 10);
		++i;
	}
	if (nbr[i] != '\0')
		number.initialized = FALSE;
	return (number);
}

static t_optional_philo_config	create_uninitialized_config(void)
{
	const t_optional_philo_config	config = {.initialized = FALSE};

	return (config);
}

static void	set_config(const char *str_arg[],
							t_config_option option,
							t_optional_philo_config *configuration)
{
	t_optional_int	arg;

	arg = parse_number(str_arg[option]);
	if (option == NUM_PHILO)
		configuration->config.number_of_philosophers = arg.value;
	if (option == DIE)
		configuration->config.time_to_die.value = arg.value;
	if (option == EAT)
		configuration->config.time_to_eat.value = arg.value;
	if (option == SLEEP)
		configuration->config.time_to_sleep.value = arg.value;
	if (option == MIN_MEALS)
		configuration->config.min_meals = arg.value;
	configuration->initialized &= arg.initialized;
}

t_optional_philo_config	parse_config_args(int argc, const char *argv[])
{
	t_optional_philo_config		optional;
	const t_bool				correct_num_args = (argc == 5 || argc == 6);

	if (!correct_num_args)
		return (create_uninitialized_config());
	optional.initialized = TRUE;
	set_config(argv, NUM_PHILO, &optional);
	set_config(argv, DIE, &optional);
	set_config(argv, EAT, &optional);
	set_config(argv, SLEEP, &optional);
	if (argc == 6)
		set_config(argv, MIN_MEALS, &optional);
	else
		optional.config.min_meals = NOT_SET;
	optional.config.death_event = FALSE;
	optional.config.need_to_finish_meals = \
		optional.config.number_of_philosophers;
	gettimeofday(&optional.config.time_start, NULL);
	return (optional);
}
