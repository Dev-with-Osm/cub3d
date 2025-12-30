#include "cub3d.h"


void draw_floor(t_game *game, int x, int wall_end)
{
    int y;
    int color;
    
    color = game->textures->floor_color;
    y = wall_end;
    while (y < (int)game->screenHeight)
    {
        put_pixel_safe(game, x, y, color);
        y++;
    }
}

void draw_ceiling(t_game *game, int x, int wall_start)
{
    int y;
    int color;
    
    color = game->textures->ceiling_color;
    y = 0;
    while (y < wall_start)
    {
        put_pixel_safe(game, x, y, color);
        y++;
    }
}

void draw_floor_ceiling(t_game *game, int x)
{
    draw_ceiling(game, x, (int)game->wall->wall_start);
    draw_floor(game, x, (int)game->wall->wall_end);
}
