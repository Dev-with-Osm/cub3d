/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:15:00 by okhourss          #+#    #+#             */
/*   Updated: 2025/12/18 19:00:55 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../get_next_line/get_next_line.h"
# include <ctype.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_rgb
{
	int				r;
	int				g;
	int				b;
}					t_rgb;

typedef enum e_tex_id
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_MAX
}					t_tex_id;

typedef struct s_config
{
	char			*tex[TEX_MAX];
	int				has_tex[TEX_MAX];
	int				floor_set;
	int				ceil_set;
	t_rgb			floor_rgb;
	t_rgb			ceil_rgb;
	int				header_done;
}					t_config;

typedef struct s_map
{
	int				map_W;
	int				map_H;
	char			**map;
	int				player_x;
	int				player_y;
	char			player_angle;
}					t_map;

typedef struct s_ff
{
	int				w;
	int				h;
	int				ww;
	int				hh;
	char			*grid;
	unsigned char	*vis;
	int				*qx;
	int				*qy;
	int				qh;
	int				qt;
}					t_ff;

typedef struct s_build
{
	int				w;
	int				h;
	int				px;
	int				py;
}					t_build;

typedef struct s_player
{
	int				x;
	int				y;
	char			dir;
}					t_player;

int					p_err(const char *msg);
int					p_is_space(char c);
void				p_skip_spaces(const char **p);
int					p_is_blank(const char *s);
int					p_check_cub_ext(const char *filename);

int					p_dup_token(const char *beg, size_t len, char **out);
int					p_take_token(const char **p, const char **beg, size_t *len);
int					p_expect_eol(const char **p);
int					p_read_uint_255(const char **p, int *out);
int					p_expect_char(const char **p, char c);

int					parse_tex_line(const char *s, char **out);
int					parse_rgb_line(const char *s, t_rgb *out);
int					dispatch_header_line(char *line, t_config *cfg);
void				free_config(t_config *cfg);

int					parse_cub_file(const char *filename, t_config *cfg,
						t_map *map);
void				free_map(t_map *m);

int					comp_map(char ***map, int *h, char ***buf, int n);
int					handle_color_line(const char *p, t_config *cfg,
						int is_floor);

int					find_player(char **map, int h, t_player *out);
void				player_to_floor(char **map, int px, int py);
int					ff_alloc(t_ff *f, char **map, int w, int h);
void				ff_seed_border(t_ff *f);
void				ff_bfs_air(t_ff *f);

int					append_line(char ***buf, int *n, char *line);
void				mark_header_done(t_config *cfg);
int					is_header_directive(const char *p);

int					pad_map(char ***map, int *h, int *w);
int					load_lines(const char *name, t_config *cfg, char ***buf,
						int *n);
int					collect_map_lines(int fd, char ***buf, int *n,
						t_config *cfg);

void				free_lines_buf(char **buf, int n);
void				free_map_n(char **map, int h);

#endif
