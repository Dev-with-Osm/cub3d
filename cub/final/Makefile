NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I.

# Platform detection
MLX_FLAGS = -L/usr/lib -lmlx -lXext -lX11 -lm

SRCS = raycast.c render.c textures.c garbage_collector.c \
       get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
	   render_helpers.c wall_column.c floor_ceiling_helpers.c main_helper.c \
	   movement.c movement_helpers.c parsing/header_core.c parsing/memory_helpers.c \
	   parsing/map_parse.c parsing/map_validate.c parsing/scan_tokens.c parsing/ff_bfs.c \
	   parsing/map_norm.c parsing/map_parse_utils.c parsing/parse_utils.c parsing/player_validate.c \
	   parsing/comp_rows.c parsing.c main.c helpers.c game_loop.c \
	   ./libft/ft_bzero.c  ./libft/ft_calloc.c  ./libft/ft_isdigit.c \
		./libft/ft_memcpy.c  ./libft/ft_memset.c  ./libft/ft_strlen.c  ./libft/ft_strncmp.c

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