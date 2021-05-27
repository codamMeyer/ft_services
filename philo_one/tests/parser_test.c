#include "ctest.h"
#include <parser.h>

CTEST(parser_test, no_args)
{
    const char *argv[] = {};
    int argc = 4;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, more_args)
{
    const char *argv[] = {};
    int argc = 7;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, negative_args)
{
    const char *argv[] = {"philo_one", "500", "200", "-20", "300"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, non_numeric_args)
{
    const char *argv[] = {"philo_one", "500", "200", "20a", "300"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}


CTEST(parser_test, correct_args)
{
    const char *argv[] = {"philo_one", "500", "200", "20", "300", "50"};
    int argc = 6;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_TRUE(config.initialized);
    ASSERT_EQUAL(config.config.number_of_philosophers.value, 500);
    ASSERT_EQUAL(config.config.time_to_die.value, 200);
    ASSERT_EQUAL(config.config.time_to_eat.value, 20);
    ASSERT_EQUAL(config.config.time_to_sleep.value, 300);
    ASSERT_EQUAL(config.config.min_meals.value, 50);
}