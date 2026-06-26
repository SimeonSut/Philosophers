CC = cc

CFLAGS = -Wall -Wextra -Werror -g

SRC = main.c util.c input_check.c

HDR = philosophers.h

NAME = philo

all: $(NAME)

$(NAME) = $(SRC) $(HDR)
	@$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	@rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re