#include "cube.h" // Assuming all texture functions are defined here

int load_textures(t_game *game)
{
    // !!! IMPORTANT: REPLACE THESE PATHS WITH YOUR ACTUAL XPM FILE LOCATIONS !!!
    
    if (!load_one_texture(game->mlx_ptr, "./textures/ye.xpm", game->textures->north))
        return (0);
    if (!load_one_texture(game->mlx_ptr, "./textures/ye.xpm", game->textures->south))
        return (0);
    if (!load_one_texture(game->mlx_ptr, "./textures/ye.xpm", game->textures->east))
        return (0);
    if (!load_one_texture(game->mlx_ptr, "./textures/ye.xpm", game->textures->west))
        return (0);
    
    // Placeholder paths for Floor and Ceiling
    if (!load_one_texture(game->mlx_ptr, "./textures/floor.xpm", game->textures->floor))
        return (0);
    if (!load_one_texture(game->mlx_ptr, "./textures/ceiling.xpm", game->textures->ceiling))
        return (0);

    return (1);
}

void draw_textured_column(t_game *game, int x, int wall_start, int wall_end)
{
	t_texture	*tex;
	float		tex_pos_y;
	float		step;
	int			tex_x;
	int			y;
	int			color;
	int			side;
	// 1. Select texture and handle failure
	tex = ft_handle_texture_selection(game, x, wall_start, wall_end);
	if (!tex)
		return ;
	// 2. Calculate coordinates setup
	tex_pos_y = ft_calculate_texture_x_y_step(game, tex, &tex_x);
	// Recalculate side (or pass it from selection func if possible)
	side = (game->wall->wall_face == EAST || game->wall->wall_face == WEST) ? 0 : 1;
	// Get the vertical step (needed for the loop)
	step = 1.0f * tex->height / game->wall->wall_height;
	// 3. Draw wall (only the wall section)
	y = wall_start;
	if (y < 0)
		y = 0; // Clamp start
	while (y < wall_end && y < game->screenHeight)
	{
		int tex_y = (int)tex_pos_y;
		tex_pos_y += step;
		color = get_texel_color(tex, tex_x, tex_y);
		color = calculate_shaded_color(color, game->wall->wall_distance, side);
		put_pixel_safe(game, x, y, color);
		y++;
	}
}

#define PLAYER_Z_HEIGHT 0.5 

void ft_draw_ceiling_projection(t_game *game, int x, int wall_start, float ray_dir_x, float ray_dir_y)
{
    int         y;
    float       dist;
    float       floor_x;
    float       floor_y;
    int         tex_x;
    int         tex_y;
    int         color;

    y = 0;
    while (y < wall_start && y < game->screenHeight)
    {
        int p = game->screenHeight / 2 - y; 
        // World distance and coordinates
        dist = (PLAYER_Z_HEIGHT * game->screenHeight) / (2.0f * p); 
        floor_x = game->player->pp_x + dist * ray_dir_x;
        floor_y = game->player->pp_y + dist * ray_dir_y;
        // --- Texture Mapping ---
        tex_x = (int)(floor_x * game->textures->ceiling->width);
        tex_y = (int)(floor_y * game->textures->ceiling->height);
        // Tiling and wrapping
        tex_x = tex_x % game->textures->ceiling->width;
        tex_y = tex_y % game->textures->ceiling->height;
        if (tex_x < 0) tex_x += game->textures->ceiling->width;
        if (tex_y < 0) tex_y += game->textures->ceiling->height;
        color = get_texel_color(game->textures->ceiling, tex_x, tex_y);
        color = calculate_shaded_color(color, dist, 0);  
        put_pixel_safe(game, x, y, color);
        y++;
    }
}

void ft_draw_floor_projection(t_game *game, int x, int wall_end, float ray_dir_x, float ray_dir_y)
{
    int         y;
    float       dist;
    float       floor_x;
    float       floor_y;
    int         tex_x;
    int         tex_y;
    int         color;
    t_texture   *floor_tex;

    floor_tex = game->textures->floor;
    y = wall_end;
    while (y < game->screenHeight)
    {
        int p = y - game->screenHeight / 2;
        
        // World distance and coordinates
        dist = (PLAYER_Z_HEIGHT * game->screenHeight) / (2.0f * p); 
        floor_x = game->player->pp_x + dist * ray_dir_x;
        floor_y = game->player->pp_y + dist * ray_dir_y;

        // --- Texture Mapping ---
        tex_x = (int)(floor_x * floor_tex->width);
        tex_y = (int)(floor_y * floor_tex->height);

        // Tiling and wrapping
        tex_x = tex_x % floor_tex->width;
        tex_y = tex_y % floor_tex->height;
        if (tex_x < 0) tex_x += floor_tex->width;
        if (tex_y < 0) tex_y += floor_tex->height;
        
        color = get_texel_color(floor_tex, tex_x, tex_y);
        color = calculate_shaded_color(color, dist, 0); 

        put_pixel_safe(game, x, y, color);
        y++;
    }
}

void render_floor_ceiling(t_game *game, int x, int wall_start, int wall_end)
{
    float current_ray_angle;
    float ray_dir_x;
    float ray_dir_y;
    
    // 1. Calculate ray direction for the column
    current_ray_angle = game->player->player_angle - (game->fov / 2) + 
                              (x * game->fov / game->screenWidth);
    ray_dir_x = cos(current_ray_angle);
    ray_dir_y = sin(current_ray_angle);

    // 2. Draw Ceiling
    ft_draw_ceiling_projection(game, x, wall_start, ray_dir_x, ray_dir_y);

    // 3. Draw Floor
    ft_draw_floor_projection(game, x, wall_end, ray_dir_x, ray_dir_y);
}


