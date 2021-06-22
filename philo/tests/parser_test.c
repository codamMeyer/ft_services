#include "ctest.h"
#include <parser.h>

CTEST(parser_test, less_args_then_expected)
{
    const char *argv[] = {};
    int argc = 4;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, more_args_then_expected)
{
    const char *argv[] = {};
    int argc = 7;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, invalid_first_arg)
{
    const char *argv[] = {"philo_one", "500a", "200", "20", "300"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, invalid_second_arg)
{
    const char *argv[] = {"philo_one", "500", "-200", "20", "300"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, invalid_third_arg)
{
    const char *argv[] = {"philo_one", "500", "200", "+20", "300"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, invalid_fourth_arg)
{
    const char *argv[] = {"philo_one", "500", "200", "20", "30w0"};
    int argc = 5;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, invalid_fifth_arg)
{
    const char *argv[] = {"philo_one", "500", "200", "20", "300", "d"};
    int argc = 6;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_FALSE(config.initialized);
}

CTEST(parser_test, correct_args)
{
    const char *argv[] = {"philo_one", "500", "200", "20", "300", "50"};
    int argc = 6;

    t_optional_philo_config config = parse_config_args(argc, argv);
    ASSERT_TRUE(config.initialized);
    ASSERT_EQUAL(config.config.number_of_philosophers, 500);
    ASSERT_EQUAL(config.config.time_to_die.value, 200);
    ASSERT_EQUAL(config.config.time_to_eat.value, 20);
    ASSERT_EQUAL(config.config.time_to_sleep.value, 300);
    ASSERT_EQUAL(config.config.min_meals, 50);
}