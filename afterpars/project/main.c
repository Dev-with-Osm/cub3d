#include "cube.h"

int aloc(t_game *game)
{
    game->player = malloc(sizeof(t_player));
    game->ray = malloc(sizeof(t_ray));
    game->img = malloc(sizeof(t_img));
    game->wall = malloc(sizeof(t_wall));
    if(!game->player || !game->ray || !game->img || !game->wall)
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

double cast_ray_for_distance(t_game *game, double ray_angle)
{
    game->ray->ray_x = game->player->pp_x;
    game->ray->ray_y = game->player->pp_y;
    game->ray->ray_dir_x = cos(ray_angle);
    game->ray->ray_dir_y = sin(ray_angle);
    
    double step_size = 0.005;
    double distance = 0;
    
    while (distance < game->max_distance) {
        game->ray->ray_x += game->ray->ray_dir_x * step_size;
        game->ray->ray_y += game->ray->ray_dir_y * step_size;
        distance += step_size;
        
        if (is_wall(game, game->ray->ray_x, game->ray->ray_y)) {
            double perpendicular_distance = distance * cos(ray_angle - game->player->player_angle);
            return perpendicular_distance;
            // return distance;  // Return distance when wall is hit
        }
    }
    return game->max_distance;  // No wall found
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
        put_pixel_safe(game, x, y, GRAY);  // White wall
    
    // Draw floor  
    for (int y = wall_end; y < game->screenHeight; y++)
        put_pixel_safe(game, x, y, WHITE);  // Brown floor
}

void rendering(t_game *game)
{
    double FOV = DEG_TO_RAD(FOV_DEGREES);
    int column = 0;
    
    while (column < game->screenWidth)
    {
        game->ray->ray_angle = game->player->player_angle - (FOV/2) + 
                              (column * FOV / game->screenWidth);
        
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
    // Clear screen
    clear_image(game, BLACK);
    
    rendering(game);
   
    mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
    
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
    mlx_hook(game->wid_ptr, 2, 1L<<0, key_handler, game);       // Key press
    mlx_hook(game->wid_ptr, 17, 0, close_window, game);         // Window close
    mlx_loop_hook(game->mlx_ptr, game_loop, game);           // Main loop

    mlx_loop(game->mlx_ptr);
}