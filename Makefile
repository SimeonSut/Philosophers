CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g

SRC = main.c utils.c input_check.c

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