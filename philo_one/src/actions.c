#include <actions.h>
#include <stdio.h>

void print_action(long int time, int philo_id, t_action action)
{
    static const char *action_strings[5] = {
                                                "started eating  🍝",
                                                "is sleeping     💤",
                                                "is thinking     💭",
                                                "has two forks   🍴",
                                                "is done eating   ✔️",
                                           };
    printf("| %6ld | philo id %4d | %s |\n", time, philo_id, action_strings[action]);
}