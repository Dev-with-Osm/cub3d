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
// --- NEW LINUX/X11 KEY DEFINES (Assumes W=119, S=115, etc.) ---
# define KEY_ESC 65307  // Consistent X11 Escape
# define KEY_W 119      // Linux keycode for 'w'
# define KEY_S 115      // Linux keycode for 's'
# define KEY_A 97       // Linux keycode for 'a'
# define KEY_D 100      // Linux keycode for 'd'
# define KEY_LEFT 65361 // Linux keycode for Left Arrow
# define KEY_RIGHT 65363 // Linux keycode for Right Arrow
# define TILE_SIZE 32
# define ARROW_RIGHT 65363
# define ARROW_LEFT 65361
# define ARROW_UP 65362
# define ARROW_DOWN 65364

// Add these defines after your color defines
#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

#define MOVE_SPEED 0.05999999
#define ROTATION_SPEED 0.069999999

#define PLAYER_Z_HEIGHT 0.5

typedef struct s_addr_lst
{
    void                *data;
    struct s_addr_lst   *next;
}                       t_addr_lst;

typedef struct s_dda
{
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
    int     side; // 0 for X-side, 1 for Y-side
}   t_dda;

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
    long last_frame_time; // ADDED/KEPT: Tracks time for delta-time movement

} t_time;

typedef struct s_player
{
    float           pp_x;
    float           pp_y;
    float           player_angle;
} t_player;

// --- TEXTURE STRUCT (t_texture) - NO CHANGE NEEDED ---
typedef struct s_texture {
    void    *img_ptr;
    char    *img_data;
    int     width;
    int     height;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
} t_texture;

// --- TEXTURES CONTAINER STRUCT (t_textures) ---
// CONFLICT RESOLVED: Unified to hold the four wall textures.
typedef struct s_textures {
    t_texture   *north; // Wall texture facing North
    t_texture   *south;
    t_texture   *east;
    t_texture   *west;
    t_texture   *floor;   // ADDED: Floor Texture
    t_texture   *ceiling; // ADDED: Ceiling Texture
} t_textures;

// Update t_wall structure - add this field:
typedef struct s_wall
{
    float   wall_end;
    float   wall_start;
    float   wall_distance;
    float   wall_height;
    float   wall_x;      // Hit coordinate for texture mapping (0.0 to 1.0)
    int     wall_face;     // Wall orientation (NORTH, SOUTH, EAST, or WEST)

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

typedef struct s_game
{
    char            **map;
    int             map_H;
    int             map_W;
    float           max_distance;
    float           fov;
    int             screen_x;
    int             screen_y;
    float           screenWidth;
    float           screenHeight;
    void            *mlx_ptr;
    void            *wid_ptr;
    float           camerax; // to check later if needed
    float           cameray; // to check later if needed
    t_player *player;
    t_wall *wall;
    t_img *img;
    t_ray *ray;
    t_keys *keys;
    t_time *tms;
    t_textures  *textures;
	t_dda *dda;
}           t_game;

// --- TEXTURE FUNCTION PROTOTYPES ---
t_texture   *load_one_texture(void *mlx_ptr, char *path, t_texture *tex); // Helper
void        draw_textured_column(t_game *game, int x, int wall_start, int wall_end);
void		render_floor_ceiling(t_game *game, int x, int wall_start, int wall_end);
// In cube.h, add this prototype:
void    free_mlx_textures(t_game *game);
void ft_draw_ceiling_projection(t_game *game, int x, int wall_start, float ray_dir_x, float ray_dir_y);
void ft_draw_floor_projection(t_game *game, int x, int wall_end, float ray_dir_x, float ray_dir_y);

// New DDA Helpers
t_texture	*ft_handle_texture_selection(t_game *game, int x, int wall_start, int wall_end);
float		ft_calculate_texture_x_y_step(t_game *game, t_texture *tex, int *tex_x);

// textures helpers - utils
int calculate_shaded_color(int base_color, float distance, int side);
int load_textures(t_game *game);
int get_texel_color(t_texture *tex, int tex_x, int tex_y);


float cast_ray(t_game *game, float ray_angle, t_wall *wall_data);
char    **ft_split(char const *s, char c);
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
void process_movement(t_game *game, float delta_time); // Updated signature
char    **ft_split_n(char *s, char c);
char    *get_map(int fd);
void set_player_direction(char c, t_game *game);
void put_pixel_safe(t_game *game, int x, int y, int color);
int close_window(t_game *game);

// --- GARBAGE COLLECTOR PROTOTYPES (Updated for O(1) add-front) ---
void    *w_malloc(size_t size);
t_addr_lst  *ft_lstn(void *content);
void    ft_lstadd_f(t_addr_lst **lst, t_addr_lst *ne); // Changed from ft_lstadd_b
void    ft_lstc(t_addr_lst **lst); // Modified to clean only (no exit)
t_addr_lst  **get_garbage_collecter(void);
// --- END GC PROTOTYPES ---

// 1st question: m i the one who needs to adress the P in the map using calculations? 
// Yes, your code currently does this correctly in `getpp`: 
// `game->player->pp_x = x + 0.5; game->player->pp_y = y + 0.5;`
#endif