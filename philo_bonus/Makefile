PHILO=philo_bonus
CC=clang
CFLAGS=-Wall -Wextra -Werror -O2
TEST_CFLAGS=$(CFLAGS)
INC_PATH=-I./src
LDFLAGS=-lpthread

PHILO_INCS=							\
	src/parser.h					\
	src/types.h						\
	src/time_utils.h				\
	src/dinner.h					\
	src/display.h					\
	src/philosopher.h				\
	src/eat.h						\
	src/sleep.h						\
	src/death.h						\


PHILO_SRCS=							\
	src/parser.c					\
	src/time_utils.c				\
	src/dinner.c					\
	src/display.c					\
	src/philosopher.c				\
	src/eat.c						\
	src/sleep.c						\
	src/death.c						\


TEST_FILES=							\
	tests/parser_test.c 			\
	tests/philo_test.c	 			\
	tests/main.c 					\

PHILO_OBJS=$(PHILO_SRCS:.c=.o)

all: $(PHILO)

$(PHILO): $(PHILO_OBJS)
		$(CC) $(CFLAGS) $(INC_PATH) main.c -o $@ $^ $(LDFLAGS)

%.o: %.c $(PHILO_INCS)
	@$(CC) $(CFLAGS) $(INC_PATH) -c -o $@ $<

test_integration: $(PHILO)
	python3 ./tests/integration_test.py
	@../run_norminette.sh

clean:
	rm -f $(PHILO_OBJS)

re: fclean all

fclean: clean
	rm -f $(PHILO)
	rm -f norminette_result

.PHONY: all clean fclean re test libftprintf
