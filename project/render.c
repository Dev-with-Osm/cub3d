#include "cube.h"

void put_pixel_safe(t_game *game, int x, int y, int color)
{
    int offset;
    
    if (x < 0 || x >= (int)game->screenWidth || y < 0 || y >= (int)game->screenHeight)
        return;
    
    offset = (y * game->img->line_length) + (x * (game->img->bits_per_pixel / 8));
    *(int *)(game->img->img_data + offset) = color;
}

void clear_image(t_game *game, int color)
{
    int *pixels = (int *)game->img->img_data;
    int total = game->screenWidth * game->screenHeight;
    int i = 0;
    
    while (i < total)
    {
        pixels[i] = color;
        i++;
    }
}

static int apply_shading(int color, float distance, int side)
{
    float brightness;
    int r, g, b;
    
    // Calculate brightness based on distance
    brightness = 1.0f - (distance / 15.0f);
    if (brightness < 0.2f)
        brightness = 0.2f;
    if (brightness > 1.0f)
        brightness = 1.0f;
    
    // Make one side darker
    if (side == 1)
        brightness *= 0.7f;
    
    // Extract RGB
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
    
    // Apply brightness
    r = (int)(r * brightness);
    g = (int)(g * brightness);
    b = (int)(b * brightness);
    
    return ((r << 16) | (g << 8) | b);
}

static t_texture *get_wall_texture(t_game *game)
{
    if (game->wall->wall_face == NORTH)
        return (game->textures->north);
    if (game->wall->wall_face == SOUTH)
        return (game->textures->south);
    if (game->wall->wall_face == EAST)
        return (game->textures->east);
    return (game->textures->west);
}

void draw_wall_column(t_game *game, int x)
{
    t_texture *tex;
    int tex_x, tex_y;
    int y, draw_start, draw_end;
    float tex_step, tex_pos;
    int color, side;
    
    tex = get_wall_texture(game);
    if (!tex || !tex->img_data)
        return;
    
    // Calculate texture X coordinate
    tex_x = (int)(game->wall->wall_x * tex->width);
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    
    // Calculate drawing boundaries
    draw_start = (int)game->wall->wall_start;
    draw_end = (int)game->wall->wall_end;
    
    if (draw_start < 0) draw_start = 0;
    if (draw_end > (int)game->screenHeight) draw_end = (int)game->screenHeight;
    
    // Calculate texture step and starting position
    tex_step = (float)tex->height / game->wall->wall_height;
    tex_pos = 0;
    if ((int)game->wall->wall_start < 0)
        tex_pos = -(int)game->wall->wall_start * tex_step;
    
    side = (game->wall->wall_face == EAST || game->wall->wall_face == WEST) ? 0 : 1;
    
    // Draw the column
    y = draw_start;
    while (y < draw_end)
    {
        tex_y = (int)tex_pos % tex->height;
        if (tex_y < 0) tex_y += tex->height;
        
        color = get_pixel_color(tex, tex_x, tex_y);
        color = apply_shading(color, game->wall->wall_distance, side);
        put_pixel_safe(game, x, y, color);
        
        tex_pos += tex_step;
        y++;
    }
}

void draw_floor_ceiling(t_game *game, int x)
{
    int y;
    float ray_dir_x, ray_dir_y;
    float row_distance, floor_x, floor_y;
    int tex_x, tex_y, color;
    t_texture *floor_tex, *ceil_tex;
    
    floor_tex = game->textures->floor;
    ceil_tex = game->textures->ceiling;
    
    ray_dir_x = cos(game->ray->ray_angle);
    ray_dir_y = sin(game->ray->ray_angle);
    
    // Draw floor
    y = (int)game->wall->wall_end;
    while (y < (int)game->screenHeight)
    {
        int p = y - (int)game->screenHeight / 2;
        if (p <= 0)
        {
            y++;
            continue;
        }
        
        row_distance = (0.5f * game->screenHeight) / (float)p;
        floor_x = game->map->pp_x + row_distance * ray_dir_x;
        floor_y = game->map->pp_y + row_distance * ray_dir_y;
        
        tex_x = (int)(floor_x * floor_tex->width) % floor_tex->width;
        tex_y = (int)(floor_y * floor_tex->height) % floor_tex->height;
        if (tex_x < 0) tex_x += floor_tex->width;
        if (tex_y < 0) tex_y += floor_tex->height;
        
        color = get_pixel_color(floor_tex, tex_x, tex_y);
        color = apply_shading(color, row_distance, 0);
        put_pixel_safe(game, x, y, color);
        y++;
    }
    
    // Draw ceiling
    y = 0;
    while (y < (int)game->wall->wall_start)
    {
        int p = (int)game->screenHeight / 2 - y;
        if (p <= 0)
        {
            y++;
            continue;
        }
        
        row_distance = (0.5f * game->screenHeight) / (float)p;
        floor_x = game->map->pp_x + row_distance * ray_dir_x;
        floor_y = game->map->pp_y + row_distance * ray_dir_y;
        
        tex_x = (int)(floor_x * ceil_tex->width) % ceil_tex->width;
        tex_y = (int)(floor_y * ceil_tex->height) % ceil_tex->height;
        if (tex_x < 0) tex_x += ceil_tex->width;
        if (tex_y < 0) tex_y += ceil_tex->height;
        
        color = get_pixel_color(ceil_tex, tex_x, tex_y);
        color = apply_shading(color, row_distance, 0);
        put_pixel_safe(game, x, y, color);
        y++;
    }
}

void rendering(t_game *game)
{
    int x = 0;
    float camera_x, ray_angle;
    float cos_correction;

    
    while (x < (int)game->screenWidth)
    {
        // Calculate ray angle for this column
        camera_x = 2.0f * x / game->screenWidth - 1.0f;
        ray_angle = game->map->player_angle + atanf(camera_x * tanf(game->fov / 2.0f));
        
        game->ray->ray_angle = ray_angle;
        
        // Cast ray
        game->wall->wall_distance = cast_ray(game, ray_angle);
        
        // Calculate wall height
        cos_correction = cos(game->ray->ray_angle - game->map->player_angle);
        game->wall->wall_distance *= cos_correction; 
        if (game->wall->wall_distance < 0.1f)
            game->wall->wall_distance = 0.1f;
        
        game->wall->wall_height = game->screenHeight / game->wall->wall_distance;
        
        // Calculate wall boundaries
        game->wall->wall_start = (game->screenHeight - game->wall->wall_height) / 2.0f;
        game->wall->wall_end = game->wall->wall_start + game->wall->wall_height;
        
        // Draw column
        draw_floor_ceiling(game, x);
        draw_wall_column(game, x);
        
        x++;
    }
}