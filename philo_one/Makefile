PHILO=philo_one
TEST_NAME=$(PHILO)_test
CC=gcc
CFLAGS= -O3 -Wall -Wextra -Werror
TEST_CFLAGS=-ggdb3 $(CFLAGS)
INC_PATH=-I./
LDFLAGS=-lpthread

PHILO_SRC=

TEST_FILES=									\
	test/main.c 							\

PHILO_OBJS = $(PHILO_SRC:.c=.o)

all: $(PHILO)

$(PHILO): $(PHILO_OBJS)
		$(CC) $(CFLAGS) $(INC_PATH) main.c -o $@ $^ $(LDFLAGS)

%.o: %.c 
		$(CC) $(CFLAGS) $(INC_PATH) -c -o $@ $< 

test_run: test
	./$(TEST_NAME)
	../run_norminette.sh $(PHILO)

# test_integration: $(FT_PRINTF) $(PUSH_SWAP) $(CHECKER)
# 	./test/integration_test.py

test: $(PHILO) $(TEST_FILES)
	$(CC) $(TEST_CFLAGS) $(INC_PATH) $(PHILO_OBJS) $(TEST_FILES) -o $(TEST_NAME) $(LDFLAGS)

clean:
	rm -f $(PHILO_OBJS)

re: fclean all

fclean: clean
	rm -f $(PHILO)
	rm -f $(TEST_NAME)
	rm -f norminette_result

.PHONY: all clean fclean re test libftprintf
