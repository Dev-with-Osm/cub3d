#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mlx.h>

#define PI 3.14159265359
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define FOV 60
#define NUM_RAYS 60

typedef struct s_player
{
    float x;
    float y;
    float angle;
} t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    int map[MAP_HEIGHT][MAP_WIDTH];
    t_player player;
} t_game;

void init_map(t_game *game)
{
    int map[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    int i = 0;
    while (i < MAP_HEIGHT)
    {
        int j = 0;
        while (j < MAP_WIDTH)
        {
            game->map[i][j] = map[i][j];
            j++;
        }
        i++;
    }
}

void draw_line(t_game *game, int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1)
    {
        mlx_pixel_put(game->mlx, game->win, x1, y1, color);
        
        if (x1 == x2 && y1 == y2)
            break;
        
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_map(t_game *game)
{
    int i = 0;
    while (i < MAP_HEIGHT)
    {
        int j = 0;
        while (j < MAP_WIDTH)
        {
            int color = (game->map[i][j] == 1) ? 0xFFFFFF : 0x333333;
            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;
            int k = 0;
            
            while (k < TILE_SIZE)
            {
                int l = 0;
                while (l < TILE_SIZE)
                {
                    mlx_pixel_put(game->mlx, game->win, x + l, y + k, color);
                    l++;
                }
                k++;
            }
            j++;
        }
        i++;
    }
}

float cast_ray(t_game *game, float angle)
{
    float ray_x = game->player.x;
    float ray_y = game->player.y;
    float ray_dx = cos(angle);
    float ray_dy = sin(angle);
    float step = 0.01;
    
    while (1)
    {
        ray_x += ray_dx * step;
        ray_y += ray_dy * step;
        
        int map_x = (int)(ray_x);
        int map_y = (int)(ray_y);
        
        if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT)
            break;
        
        if (game->map[map_y][map_x] == 1)
        {
            float distance = sqrt((ray_x - game->player.x) * (ray_x - game->player.x) +
                                 (ray_y - game->player.y) * (ray_y - game->player.y));
            return distance;
        }
        
        if (sqrt((ray_x - game->player.x) * (ray_x - game->player.x) +
                (ray_y - game->player.y) * (ray_y - game->player.y)) > 20)
            break;
    }
    return 20;
}

void draw_rays(t_game *game)
{
    float fov_rad = (FOV * PI) / 180.0;
    float angle_step = fov_rad / NUM_RAYS;
    float start_angle = game->player.angle - (fov_rad / 2);
    
    int i = 0;
    while (i < NUM_RAYS)
    {
        float ray_angle = start_angle + (i * angle_step);
        float distance = cast_ray(game, ray_angle);
        
        int end_x = (int)(game->player.x * TILE_SIZE + cos(ray_angle) * distance * TILE_SIZE);
        int end_y = (int)(game->player.y * TILE_SIZE + sin(ray_angle) * distance * TILE_SIZE);
        
        // Alternate colors for better visibility
        int color = (i % 2 == 0) ? 0x00FF00 : 0x00AA00;
        
        draw_line(game, 
                 (int)(game->player.x * TILE_SIZE),
                 (int)(game->player.y * TILE_SIZE),
                 end_x, end_y, color);
        i++;
    }
}

void draw_player(t_game *game)
{
    int px = (int)(game->player.x * TILE_SIZE);
    int py = (int)(game->player.y * TILE_SIZE);
    int radius = 5;
    
    // Draw player circle
    int i = -radius;
    while (i <= radius)
    {
        int j = -radius;
        while (j <= radius)
        {
            if (i * i + j * j <= radius * radius)
                mlx_pixel_put(game->mlx, game->win, px + i, py + j, 0xFF0000);
            j++;
        }
        i++;
    }
    
    // Draw direction line
    int dir_x = px + (int)(cos(game->player.angle) * 20);
    int dir_y = py + (int)(sin(game->player.angle) * 20);
    draw_line(game, px, py, dir_x, dir_y, 0xFFFF00);
}

int render(t_game *game)
{
    mlx_clear_window(game->mlx, game->win);
    draw_map(game);
    draw_rays(game);
    draw_player(game);
    return 0;
}

int key_press(int keycode, t_game *game)
{
    float move_speed = 0.1;
    float rot_speed = 0.1;
    
    // ESC
    if (keycode == 53 || keycode == 65307)
        exit(0);
    
    // W - forward
    if (keycode == 13 || keycode == 119)
    {
        float new_x = game->player.x + cos(game->player.angle) * move_speed;
        float new_y = game->player.y + sin(game->player.angle) * move_speed;
        if (game->map[(int)new_y][(int)new_x] == 0)
        {
            game->player.x = new_x;
            game->player.y = new_y;
        }
    }
    
    // S - backward
    if (keycode == 1 || keycode == 115)
    {
        float new_x = game->player.x - cos(game->player.angle) * move_speed;
        float new_y = game->player.y - sin(game->player.angle) * move_speed;
        if (game->map[(int)new_y][(int)new_x] == 0)
        {
            game->player.x = new_x;
            game->player.y = new_y;
        }
    }
    
    // A - rotate left
    if (keycode == 0 || keycode == 97)
        game->player.angle -= rot_speed;
    
    // D - rotate right
    if (keycode == 2 || keycode == 100)
        game->player.angle += rot_speed;
    
    // Left arrow
    if (keycode == 123 || keycode == 65361)
        game->player.angle -= rot_speed;
    
    // Right arrow
    if (keycode == 124 || keycode == 65363)
        game->player.angle += rot_speed;
    
    render(game);
    return 0;
}

int main(void)
{
    t_game game;
    
    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Raycast");
    
    init_map(&game);
    
    game.player.x = 5.0;
    game.player.y = 5.0;
    game.player.angle = 0;
    
    render(&game);
    
    mlx_hook(game.win, 2, 1L << 0, key_press, &game);
    mlx_loop_hook(game.mlx, render, &game);
    mlx_loop(game.mlx);
    
    return 0;
}