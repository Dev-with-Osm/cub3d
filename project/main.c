#include "cube.h"

static int init_structs(t_game *game)
{
    game->map = w_malloc(sizeof(t_map));
    game->ray = w_malloc(sizeof(t_ray));
    game->img = w_malloc(sizeof(t_img));
    game->wall = w_malloc(sizeof(t_wall));
    game->keys = w_malloc(sizeof(t_keys));
    game->tms = w_malloc(sizeof(t_time));
    game->textures = w_malloc(sizeof(t_textures));
    
    if (!game->map || !game->ray || !game->img || !game->wall ||
        !game->keys || !game->tms || !game->textures)
        return (0);
    
    game->textures->north = w_malloc(sizeof(t_texture));
    game->textures->south = w_malloc(sizeof(t_texture));
    game->textures->east = w_malloc(sizeof(t_texture));
    game->textures->west = w_malloc(sizeof(t_texture));
    game->textures->floor = w_malloc(sizeof(t_texture));
    game->textures->ceiling = w_malloc(sizeof(t_texture));
    
    if (!game->textures->north || !game->textures->south ||
        !game->textures->east || !game->textures->west ||
        !game->textures->floor || !game->textures->ceiling)
        return (0);
    
    game->keys->w_pressed = 0;
    game->keys->s_pressed = 0;
    game->keys->a_pressed = 0;
    game->keys->d_pressed = 0;
    game->keys->left_pressed = 0;
    game->keys->right_pressed = 0;
    game->tms->last_frame_time = 0;
    
    return (1);
}

static void find_player(t_game *game)
{
    int x, y, max_w = 0;
    
    y = 0;
    while (game->map->map[y])
    {
        x = 0;
        while (game->map->map[y][x])
        {
            if (game->map->map[y][x] == 'N' || game->map->map[y][x] == 'S' ||
                game->map->map[y][x] == 'E' || game->map->map[y][x] == 'W')
            {
                set_player_direction(game->map->map[y][x], game);
                game->map->map[y][x] = '0';
                game->map->pp_x = x + 0.5f;
                game->map->pp_y = y + 0.5f;
            }
            x++;
        }
        if (x > max_w)
            max_w = x;
        y++;
    }
    
    game->map->map_H = y;
    game->map->map_W = max_w;
    game->screenHeight = y * tile;
    game->screenWidth = max_w * tile;
    game->fov = DEG_TO_RAD(60);
}

t_img *create_image(t_game *game)
{
    t_img *img;
    
    img = w_malloc(sizeof(t_img));
    if (!img)
        return (NULL);
    
    img->img_ptr = mlx_new_image(game->mlx_ptr, game->screenWidth, game->screenHeight);
    if (!img->img_ptr)
        return (NULL);
    
    img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
                                      &img->line_length, &img->endian);
    if (!img->img_data)
    {
        mlx_destroy_image(game->mlx_ptr, img->img_ptr);
        return (NULL);
    }
    
    return (img);
}

void set_player_direction(char c, t_game *game)
{
    if (c == 'N')
        game->map->player_angle = DEG_TO_RAD(270);
    else if (c == 'S')
        game->map->player_angle = DEG_TO_RAD(90);
    else if (c == 'E')
        game->map->player_angle = DEG_TO_RAD(0);
    else if (c == 'W')
        game->map->player_angle = DEG_TO_RAD(180);
}

int key_press(int keycode, t_game *game)
{
    if (keycode == KEY_ESC || keycode == LINUX_KEY_ESC || keycode == MAC_KEY_ESC)
        close_window(game);
    else if (keycode == KEY_W || keycode == MAC_KEY_W)
        game->keys->w_pressed = 1;
    else if (keycode == KEY_S || keycode == MAC_KEY_S)
        game->keys->s_pressed = 1;
    else if (keycode == KEY_A || keycode == MAC_KEY_A)
        game->keys->a_pressed = 1;
    else if (keycode == KEY_D || keycode == MAC_KEY_D)
        game->keys->d_pressed = 1;
    else if (keycode == KEY_LEFT || keycode == LINUX_KEY_LEFT || keycode == MAC_KEY_LEFT)
        game->keys->left_pressed = 1;
    else if (keycode == KEY_RIGHT || keycode == LINUX_KEY_RIGHT || keycode == MAC_KEY_RIGHT)
        game->keys->right_pressed = 1;
    
    return (0);
}

int key_release(int keycode, t_game *game)
{
    if (keycode == KEY_W || keycode == MAC_KEY_W)
        game->keys->w_pressed = 0;
    else if (keycode == KEY_S || keycode == MAC_KEY_S)
        game->keys->s_pressed = 0;
    else if (keycode == KEY_A || keycode == MAC_KEY_A)
        game->keys->a_pressed = 0;
    else if (keycode == KEY_D || keycode == MAC_KEY_D)
        game->keys->d_pressed = 0;
    else if (keycode == KEY_LEFT || keycode == LINUX_KEY_LEFT || keycode == MAC_KEY_LEFT)
        game->keys->left_pressed = 0;
    else if (keycode == KEY_RIGHT || keycode == LINUX_KEY_RIGHT || keycode == MAC_KEY_RIGHT)
        game->keys->right_pressed = 0;
    
    return (0);
}

int close_window(t_game *game)
{
    if (game->img && game->img->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
    if (game->wid_ptr)
        mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
    
    free_textures(game);
    ft_lstc(get_garbage_collecter());
    
    exit(0);
    return (0);
}

int game_loop(t_game *game)
{
    struct timeval start, end;
    long current_time;
    float delta_time;
    
    gettimeofday(&start, NULL);
    current_time = start.tv_sec * 1000000L + start.tv_usec;
    
    if (game->tms->last_frame_time == 0)
        game->tms->last_frame_time = current_time;
    
    delta_time = (float)(current_time - game->tms->last_frame_time) / 1000000.0f;
    game->tms->last_frame_time = current_time;
    
    handle_movement(game, delta_time);
    clear_image(game, BLACK);
    rendering(game);
    mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
    
    gettimeofday(&end, NULL);
    game->tms->elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L +
                                      (end.tv_usec - start.tv_usec);
    
    return (0);
}

int main(int ac, char **av)
{
    t_game *game;
    int fd;
    t_addr_lst **gc_list;
    
    gc_list = get_garbage_collecter();
    
    if (ac <= 1)
    {
        fprintf(stderr, "Error: No map file provided\n");
        return (ft_lstc(gc_list), 1);
    }
    
    game = w_malloc(sizeof(t_game));
    if (!init_structs(game))
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return (ft_lstc(gc_list), 1);
    }
    
    fd = open(av[1], O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Error: Cannot open map file\n");
        return (ft_lstc(gc_list), 1);
    }
    
    game->map->map = ft_split_n(get_map(fd), '\n');
    close(fd);
    
    find_player(game);
    
    game->mlx_ptr = mlx_init();
    game->max_distance = sqrt(game->screenHeight * game->screenHeight +
                             game->screenWidth * game->screenWidth);
    game->wid_ptr = mlx_new_window(game->mlx_ptr, game->screenWidth,
                                   game->screenHeight, "CUB3D");
    game->img = create_image(game);
    
    if (!load_textures(game))
    {
        fprintf(stderr, "Error: Failed to load textures\n");
        free_textures(game);
        return (ft_lstc(gc_list), 1);
    }
    
    mlx_hook(game->wid_ptr, 2, 1L << 0, key_press, game);
    mlx_hook(game->wid_ptr, 3, 1L << 1, key_release, game);
    mlx_hook(game->wid_ptr, 17, 0, close_window, game);
    mlx_loop_hook(game->mlx_ptr, game_loop, game);
    mlx_loop(game->mlx_ptr);
    
    free_textures(game);
    ft_lstc(gc_list);
    
    return (0);
}