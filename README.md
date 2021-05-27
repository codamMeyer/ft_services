# Philosophers [![codamMeyer](https://circleci.com/gh/codamMeyer/push_swap.svg?style=shield)](https://app.circleci.com/pipelines/github/codamMeyer/philosophers?branch=main)

#### Parser
- [x] parse **number_of_philosophers**
    - is the number of philosophers and also the number of forks

- [x] parse **time_to_die**
    -  is in milliseconds, if a philosopher doesn’t start eating ’time_to_die’ milliseconds after starting his last meal or the beginning of the simulation, it dies
- [x] parse **time_to_eat**
    - is in milliseconds and is the time it takes for a philosopher to eat. During that time he will need to keep the two forks.
- [x] parse **time_to_sleep**
    - is in milliseconds and is the time the philosopher will spend sleeping.
- [x] parse optional  **[number_of_times_each_philosopher_must_eat]**
    - argument is optional, if all philosophers eat at least ’number_of_times_each_philosopher_must_eat’ the simulation will stop. If not specified, the simulation will stop only at the death of a philosopher.
