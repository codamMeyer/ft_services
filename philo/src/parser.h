#ifndef PARSER_H
# define PARSER_H
# include <types.h>

typedef enum e_config_option
{
	NUM_PHILO = 1,
	DIE = 2,
	EAT = 3,
	SLEEP = 4,
	MIN_MEALS = 5
}	t_config_option;

t_optional_philo_config	parse_config_args(int argc, const char *argv[]);

#endif