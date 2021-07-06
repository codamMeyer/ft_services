#include <stdio.h>
#include <parser.h>

int	main(int argc, const char *argv[])
{
	t_optional_philo_config	optional;
	t_status				ret;

	optional = parse_config_args(argc, argv);
	if (!optional.initialized)
	{
		// display_usage_message();
		printf("Wrong args\n");
		return (ERROR);
	}
	if (optional.config.number_of_philosophers < 2)
	{
		printf("Invalid Number of Philosophers\n");
		return (ERROR);
	}
	// ret = run(&optional.config);
	return (ret);
}
