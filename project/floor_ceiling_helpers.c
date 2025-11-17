#include "cube.h"

static void get_texture_coords(float pos_x, float pos_y, t_texture *tex,
                               int *tex_x, int *tex_y)
{
    *tex_x = (int)(pos_x * tex->width) % tex->width;
    *tex_y = (int)(pos_y * tex->height) % tex->height;
    if (*tex_x < 0)
        *tex_x += tex->width;
    if (*tex_y < 0)
        *tex_y += tex->height;
}

static void draw_ceiling_pixel(t_game *game, int x, int y,
                               float ray_dir_x, float ray_dir_y)
{
    int p;
    float row_distance, floor_x, floor_y;
    int tex_x, tex_y, color;
    
    p = (int)game->screenHeight / 2 - y;
    row_distance = (0.5f * game->screenHeight) / (float)p;
    floor_x = game->map->pp_x + row_distance * ray_dir_x;
    floor_y = game->map->pp_y + row_distance * ray_dir_y;
    get_texture_coords(floor_x, floor_y, game->textures->ceiling, &tex_x, &tex_y);
    color = get_pixel_color(game->textures->ceiling, tex_x, tex_y);
    color = apply_shading(color, row_distance, 0);
    put_pixel_safe(game, x, y, color);
}

void draw_ceiling(t_game *game, int x, float ray_dir_x, float ray_dir_y)
{
    int y;
    int p;
    
    y = 0;
    while (y < (int)game->wall->wall_start)
    {
        p = (int)game->screenHeight / 2 - y;
        if (p > 0)
            draw_ceiling_pixel(game, x, y, ray_dir_x, ray_dir_y);
        y++;
    }
}

static void draw_floor_pixel(t_game *game, int x, int y,
                             float ray_dir_x, float ray_dir_y)
{
    int p;
    float row_distance, floor_x, floor_y;
    int tex_x, tex_y, color;
    
    p = y - (int)game->screenHeight / 2;
    row_distance = (0.5f * game->screenHeight) / (float)p;
    floor_x = game->map->pp_x + row_distance * ray_dir_x;
    floor_y = game->map->pp_y + row_distance * ray_dir_y;
    get_texture_coords(floor_x, floor_y, game->textures->floor, &tex_x, &tex_y);
    color = get_pixel_color(game->textures->floor, tex_x, tex_y);
    color = apply_shading(color, row_distance, 0);
    put_pixel_safe(game, x, y, color);
}

void draw_floor(t_game *game, int x, float ray_dir_x, float ray_dir_y)
{
    int y;
    int p;
    
    y = (int)game->wall->wall_end;
    while (y < (int)game->screenHeight)
    {
        p = y - (int)game->screenHeight / 2;
        if (p > 0)
            draw_floor_pixel(game, x, y, ray_dir_x, ray_dir_y);
        y++;
    }
}

