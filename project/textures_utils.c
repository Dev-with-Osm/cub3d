#include "cube.h"

// Implementation of MLX image cleanup
void    free_mlx_textures(t_game *game)
{
    if (!game->mlx_ptr || !game->textures)
        return;
    
    // Check and destroy each of the six image pointers created by mlx_xpm_file_to_image
    if (game->textures->north && game->textures->north->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->north->img_ptr);
    if (game->textures->south && game->textures->south->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->south->img_ptr);
    if (game->textures->east && game->textures->east->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->east->img_ptr);
    if (game->textures->west && game->textures->west->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->west->img_ptr);
    if (game->textures->floor && game->textures->floor->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->floor->img_ptr);
    if (game->textures->ceiling && game->textures->ceiling->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->textures->ceiling->img_ptr);
}

int calculate_shaded_color(int base_color, float distance, int side)
{
    float brightness = 1.0 - (distance / 12.0); // 12.0 is the depth limit for full darkness
    
    // Clamp brightness to prevent overexposure or pitch black areas
    if (brightness < 0.3) 
        brightness = 0.3;
    else if (brightness > 1.0) 
        brightness = 1.0;
    
    // Apply directional shade: make N/S walls (side 1) slightly darker
    if (side == 1)
        brightness *= 0.8; 
    
    int r = (base_color >> 16) & 0xFF;
    int g = (base_color >> 8) & 0xFF;
    int b = base_color & 0xFF;
    
    // Apply brightness factor to each component
    r = (int)(r * brightness);
    g = (int)(g * brightness);
    b = (int)(b * brightness);
    
    // Recombine the final shaded color
    return (r << 16) | (g << 8) | b;
}

t_texture *load_one_texture(void *mlx_ptr, char *path, t_texture *tex)
{
    if (!tex)
        return (NULL);
    
    // 1. Load the XPM file
    tex->img_ptr = mlx_xpm_file_to_image(mlx_ptr, path, &tex->width, &tex->height);
    if (!tex->img_ptr) {
        fprintf(stderr, "Error: Failed to load texture from %s\n", path);
        return (NULL); 
    }
    
    // 2. Get the raw data address for direct pixel access
    tex->img_data = mlx_get_data_addr(tex->img_ptr, &tex->bits_per_pixel,
                                      &tex->line_length, &tex->endian);
    if (!tex->img_data) {
        mlx_destroy_image(mlx_ptr, tex->img_ptr);
        return (NULL);
    }
    return (tex);
}


int get_texel_color(t_texture *tex, int tex_x, int tex_y)
{
    char    *dst;
    int     bpp_bytes;
    
    // Safety check for bounds
    if (tex_x < 0 || tex_x >= tex->width || tex_y < 0 || tex_y >= tex->height)
        return (0x0);
    
    bpp_bytes = tex->bits_per_pixel / 8;
    
    // Calculate destination address: Y * line_length + X * bytes_per_pixel 
    dst = tex->img_data + (tex_y * tex->line_length) + (tex_x * bpp_bytes);
          
    return (*(unsigned int *)dst);
}

float	ft_calculate_texture_x_y_step(t_game *game, t_texture *tex, int *tex_x)
{
	float	step;
	float	tex_pos_y;

	// 2. Calculate tex_x and handle flipping
	*tex_x = (int)(game->wall->wall_x * (float)tex->width);

	// Check if the ray hit the wall "backwards" relative to the map orientation.
	if ((game->wall->wall_face == EAST && cos(game->ray->ray_angle) > 0) || 
		(game->wall->wall_face == WEST && cos(game->ray->ray_angle) < 0) ||
		(game->wall->wall_face == NORTH && sin(game->ray->ray_angle) > 0) ||
		(game->wall->wall_face == SOUTH && sin(game->ray->ray_angle) < 0))
	{
		*tex_x = tex->width - *tex_x - 1;
	}

	// 3. Calculate vertical step and initial position
	step = 1.0f * tex->height / game->wall->wall_height;

	if (game->wall->wall_start < 0)
		tex_pos_y = (float)(-game->wall->wall_start) * step;
	else 
		tex_pos_y = 0.0f;
	
	return (tex_pos_y);
}

t_texture	*ft_handle_texture_selection(t_game *game, int x, int wall_start, int wall_end)
{
	t_texture	*tex;
	int			color;
	int			side;

	// 1. Select the correct texture
	if (game->wall->wall_face == NORTH)
		tex = game->textures->north;
	else if (game->wall->wall_face == SOUTH)
		tex = game->textures->south;
	else if (game->wall->wall_face == EAST)
		tex = game->textures->east;
	else
		tex = game->textures->west;

	// Handle texture loading failure (draw solid shaded color as fallback)
	if (!tex || !tex->img_data)
	{
		side = (game->wall->wall_face == EAST || game->wall->wall_face == WEST) ? 0 : 1;
		// Use GRAY (0x808080) for fallback
		color = calculate_shaded_color(0x808080, game->wall->wall_distance, side);
		for (int y = wall_start; y < wall_end; y++)
			put_pixel_safe(game, x, y, color);
		return (NULL);
	}
	return (tex);
}