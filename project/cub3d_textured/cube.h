# ifndef CUBE_H
# define CUBE_H

# include <math.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include "get_next_line/get_next_line.h"
// # include "libft/libft.h"
#ifdef __APPLE__
#include "mlx/mlx.h"
#else
# include <mlx.h>
# include <X11/Xlib.h>
#endif

#define PI 3.14159265358979323846264338327950288
#define NUM_RAYS 2000
#define FOV_DEGREES 60
#define tile 72
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0) // used to just test things, will be removed later
#define WALL_HEIGHT_MULTIPLIER 1.0
#define RED     0xFF0000    // Pure red
#define GREEN   0x00FF00    // Pure green  
#define BLUE    0x0000FF    // Pure blue
#define WHITE   0xFFFFFF    // White
#define BLACK   0x000000    // Black
#define GRAY    0x808080    // Gray
#define YELLOW  0xFFFF00    // Yellow (red + green)
#define PURPLE  0xFF00FF    // Purple (red + blue)

# define KEY_ESC 53
# define KEY_W 13
# define KEY_S 1
# define KEY_A 0
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define TILE_SIZE 32
# define ARROW_RIGHT 65363
# define ARROW_LEFT 65361
# define ARROW_UP 65362
# define ARROW_DOWN 65364

#define MOVE_SPEED 0.09998888
#define ROTATION_SPEED 0.05999

typedef struct s_addr_lst
{
	void				*data;
	struct s_addr_lst	*next;
}						t_addr_lst;

typedef struct s_img
{
    void *img_ptr;        // MLX image pointer
    char *img_data;       // Raw pixel data array
    int bits_per_pixel;   // Usually 32 (4 bytes per pixel)
    int line_length;      // Bytes per screen row
    int endian;           // Byte order (usually 0)
} t_img;

typedef struct s_time
{
	long frame_sleep_time;
	long elapsed_microseconds;
	double sleep_time;

} t_time;

typedef struct s_player
{
	float			pp_x;
	float			pp_y;
	float			player_angle;
} t_player;

typedef struct s_wall
{
	float				wall_end; // check if float is needed instead of int
	float				wall_start; // check if float is needed instead of int
	float			wall_distance;
	float				wall_height; // check if float is needed instead of int

} t_wall;

typedef struct s_ray
{
	float			ray_angle;
	float			ray_x;
	float			ray_y;
	float 			ray_dir_y;
    float 			ray_dir_x;
} t_ray;

typedef struct s_keys{
    int w_pressed;
    int s_pressed;
    int a_pressed;
    int d_pressed;
    int left_pressed;
    int right_pressed;
} t_keys;

typedef struct s_texture
{
    void *img_ptr;
    char *img_data;
    int bits_per_pixel;
    int line_length;
    int endian;
    int width;
    int height;
} t_texture;

typedef struct s_game
{
	char			**map;
	int				map_H;
	int				map_W;
	float			max_distance;
	float			fov;
	int				screen_x;
	int				screen_y;
	float			screenWidth;
	float			screenHeight;
    void			*mlx_ptr;
    void			*wid_ptr;
	float			camerax; // to check later if needed
	float			cameray; // to check later if needed
	struct s_player *player;
	struct s_wall *wall;
	struct s_img *img;
	struct s_ray *ray;
	struct s_keys *keys;
	struct s_time *tms;
    t_texture *wall_texture;
    t_texture *floor_texture;
    t_texture *ceiling_texture;
}			t_game;

char	**ft_split(char const *s, char c);
t_img *create_image(t_game *game);
int is_wall(t_game *game, float new_x, float new_y);
void draw_player(t_game *game);
void draw_all_rays(t_game *game);
void draw_ray(t_game *game, t_player *player, int ray_color);
void draw_map(t_game *game);
void clear_image(t_game *game, int color);
int game_loop(t_game *game);
int key_press(int keycode, t_game *game);
int key_release(int keycode, t_game *game);
void process_movement(t_game *game);
char	**ft_split_n(char *s, char c);
char	*get_map(int fd);
void set_player_direction(char c, t_game *game);
void put_pixel_safe(t_game *game, int x, int y, int color);
int close_window(t_game *game);
void	*w_malloc(size_t size);
void	ft_lstc(t_addr_lst **lst);
t_addr_lst	**get_garbage_collecter(void);
void load_textures(t_game *game);

// 1st question: m i the one who needs to adress the P in the map using calculations? or it just need to be in the given map as an element 'P'
#endif