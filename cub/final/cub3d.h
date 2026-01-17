/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:05:04 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/17 18:07:40 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <ctype.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>
# include <stdlib.h>
# include <mlx.h>
# include <X11/Xlib.h>

# include "get_next_line/get_next_line.h"

# define PI 3.14159265358979323846264338327950288 
# define FOV 60 
# define LINUX_KEY_ESC 65307
# define LINUX_KEY_LEFT 65361 
# define LINUX_KEY_RIGHT 65363
# define KEY_ESC 65307
# define KEY_W 119      
# define KEY_S 115      
# define KEY_A 97       
# define KEY_D 100      
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define TILE_SIZE 32
# define SCREEN_H 780
# define SCREEN_W 1480
# define NORTH 0
# define SOUTH 1
# define EAST  2
# define WEST  3
# define M_SPEED 0.03
# define ROT_SPEED 0.04

typedef struct s_addr_lst
{
	void				*data;
	struct s_addr_lst	*next;
}	t_addr_lst;

typedef struct s_dda
{
	int					dda_hit;
	int					map_x;
	int					map_y;
	float				move_rate_x;
	float				move_rate_y;
	float				side_dist_x;
	float				side_dist_y;
	float				delta_dist_x;
	float				delta_dist_y;
	int					step_x;
	int					step_y;
	int					side;
}	t_dda;

typedef struct s_img
{
	void				*img_ptr;
	char				*img_data;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}	t_img;

typedef struct s_time
{
	long				frame_sleep_time;
	long				elapsed_microseconds;
	double				sleep_time;
	long				last_frame_time;
}	t_time;

typedef struct s_texture
{
	void				*img_ptr;
	char				*img_data;
	int					width;
	int					height;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}	t_texture;

typedef struct s_textures
{
	t_texture			*north;
	t_texture			*south;
	t_texture			*east;
	t_texture			*west;
	int					floor_color;
	int					ceiling_color;
}	t_textures;

typedef struct s_wall
{
	float				wall_end;
	float				wall_start;
	float				wall_distance;
	float				wall_height;
	float				wall_x;
	int					wall_face;
	int					draw_start;
	int					draw_end;
}	t_wall;

typedef struct s_ray
{
	float				ray_angle;
	float				ray_x;
	float				ray_y;
	float				move_rate_y;
	float				move_rate_x;
}	t_ray;

typedef struct s_keys
{
	int					w_pressed;
	int					s_pressed;
	int					a_pressed;
	int					d_pressed;
	int					left_pressed;
	int					right_pressed;
}	t_keys;

typedef struct s_map
{
	float				player_x;
	float				player_y;
	float				player_angle;
	char				player_direction;
	int					map_height;
	int					map_width;
	char				**map;
}	t_map;

typedef struct s_game
{
	t_map				*map;
	t_wall				*wall;
	t_img				*img;
	t_ray				*ray;
	t_keys				*keys;
	t_time				*tms;
	t_dda				*dda;
	t_textures			*textures;
	float				max_distance;
	float				fov;
	float				screen_width;
	float				camerax;
	float				cameray;
	float				screen_height;
	int					screen_x;
	int					screen_y;
	void				*wid_ptr;
	void				*mlx_ptr;
}	t_game;

typedef struct s_rgb
{
	int					r;
	int					g;
	int					b;
}	t_rgb;

typedef enum e_tex_id
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_MAX
}	t_tex_id;

typedef struct s_config
{
	int					has_tex[TEX_MAX];
	char				*tex[TEX_MAX];
	int					floor_set;
	int					ceil_set;
	int					header_done;
	t_rgb				floor_rgb;
	t_rgb				ceil_rgb;
}	t_config;

typedef struct s_ff
{
	int					w;
	int					h;
	int					ww;
	int					hh;
	int					*qx;
	int					*qy;
	int					qh;
	int					qt;
	char				*grid;
	unsigned char		*vis;
}	t_ff;

typedef struct s_build
{
	int					w;
	int					h;
	int					px;
	int					py;
}	t_build;

typedef struct s_player
{
	int					x;
	int					y;
	char				dir;
}	t_player;

/*
** Core
*/
void				rendering(t_game *game);
int					game_loop(t_game *game);
float				cast_ray(t_game *game, float ray_angle);
int					init_structs(t_game *game);
void				delta_distance(t_dda *dda, float angle);
void				side_dist(t_game *game, t_dda *dda, int sign);
void				draw_wall_column(t_game *game, int x);
void				draw_floor_ceiling(t_game *game, int x);
void				draw_floor(t_game *game, int x, int wall_end);
void				draw_ceiling(t_game *game, int x, int wall_start);
void				draw_wall_pixels(t_game *game, int x, int tex_x);
void				get_draw_boundaries(t_game *game);
int					get_texture_x(t_game *game, t_texture *tex);
t_texture			*get_wall_texture(t_game *game);
int					apply_shading(int color, float distance, int side);

/*
** Textures
*/
int					load_textures(t_game *game, char **tex);
t_texture			*load_texture(void *mlx_ptr, char *path);
int					get_pixel_color(t_texture *tex, int x, int y);
void				free_mlx_textures(t_game *game);

/*
** MLX / Input
*/
void				decide_side(int *side, int face);
t_img				*create_image(t_game *game);
int					key_press(int keycode, t_game *game);
int					key_release(int keycode, t_game *game);
int					close_window(t_game *game);
void				put_pixel_safe(t_game *game, int x, int y, int color);

/*
** Movement
*/
int					is_wall(t_game *game, float x, float y);
void				set_player_direction(char c, t_game *game);
float				normalize_delta(float delta_time);
float				calculate_move_angle(t_game *game, int f, int s);
void				apply_movement(t_game *game, float angle, float speed);
void				apply_rotation(t_game *game, float rotation_speed);
void				process_movement(t_game *game, float delta_time);

/*
** Garbage collector
*/
void				*w_malloc(size_t size);
t_addr_lst			*ft_lstn(void *content);
void				ft_lstadd_f(t_addr_lst **lst, t_addr_lst *ne);
void				ft_lstc(t_addr_lst **lst);
t_addr_lst			**get_garbage_collecter(void);

/*
** Parsing
*/
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

int					parse_cub_file(const char *filename,
						t_config *cfg, t_map *map);
void				free_map(t_map *m);

int					comp_map(char ***map, int *h, char ***buf, int n);
int					handle_color_line(const char *p,
						t_config *cfg, int is_floor);

int					find_player(char **map, int h, t_player *out);
void				player_to_floor(char **map, int px, int py);
int					ff_alloc(t_ff *f, char **map, int w, int h);
void				ff_seed_border(t_ff *f);
void				ff_bfs_air(t_ff *f);

int					append_line(char ***buf, int *n, char *line);
void				mark_header_done(t_config *cfg);
int					is_header_directive(const char *p);

int					pad_map(char ***map, int *h, int *w);
int					load_lines(const char *name,
						t_config *cfg, char ***buf, int *n);
int					collect_map_lines(int fd,
						char ***buf, int *n, t_config *cfg);

void				free_lines_buf(char **buf, int n);
void				free_map_n(char **map, int h);

int					parsing(int argc, char **argv,
						t_game *game, t_config *cfg);

/*
** Libft
*/
size_t				ft_strlen(const char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_strcmp(const char *s1, char *s2);
int					ft_isdigit(int c);

#endif
