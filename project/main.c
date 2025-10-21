#include "cube.h"

// Note: You must ensure 'calculate_shaded_color', 'draw_textured_column', 
// 'render_floor_ceiling', and 'process_movement' are implemented in your helper files.

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
    // --- USING w_malloc FOR ALL STRUCTS ---
    game->player = w_malloc(sizeof(t_player));
    game->ray = w_malloc(sizeof(t_ray));
    game->img = w_malloc(sizeof(t_img));
    game->wall = w_malloc(sizeof(t_wall));
    game->keys = w_malloc(sizeof(t_keys));
    game->tms = w_malloc(sizeof(t_time));
    game->textures = w_malloc(sizeof(t_textures));
    
    // Check if w_malloc failed (w_malloc handles exit, but we check final pointer state)
    if(!game->player || !game->ray || !game->img
        || !game->wall || !game->keys || !game->tms || !game->textures)
        return (0); // w_malloc already exited, this is just for safety/style

    // Allocate all six individual texture structs
    game->textures->north = w_malloc(sizeof(t_texture));
    game->textures->south = w_malloc(sizeof(t_texture));
    game->textures->east = w_malloc(sizeof(t_texture));
    game->textures->west = w_malloc(sizeof(t_texture));
    game->textures->floor = w_malloc(sizeof(t_texture));
    game->textures->ceiling = w_malloc(sizeof(t_texture));

    // Check texture allocations (no need for complex 'if' as w_malloc exits on fail)
    // This check mainly validates that w_malloc didn't fail silently.
    if (!game->textures->north || !game->textures->south || 
        !game->textures->east || !game->textures->west ||
        !game->textures->floor || !game->textures->ceiling)
        return (0);
    
    game->tms->last_frame_time = 0; 
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
    char **map_ptr;

    map_ptr = game->map; // Use a local pointer for cleaner map iteration
    y = 0;
    while(map_ptr[y])
    {
        x = 0;
        while(map_ptr[y][x])
        {
            if(map_ptr[y][x] == 'E' || map_ptr[y][x] == 'W' || 
               map_ptr[y][x] == 'S' || map_ptr[y][x] == 'N')
            {
                set_player_direction(map_ptr[y][x], game);
                map_ptr[y][x] = '0';
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

void rendering(t_game *game)
{
    int column = 0;
    float cos_correction;
    
    while (column < game->screenWidth)
    {
        // 1. Ray Angle and DDA Raycast
        game->ray->ray_angle = game->player->player_angle - (game->fov / 2) + 
                              (column * game->fov / game->screenWidth);
        game->wall->wall_distance = cast_ray(game, game->ray->ray_angle, game->wall);

        // 2. Fish-eye Correction
        cos_correction = cos(game->ray->ray_angle - game->player->player_angle);
        game->wall->wall_distance *= cos_correction; 
        if (game->wall->wall_distance < 0.001)
             game->wall->wall_distance = 0.001;
             
        // 3. Wall Height Calculation
        game->wall->wall_height = (int)((game->screenHeight * 0.8) / game->wall->wall_distance);
        if (game->wall->wall_height > game->screenHeight)
            game->wall->wall_height = game->screenHeight;
            
        // 4. Wall Start/End
        game->wall->wall_start = (game->screenHeight - game->wall->wall_height) / 2;
        game->wall->wall_end = game->wall->wall_start + game->wall->wall_height;
        
        // 5. Draw Scene: Wall (Texture Mapping) and Floor/Ceiling (Projection)
        draw_textured_column(game, column, game->wall->wall_start, game->wall->wall_end);
        render_floor_ceiling(game, column, game->wall->wall_start, game->wall->wall_end);
        
        column++;
    }
}

int game_loop(t_game *game) 
{
    struct timeval start, end;
    long current_time;
    float delta_time; 
    
    // 1. Start timing at the BEGINNING of frame
    gettimeofday(&start, NULL);
    current_time = start.tv_sec * 1000000L + start.tv_usec;

    // Calculate delta_time (time elapsed since the last frame)
    if (game->tms->last_frame_time == 0)
        game->tms->last_frame_time = current_time;
    
    delta_time = (float)(current_time - game->tms->last_frame_time) / 1000000.0f; // in seconds
    game->tms->last_frame_time = current_time; // Update for the next frame
    
    // --- PASS delta_time TO MOVEMENT ---
    process_movement(game, delta_time);
    
    clear_image(game, BLACK);
    rendering(game);
    
    // 2. Display the frame
    mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
    
    // 3. End timing and sleeping (optional, for frame rate capping)
    gettimeofday(&end, NULL);
    game->tms->elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L + 
                                (end.tv_usec - start.tv_usec);
    // if (game->tms->sleep_time > game->tms->elapsed_microseconds)
    //     usleep(game->tms->sleep_time - game->tms->elapsed_microseconds);
    
    return 0;
}

int main(int ac, char **av)
{
    t_game *game;
    int fd;
    t_addr_lst  **gc_list;

    gc_list = get_garbage_collecter(); // Initialize GC list pointer

    if(ac <= 1)
    {
        ft_lstc(gc_list);
        return 0;
    }
    
    // Use w_malloc for game struct
    game = w_malloc(sizeof(t_game));
    
    // Use original aloc function to allocate sub-structs via w_malloc
    if(!aloc(game))
    {
        ft_lstc(gc_list);
        return 0;
    }
    
    // NOTE: Map split/reading must handle its own memory, or use w_malloc.
    fd = open(av[1], O_RDONLY);
    game->map = ft_split_n(get_map(fd), '\n');
    getpp(game);

	game->mlx_ptr = mlx_init();
    game->max_distance = sqrt((game->screenHeight * game->screenHeight) + (game->screenWidth * game->screenWidth));
    game->wid_ptr = mlx_new_window(game->mlx_ptr, game->screenWidth, game->screenHeight, "cub3D Ray Casting");
    
    game->img = create_image(game); 

    // --- TEXTURE LOADING ---
    if (!load_textures(game)) 
    {
        fprintf(stderr, "Fatal Error: Failed to load one or more textures. Check paths.\n");
        // Ensure MLX textures and GC memory are freed on failure
        free_mlx_textures(game); 
        ft_lstc(gc_list);
        return 1; 
    }

    mlx_hook(game->wid_ptr, 2, 1L<<0, key_press, game); 
    mlx_hook(game->wid_ptr, 3, 1L<<1, key_release, game);
    mlx_hook(game->wid_ptr, 17, 0, close_window, game); // Window close
    mlx_loop_hook(game->mlx_ptr, game_loop, game);       // Main loop

    mlx_loop(game->mlx_ptr);
    
    // --- FINAL CLEANUP BEFORE EXIT ---
    // 1. Destroy MLX graphical resources (fixes Valgrind leak)
    free_mlx_textures(game); 

    // 2. Free all tracked memory (using your garbage collector)
    ft_lstc(gc_list);
    // ---------------------------------
    return 0;
}