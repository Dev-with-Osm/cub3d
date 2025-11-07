# CC = cc
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address
# SRC = main.c textures.c helpers.c  sp.c ft_split_n.c getmp.c  garbage_collecta.c \
# 	get_next_line/get_next_line.c get_next_line/get_next_line_utils.c cast_ray.c textures_utils.c movement.c rendring.c
# UNAME         = $(shell uname)

# ifeq ($(UNAME), Linux)
# 	LINKERS = -lmlx -lXext -lX11 -lm 
# else
# 	LINKERS = -L mlx -lmlx -lm  -framework OpenGL -framework AppKit
# endif

# NAME = cube
# OBJ = $(SRC:.c=.o)

# all: $(NAME)

# $(NAME): $(OBJ) 
# 	$(CC) $(CFLAGS) $(OBJ) $(LINKERS) -o $(NAME)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re

# .SECONDARY: $(OBJ)


NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -O2
INCLUDES = -I.

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    MLX_FLAGS = -L./mlx -lmlx -framework OpenGL -framework AppKit
else
    MLX_FLAGS = -L/usr/lib -lmlx -lXext -lX11 -lm
endif

SRCS = main.c raycast.c render.c textures.c movement.c \
       garbage_collector.c map_parser.c \
       get_next_line/get_next_line.c get_next_line/get_next_line_utils.c ft_split_n.c sp.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re