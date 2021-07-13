PHILO=philo_bonus
CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3 -ggdb3 -fsanitize=address
TEST_CFLAGS=$(CFLAGS)
INC_PATH=-I./src
LDFLAGS=-lpthread

PHILO_INCS=							\
	src/parser.h					\
	src/types.h						\
	src/time_utils.h				\
	src/dinner.h					\
	src/display.h					\

PHILO_SRCS=							\
	src/parser.c					\
	src/time_utils.c				\
	src/dinner.c					\
	src/display.c					\


TEST_FILES=							\
	tests/parser_test.c 			\
	tests/philo_test.c	 			\
	tests/main.c 					\

PHILO_OBJS=$(PHILO_SRCS:.c=.o)

all: $(PHILO)

$(PHILO): $(PHILO_OBJS)
		$(CC) $(CFLAGS) $(INC_PATH) main.c  -o $@ $^ $(LDFLAGS)

%.o: %.c $(PHILO_INCS)
	@$(CC) $(CFLAGS) $(INC_PATH) -c -o $@ $<

test_integration: $(PHILO)
	./tests/integration_test.py

clean:
	rm -f $(PHILO_OBJS)

re: fclean all

fclean: clean
	rm -f $(PHILO)
	rm -f norminette_result

.PHONY: all clean fclean re test libftprintf
