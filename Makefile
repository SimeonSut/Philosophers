CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g

SRC = main.c input_check.c input_check_utils.c routine.c routine_utils.c time_and_states.c cycles.c

HDR = philosophers.h

NAME = philo

all: $(NAME)

$(NAME): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	@rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re