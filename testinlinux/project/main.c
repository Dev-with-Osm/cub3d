#include "cube.h"

void put_pixel_safe(t_game *game, int x, int y, int color)
{
    if (x >= 0 && x < game->screenWidth && y >= 0 && y < game->screenHeight )
    {
        int pixel_index = (y * game->img->line_length) + (x * (game->img->bits_per_pixel / 8));
        *(int *)(game->img->img_data + pixel_index) = color;
    }
}

int aloc(t_game *game)
{
    game->player = malloc(sizeof(t_player));
    game->ray = malloc(sizeof(t_ray));
    game->img = malloc(sizeof(t_img));
    game->wall = malloc(sizeof(t_wall));
    game->keys = malloc(sizeof(t_keys));
    game->tms = malloc(sizeof(t_time));
    if(!game->player || !game->ray || !game->img
        || !game->wall || !game->keys || !game->tms)
        return (0);
    return (1);
}

void get_data(t_game *game, int y, int x)
{
    game->map_H = y;
    game->map_W = x;
    game->screenHeight = y * tile;
    game->screenWidth = x * tile;
    game->fov = DEG_TO_RAD(60);
    game->tms->sleep_time = 16667;
    game->keys->a_pressed = 0;
    game->keys->d_pressed = 0;
    game->keys->left_pressed = 0;
    game->keys->right_pressed = 0;
    game->keys->s_pressed = 0;
    game->keys->w_pressed = 0;
}

void getpp(t_game *game)
{
    int x, y;
    int max_width = 0;

    y = 0;
    while(game->map[y])
    {
        x = 0;
        while(game->map[y][x])
        {
            if(game->map[y][x] == 'E' || game->map[y][x] == 'W' || 
               game->map[y][x] == 'S' || game->map[y][x] == 'N')
            {
                set_player_direction(game->map[y][x], game);
                game->map[y][x] = '0';
                game->player->pp_x = x + 0.5;  // Center in grid cell
                game->player->pp_y = y + 0.5;
            }
            x++;
        }
        if (x > max_width)
            max_width = x;
        y++;
    }
    get_data(game ,y, max_width);
}

void clear_image(t_game *game, int color)
{
    int total_pixels = game->screenWidth * game->screenHeight;
    int *pixel_data = (int *)game->img->img_data;
    
    for (int i = 0; i < total_pixels; i++)
        pixel_data[i] = color;
}

float cast_ray(t_game *game, float ray_angle, t_wall *wall_data)
{
    float ray_dir_x = cos(ray_angle);
    float ray_dir_y = sin(ray_angle);
    int map_x = (int)game->player->pp_x;
    int map_y = (int)game->player->pp_y;
    float side_dist_x;
    float side_dist_y;
    // Calculates the distance needed to travel along the ray to move 1 unit in X or Y
    float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
    float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
    int step_x;
    int step_y;
    int hit = 0;
    int side = -1; // 0 for X-side, 1 for Y-side
    float perp_wall_dist = game->max_distance;

    // Calculate step and initial sideDist (distance to the first grid line)
    if (ray_dir_x < 0) {
        step_x = -1;
        side_dist_x = (game->player->pp_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - game->player->pp_x) * delta_dist_x;
    }
    if (ray_dir_y < 0) {
        step_y = -1;
        side_dist_y = (game->player->pp_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - game->player->pp_y) * delta_dist_y;
    }

    // Perform DDA: Jump from square to square until a wall is found
    while (hit == 0) {
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // Hit X-side (Vertical grid line)
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // Hit Y-side (Horizontal grid line)
        }

        if (map_x < 0 || map_x >= game->map_W || map_y < 0 || map_y >= game->map_H)
            return (game->max_distance); // Hit boundary
        if (game->map[map_y][map_x] == '1')
            hit = 1;
    }

    // Calculate perpendicular distance to avoid fisheye effect
    if (side == 0)
        perp_wall_dist = (side_dist_x - delta_dist_x);
    else
        perp_wall_dist = (side_dist_y - delta_dist_y);
    
    // --- Wall Hit Data Calculation (for texturing) ---
    float wall_x; 

    if (side == 0) // X-side hit: collision point is determined by Y-ray and player Y
        wall_x = game->player->pp_y + perp_wall_dist * ray_dir_y;
    else // Y-side hit: collision point is determined by X-ray and player X
        wall_x = game->player->pp_x + perp_wall_dist * ray_dir_x;

    // Get the fractional part (0.0 to 1.0) of the coordinate
    wall_x -= floor(wall_x); 
    wall_data->wall_x = wall_x;
    wall_data->wall_distance = perp_wall_dist;
    // Set wall face for completeness (even though we use one texture)
    wall_data->wall_face = (side == 0) ? ((ray_dir_x > 0) ? WEST : EAST) : ((ray_dir_y > 0) ? NORTH : SOUTH);
    // --- End Wall Hit Data Calculation ---

    return (perp_wall_dist);
}



float cast_ray_for_distance(t_game *game, float ray_angle)
{
    game->ray->ray_x = game->player->pp_x;
    game->ray->ray_y = game->player->pp_y;
    game->ray->ray_dir_x = cos(ray_angle);
    game->ray->ray_dir_y = sin(ray_angle);
    
    float step_size = 0.005;
    float distance = 0;
    
    while (distance < game->max_distance) {
        game->ray->ray_x += game->ray->ray_dir_x * step_size;
        game->ray->ray_y += game->ray->ray_dir_y * step_size;
        distance += step_size;
        
        if (is_wall(game, game->ray->ray_x, game->ray->ray_y)) {
            float perpendicular_distance = distance * cos(ray_angle - game->player->player_angle);
            return perpendicular_distance;
            // return distance;  // Return distance when wall is hit
        }
    }
    return game->max_distance;  // No wall found
}

int calculate_shaded_wall_color(t_game *game)
{
    int base_color = GRAY;
    
    // Calculate brightness (clamped in one step)
    float brightness = 1.0 - (game->wall->wall_distance / 12.0);
    // Clamp between 0.3 and 1.0 in one operation
    if (brightness < 0.3) 
        brightness = 0.3;
    else if (brightness > 1.0) 
        brightness = 1.0;
    
    // Extract RGB components once
    int r = (base_color >> 16) & 0xFF;
    int g = (base_color >> 8) & 0xFF;
    int b = base_color & 0xFF;
    
    // Apply brightness with proper casting
    r = (int)(r * brightness);
    g = (int)(g * brightness);
    b = (int)(b * brightness);
    
    // Recombine into single color value
    return (r << 16) | (g << 8) | b;
}

void draw_wall_column(t_game *game, int x, int wall_start, int wall_end)
{
    // Clamp values to screen bounds
    if (wall_start < 0) wall_start = 0;
    if (wall_end > game->screenHeight) wall_end = game->screenHeight;
    
    // Draw ceiling
    for (int y = 0; y < wall_start; y++)
        put_pixel_safe(game, x, y, BLUE);
    
    // Draw wall
    for (int y = wall_start; y < wall_end; y++)
        put_pixel_safe(game, x, y, calculate_shaded_wall_color(game));  // White wall
    
    // Draw floor  
    for (int y = wall_end; y < game->screenHeight; y++)
        put_pixel_safe(game, x, y, WHITE);  // Brown floor
}

void rendering(t_game *game)
{
    int column = 0;
    
    while (column < game->screenWidth)
    {
        game->ray->ray_angle = game->player->player_angle - (game->fov/2) + 
                              (column * game->fov / game->screenWidth);
        
        game->wall->wall_distance = cast_ray_for_distance(game, game->ray->ray_angle);
        
        game->wall->wall_height = (int)((game->screenHeight * 0.8) / game->wall->wall_distance);
        
        if (game->wall->wall_height > game->screenHeight)
            game->wall->wall_height = game->screenHeight;
            
        
        game->wall->wall_start = (game->screenHeight - game->wall->wall_height) / 2;
        game->wall->wall_end = game->wall->wall_start + game->wall->wall_height;
        
        draw_wall_column(game, column, game->wall->wall_start, game->wall->wall_end);
        
        column++;
    }
}

int game_loop(t_game *game) 
{
    struct timeval start, end;
    
    // Start timing at the BEGINNING of frame
    gettimeofday(&start, NULL);
    
    process_movement(game);
    clear_image(game, BLACK);
    rendering(game);
    
    // Display the frame BEFORE sleeping
    mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
    
    // End timing after all work is done
    gettimeofday(&end, NULL);
    
    // Calculate elapsed time in microseconds (CORRECT order: end - start)
    game->tms->elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L + 
                                (end.tv_usec - start.tv_usec);
    
    // Calculate sleep time - FIXED: now uses sleep_time correctly
    game->tms->frame_sleep_time = game->tms->sleep_time - game->tms->elapsed_microseconds;
    
    // Only sleep if we have time left (don't sleep negative amounts!)
    if (game->tms->frame_sleep_time > 0) {
        usleep(game->tms->frame_sleep_time);
    }
    
    return 0;
}

int main(int ac, char **av)
{
    t_game *game;
    int fd;

    if(ac <= 1)
        return 0;
    game = malloc(sizeof(t_game));
    if(!game || !aloc(game))
        return 0;
    fd = open(av[1], O_RDONLY);
    game->map = ft_split_n(get_map(fd), '\n');
    getpp(game);

	game->mlx_ptr = mlx_init();
    game->max_distance = sqrt((game->screenHeight * game->screenHeight) + (game->screenWidth * game->screenWidth));
    // game->max_distance = 50.0;
    game->wid_ptr = mlx_new_window(game->mlx_ptr, game->screenWidth, game->screenHeight, "cub3D Ray Casting");
    game->img = create_image(game);

    mlx_hook(game->wid_ptr, 2, 1L<<0, key_press, game); 
    mlx_hook(game->wid_ptr, 3, 1L<<1, key_release, game);
    mlx_hook(game->wid_ptr, 17, 0, close_window, game);         // Window close
    mlx_loop_hook(game->mlx_ptr, game_loop, game);           // Main loop

    mlx_loop(game->mlx_ptr);
}