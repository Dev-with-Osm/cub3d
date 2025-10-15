NAME = cube3d
SRCS = parsing/comp_rows.c parsing/header_core.c parsing/map_parse.c parsing/map_validate.c parsing/scan_tokens.c parsing/ff_bfs.c parsing/map_norm.c parsing/map_parse_utils.c parsing/parse_utils.c parsing/player_validate.c main_full.c get_next_line/get_next_line_utils.c get_next_line/get_next_line.c
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -g3 -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)
