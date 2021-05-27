#include <parser.h>
#include <limits.h>
#include <stdio.h>

static t_bool ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static t_optional_int parse_number(const char *nbr)
{
	int			i;
	t_optional_int number;

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

t_bool is_correct_config(const char *str_arg, t_time_ms *configuration)
{
	t_optional_int arg;

	arg = parse_number(str_arg);
	configuration->value = arg.value;
	return (arg.initialized);
}

t_optional_philo_config parse_config_args(int argc, const char *argv[]) 
{
    t_optional_philo_config	optional;

	if (!(argc == 5 || argc == 6))
	{
		optional.initialized = FALSE;
		return (optional);
	}
	optional.initialized = TRUE;
	optional.initialized &= is_correct_config(argv[1], &optional.config.number_of_philosophers);
	optional.initialized &= is_correct_config(argv[2], &optional.config.time_to_die);
	optional.initialized &= is_correct_config(argv[3], &optional.config.time_to_eat);
	optional.initialized &= is_correct_config(argv[4], &optional.config.time_to_sleep);
	if (argc == 6)
		optional.initialized &= is_correct_config(argv[5], &optional.config.min_meals);
    return (optional);
}
