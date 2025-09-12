CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
SRC = main.c  sp.c draw.c helpers.c
# drawing.c wall.c utils.c ft_split_n.c getmp.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c 
UNAME         = $(shell uname)

ifeq ($(UNAME), Linux)
	LINKERS = -lmlx -lXext -lX11 -lm 
else
	LINKERS = -L mlx -lmlx -lm  -framework OpenGL -framework AppKit
endif

NAME = cube
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) $(LINKERS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJ)

# LIBFT =	libft.a

# all: $(NAME)

# $(NAME): $(OBJ) $(LIBFT)
# 	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LINKERS) -o $(NAME)

# $(LIBFT):
# 	make -C libft bonus
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ)
# 	make -C libft clean

# fclean: clean
# 	rm -f $(NAME)
# 	rm -rf $(LIBFT)
# 	make -C libft fclean

# re: fclean $(NAME)

# .PHONY: clean

# .SECONDARY: $(OBJ)

# -Lmlx -lmlx -framework OpenGL -framework AppKit -> to be added before the  -o $(NAME)
