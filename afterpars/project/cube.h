# ifndef CUBE_H
# define CUBE_H

# include <math.h>
# include <string.h>
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
#define NUM_RAYS 3100
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

#define MOVE_SPEED 0.17899999
#define ROTATION_SPEED 0.17899999

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

typedef struct s_player
{
	double			pp_x;
	double			pp_y;
	double			player_angle;
} t_player;

typedef struct s_wall
{
	double				wall_end; // check if double is needed instead of int
	double				wall_start; // check if double is needed instead of int
	double			wall_distance;
	double				wall_height; // check if double is needed instead of int

} t_wall;

typedef struct s_ray
{
	double			ray_angle;
	double			ray_x;
	double			ray_y;
	double 			ray_dir_y;
    double 			ray_dir_x;
} t_ray;

typedef struct s_game
{
	char			**map;
	int				map_H;
	int				map_W;
	double			max_distance;
	double			fov;
	int				screen_x;
	int				screen_y;
	double			screenWidth;
	double			screenHeight;
    void			*mlx_ptr;
    void			*wid_ptr;
	double			camerax; // to check later if needed
	double			cameray; // to check later if needed
	struct s_player *player;
	struct s_wall *wall;
	struct s_img *img;
	struct s_ray *ray;
}			t_game;

char	**ft_split(char const *s, char c);
t_img *create_image(t_game *game);
int is_wall(t_game *game, double new_x, double new_y);
void draw_player(t_game *game);
void draw_all_rays(t_game *game);
void draw_ray(t_game *game, t_player *player, int ray_color);
void draw_map(t_game *game);
void clear_image(t_game *game, int color);
int game_loop(t_game *game);
int key_handler(int keycode, t_game *game);
char	**ft_split_n(char *s, char c);
char	*get_map(int fd);
t_game	*tmap(void);
void set_player_direction(char c, t_game *game);
void put_pixel_safe(t_game *game, int x, int y, int color);
int close_window(t_game *game);
void	*w_malloc(size_t size);
void	ft_lstc(t_addr_lst **lst);
t_addr_lst	**get_garbage_collecter(void);

// 1st question: m i the one who needs to adress the P in the map using calculations? or it just need to be in the given map as an element 'P'
#endif