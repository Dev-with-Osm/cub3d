NAME = cube3d
SRCS = parsing/parsing.c
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra #-Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lmlx -lX11 -lXext -lm -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)