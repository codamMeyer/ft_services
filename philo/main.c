#include <stdio.h>
#include <display.h>
#include <parser.h>
#include <dinner.h>
#include <unistd.h>

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
		write(STDERR_FILENO, "Invalid Number of Philosophers\n", 31);
		return (ERROR);
	}
	ret = run(&optional.config);
	return (ret);
}
