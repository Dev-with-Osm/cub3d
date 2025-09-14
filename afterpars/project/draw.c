#include "cube.h"

void put_pixel_safe(t_game *game, int x, int y, int color)
{
    if (x >= 0 && x < game->screenWidth && y >= 0 && y < game->screenHeight )
    {
        int pixel_index = (y * game->img->line_length) + (x * (game->img->bits_per_pixel / 8));
        *(int *)(game->img->img_data + pixel_index) = color;
    }
}

void draw_map(t_game *game)
{
    int x;
    int color;
    int py;
    int px;
    int y = 0;
    while ( y < game->map_H )
    {
        x = 0;
        while ( x < game->map_W )
        {
            if(game->map[y][x] == '1')
                color = WHITE;
            else
                color = BLACK;
            py = 0;
            while ( py < tile )
            {
                px = 0;
                while ( px < tile ) 
                {
                    put_pixel_safe(game, x * tile + px, y * tile + py, color);
                    px++;
                }
                py++;
            }
            x++;
        }
        y++;
    }
}

void draw_ray(t_game *game, t_player *player, int ray_color) // not needed in the project it's just for  better understanding of the raycasting concept 
{
    double steep_size;     // Small gameeps for smooth lines
    double distance;
    
    game->ray->ray_x = player->pp_x;  // gameart at player position
    game->ray->ray_y = player->pp_y;
    game->ray->ray_dir_x = cos(game->ray->ray_angle);
    game->ray->ray_dir_y = sin(game->ray->ray_angle);
    
    steep_size = 0.05;
    distance = 0;
    while (distance < game->max_distance) {
        game->ray->ray_x += game->ray->ray_dir_x * steep_size;
        game->ray->ray_y += game->ray->ray_dir_y * steep_size;
        distance += steep_size;
        // Convert world coordinates to screen pixels
        game->screen_x = (int)(game->ray->ray_x * tile);
        game->screen_y = (int)(game->ray->ray_y * tile);
        // Draw pixel at this ray position
        put_pixel_safe(game, game->screen_x, game->screen_y, ray_color);
        if (is_wall(game, game->ray->ray_x, game->ray->ray_y)) {
            put_pixel_safe(game, (game->screen_x - 0.5), (game->screen_y - 0.5), BLACK);  // Hit point
            break;
        }       
    }
}

void draw_all_rays(t_game *game)
{
    double FOV = DEG_TO_RAD(FOV_DEGREES);
    int i = 0;
    while ( i < NUM_RAYS)
    {
        game->ray->ray_angle = game->player->player_angle - (FOV/2) + (i * FOV/NUM_RAYS);
        draw_ray( game, game->player,  GRAY);
        i++;
    }
}

void draw_player(t_game *game)
{
    t_player *player = game->player;
    int player_x = (int)(player->pp_x * tile);
    int player_y = (int)(player->pp_y * tile);
    int radius = 4;
    int y;
    int x;

    y = -radius;
    while (y <= radius)
    {
        x = -radius;
        while (x <= radius) {
            if (x*x + y*y <= radius*radius) {
                put_pixel_safe(game, player_x + x, player_y + y, WHITE);
            }
            x++;
        }
        y++;
    }
}
