PHILO=philo
TEST_NAME=$(PHILO)_test
CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3 -ggdb3 -fsanitize=leak -fsanitize=address
TEST_CFLAGS=$(CFLAGS)
INC_PATH=-I./src
LDFLAGS=-lpthread

PHILO_INCS=						\
	src/parser.h				\
	src/types.h					\
	src/philosopher.h			\
	src/fork.h					\
	src/display.h				\
	src/time_utils.h			\
	src/dinner.h				\


PHILO_SRCS=						\
	src/parser.c				\
	src/philosopher.c			\
	src/fork.c					\
	src/display.c				\
	src/time_utils.c			\
	src/philosopher_eat.c		\
	src/philosopher_actions.c	\
	src/dinner.c				\


TEST_FILES=						\
	tests/parser_test.c 		\
	tests/philo_test.c	 		\
	tests/main.c 				\

PHILO_OBJS=$(PHILO_SRCS:.c=.o)

all: $(PHILO)

$(PHILO): $(PHILO_OBJS)
		$(CC) $(CFLAGS) $(INC_PATH) main.c  -o $@ $^ $(LDFLAGS)

%.o: %.c $(PHILO_INCS)
	@$(CC) $(CFLAGS) $(INC_PATH) -c -o $@ $<

test_run: test
	./$(TEST_NAME)
	@../run_norminette.sh

test_integration: $(PHILO)
	./tests/integration_test.py

test: $(PHILO_OBJS) $(TEST_FILES)
	$(CC) $(TEST_CFLAGS) $(INC_PATH) $(PHILO_OBJS) $(TEST_FILES) -o $(TEST_NAME) $(LDFLAGS)

clean:
	rm -f $(PHILO_OBJS)

re: fclean all

fclean: clean
	rm -f $(PHILO)
	rm -f $(TEST_NAME)
	rm -f norminette_result

.PHONY: all clean fclean re test libftprintf
