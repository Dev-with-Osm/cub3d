#include "cube.h"

// Load single texture from XPM file
static t_texture *load_texture(t_game *game, char *path)
{
    t_texture *tex;
    
    if (!path)
        return (NULL);
    
    tex = malloc(sizeof(t_texture));
    if (!tex)
        return (NULL);
    
    tex->img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, path, 
                                         &tex->width, &tex->height);
    if (!tex->img_ptr)
    {
        free(tex);
        return (NULL);
    }
    
    tex->img_data = mlx_get_data_addr(tex->img_ptr, &tex->bits_per_pixel,
                                     &tex->line_length, &tex->endian);
    return (tex);
}

// Load all three textures
int load_textures(t_game *game)
{
    game->textures = malloc(sizeof(t_textures));
    if (!game->textures)
        return (0);
    
    game->textures->wall = load_texture(game, "./textures/wall.xpm");
    game->textures->floor = load_texture(game, "./textures/floor.xpm");
    game->textures->ceiling = load_texture(game, "./textures/ceiling.xpm");
    
    if (!game->textures->wall || !game->textures->floor || 
        !game->textures->ceiling)
    {
        printf("Error: Failed to load textures\n");
        free_textures(game);
        return (0);
    }
    
    return (1);
}

// Free all textures
void free_textures(t_game *game)
{
    if (!game->textures)
        return;
    
    if (game->textures->wall && game->textures->wall->img_ptr)
    {
        mlx_destroy_image(game->mlx_ptr, game->textures->wall->img_ptr);
        free(game->textures->wall);
    }
    if (game->textures->floor && game->textures->floor->img_ptr)
    {
        mlx_destroy_image(game->mlx_ptr, game->textures->floor->img_ptr);
        free(game->textures->floor);
    }
    if (game->textures->ceiling && game->textures->ceiling->img_ptr)
    {
        mlx_destroy_image(game->mlx_ptr, game->textures->ceiling->img_ptr);
        free(game->textures->ceiling);
    }
    
    free(game->textures);
}

// Get pixel from texture - inline for speed
static inline int get_tex_pixel(t_texture *tex, int x, int y)
{
    int offset;
    
    if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
        return (0);
    
    offset = (y * tex->line_length) + (x * (tex->bits_per_pixel / 8));
    return (*(int *)(tex->img_data + offset));
}

static inline int get_simple_floor_ceiling(t_game *game, t_texture *tex, int screen_x, int screen_y)
{
    int tex_x, tex_y;
    
    // Simple projection - fast but effective
    tex_x = (int)((game->player->pp_x + screen_x * 0.01f) * tex->width) & (tex->width - 1);
    tex_y = (int)((game->player->pp_y + screen_y * 0.01f) * tex->height) & (tex->height - 1);
    
    return get_tex_pixel(tex, tex_x, tex_y);
}

// OPTIMIZED: Draw column with minimal calculations
void draw_textured_column(t_game *game, int x, int wall_start, int wall_end)
{
    t_texture *tex;
    int tex_x, tex_y;
    int color;
    int y;
    float tex_step;
    float tex_pos;
    
    // Clamp to screen bounds
    if (wall_start < 0) wall_start = 0;
    if (wall_end > game->screenHeight) wall_end = game->screenHeight;
    
    // Draw ceiling - simplified for speed
    y = 0;
    while (y < wall_start)
    {
        color = get_simple_floor_ceiling(game, game->textures->ceiling, x, y);
        put_pixel_safe(game, x, y, color);
        y++;
    }
    
    // Draw textured wall - optimized with incremental texture calculation
    tex = game->textures->wall;
    tex_x = (int)(game->wall->wall_x * tex->width);
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    
    // Pre-calculate texture stepping
    tex_step = (float)tex->height / game->wall->wall_height;
    tex_pos = 0;
    
    // Pre-calculate shaded color (walls are usually similar distance in a column)
    int base_color;
    float brightness = 1.0f - (game->wall->wall_distance * 0.0833f);
    if (brightness < 0.3f) brightness = 0.3f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    y = wall_start;
    while (y < wall_end)
    {
        tex_y = (int)tex_pos;
        if (tex_y >= tex->height) tex_y = tex->height - 1;
        
        base_color = get_tex_pixel(tex, tex_x, tex_y);
        
        // Apply shading
        int r = (int)(((base_color >> 16) & 0xFF) * brightness);
        int g = (int)(((base_color >> 8) & 0xFF) * brightness);
        int b = (int)((base_color & 0xFF) * brightness);
        color = (r << 16) | (g << 8) | b;
        
        put_pixel_safe(game, x, y, color);
        
        tex_pos += tex_step;
        y++;
    }
    
    // Draw floor - simplified for speed
    y = wall_end;
    while (y < game->screenHeight)
    {
        color = get_simple_floor_ceiling(game, game->textures->floor, x, y);
        put_pixel_safe(game, x, y, color);
        y++;
    }
}