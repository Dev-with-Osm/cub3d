#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include "get_next_line/get_next_line.h"
#ifdef __APPLE__
#include "mlx/mlx.h"
#else
# include <mlx.h>
# include <X11/Xlib.h>
#endif

#define PI 3.14159265358979323846264338327950288
#define NUM_RAYS 1500
#define FOV_DEGREES 65
#define tile 72
// #define DEG_TO_RAD(deg) ((deg) * PI / 180.0) 
#define WALL_HEIGHT_MULTIPLIER 1.0
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define WHITE   0xFFFFFF
#define BLACK   0x000000
#define GRAY    0x808080
#define YELLOW  0xFFFF00
#define PURPLE  0xFF00FF

// --- KEY DEFINITIONS ---
# define LINUX_KEY_ESC 65307
# define LINUX_KEY_LEFT 65361 
# define LINUX_KEY_RIGHT 65363 
# define MAC_KEY_ESC 53
# define MAC_KEY_LEFT 123
# define MAC_KEY_RIGHT 124
# define MAC_KEY_W 13
# define MAC_KEY_S 1 
# define MAC_KEY_A 0 
# define MAC_KEY_D 2 
# define KEY_ESC 65307
# define KEY_W 119      
# define KEY_S 115      
# define KEY_A 97       
# define KEY_D 100      
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define TILE_SIZE 32

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

#define MOVE_SPEED 0.05
#define ROT_SPEED 0.03
#define PLAYER_Z_HEIGHT 0.5

typedef struct s_addr_lst
{
	void                *data;
	struct s_addr_lst   *next;
}                       t_addr_lst;

typedef struct s_dda
{
	int     dda_hit;
	int     map_x;
	int     map_y;
	float   ray_dir_x;
	float   ray_dir_y;
	float   side_dist_x;
	float   side_dist_y;
	float   delta_dist_x;
	float   delta_dist_y;
	int     step_x;
	int     step_y;
	int     side;
}   t_dda;

typedef struct s_img
{
	void *img_ptr;
	char *img_data;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_img;

typedef struct s_time
{
	long frame_sleep_time;
	long elapsed_microseconds;
	double sleep_time;
	long last_frame_time;
} t_time;

typedef struct s_texture {
	void    *img_ptr;
	char    *img_data;
	int     width;
	int     height;
	int     bits_per_pixel;
	int     line_length;
	int     endian;
} t_texture;

typedef struct s_textures {
    t_texture   *north;
    t_texture   *south;
    t_texture   *east;
    t_texture   *west;
    int         floor_color;      // Changed from t_texture *floor
    int         ceiling_color;    // Changed from t_texture *ceiling
} t_textures;

typedef struct s_wall
{
	float   wall_end;
	float   wall_start;
	float   wall_distance;
	float   wall_height;
	float   wall_x;
	int     wall_face;
} t_wall;

typedef struct s_ray
{
	float           ray_angle;
	float           ray_x;
	float           ray_y;
	float           ray_dir_y;
	float           ray_dir_x;
} t_ray;

typedef struct s_keys{
	int w_pressed;
	int s_pressed;
	int a_pressed;
	int d_pressed;
	int left_pressed;
	int right_pressed;
} t_keys;

typedef struct s_map
{
	char                **map;
	int                 map_H;
	int                 map_W;
	float               pp_x;
	float               pp_y;
	float               player_angle;
}                    t_map;

typedef struct s_game
{
	float           max_distance;
	float           fov;
	int             screen_x;
	int             screen_y;
	float           screenWidth;
	float           screenHeight;
	void            *mlx_ptr;
	void            *wid_ptr;
	float           camerax;
	float           cameray;
	t_map   *map;
	t_wall *wall;
	t_img *img;
	t_ray *ray;
	t_keys *keys;
	t_time *tms;
	t_textures  *textures;
	t_dda *dda;
}           t_game;

// Core functions
void        rendering(t_game *game);
int         game_loop(t_game *game);
float       cast_ray(t_game *game, float ray_angle);
void        draw_wall_column(t_game *game, int x);
void		draw_floor_ceiling(t_game *game, int x);
void        draw_floor(t_game *game, int x, int wall_end);
void        draw_ceiling(t_game *game, int x, int wall_start);
void        draw_wall_pixels(t_game *game, int x, int tex_x, int draw_start, int draw_end);
void        get_draw_boundaries(t_game *game, int *draw_start, int *draw_end);
int			get_texture_x(t_game *game, t_texture *tex);
t_texture *get_wall_texture(t_game *game);
int apply_shading(int color, float distance, int side);


// Texture functions
int         load_textures(t_game *game);
t_texture   *load_texture(void *mlx_ptr, char *path);
void        free_textures(t_game *game);
int         get_pixel_color(t_texture *tex, int x, int y);

// MLX/Input functions
void	    decide_side(int *side, int face);
t_img       *create_image(t_game *game);
int         key_press(int keycode, t_game *game);
int         key_release(int keycode, t_game *game);
int         close_window(t_game *game);
void        put_pixel_safe(t_game *game, int x, int y, int color);
void        clear_image(t_game *game, int color);

// Movement
void        handle_movement(t_game *game, float delta_time);
int         is_wall(t_game *game, float x, float y);
void        set_player_direction(char c, t_game *game);
float       normalize_delta(float delta_time);
float       calculate_move_angle(t_game *game, int forward, int strafe);
void        apply_movement(t_game *game, float angle, float speed);
void        apply_rotation(t_game *game, float rotation_speed);

// Garbage collector
void        *w_malloc(size_t size);
t_addr_lst  *ft_lstn(void *content);
void        ft_lstadd_f(t_addr_lst **lst, t_addr_lst *ne);
void        ft_lstc(t_addr_lst **lst);
t_addr_lst  **get_garbage_collecter(void);

// Map parsing
char        **ft_split(char const *s, char c);
char        **ft_split_n(char *s, char c);
char        *get_map(int fd);
int         init_structs(t_game *game);

#endif