#include "cub3d.h" 

// normenette is readey for session test

// static void find_player(t_game *game)
// {
//     int x, y, max_w = 0;
    
//     y = 0;
//     while (game->map->map[y])
//     {
//         x = 0;
//         while (game->map->map[y][x])
//         {
//             if (game->map->map[y][x] == 'N' || game->map->map[y][x] == 'S' ||
//                 game->map->map[y][x] == 'E' || game->map->map[y][x] == 'W')
//             {
//                 set_player_direction(game->map->map[y][x], game);
//                 game->map->map[y][x] = '0';
//                 game->map->player_x = x + 0.5f;
//                 game->map->player_y = y + 0.5f;
//             }
//             x++;
//         }
//         if (x > max_w)
//             max_w = x;
//         y++;
//     }
//     game->map->map_H = y;
//     game->map->map_W = max_w;
//     game->screenHeight = y * tile;
//     game->screenWidth = max_w * tile;
//     game->fov = 60 * PI / 180.0;
// }



int main(int ac, char **av)
{
    t_config cfg;
    t_game *game;
    t_addr_lst **gc_list;
    
    gc_list = get_garbage_collecter();
    game = w_malloc(sizeof(t_game));
    if (!init_structs(game))
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return (ft_lstc(gc_list), 1);
    }
    if (parsing(ac, av, game, &cfg) == 1)
        return (ft_lstc(gc_list), 1);
    game->mlx_ptr = mlx_init();
    game->max_distance = sqrt(game->screenHeight * game->screenHeight +
                             game->screenWidth * game->screenWidth);
    game->wid_ptr = mlx_new_window(game->mlx_ptr, game->screenWidth,
                                   game->screenHeight, "CUB3D");
    game->img = create_image(game);
    if (!load_textures(game, cfg.tex))
    {
        fprintf(stderr, "Error: Failed to load textures\n");
        free_textures(game);
        // osm: i added close window here; 
        return (close_window(game), 1);
    }   
    mlx_hook(game->wid_ptr, 2, 1L << 0, key_press, game);
    mlx_hook(game->wid_ptr, 3, 1L << 1, key_release, game);
    mlx_hook(game->wid_ptr, 17, 0, close_window, game);
    mlx_loop_hook(game->mlx_ptr, game_loop, game);
    mlx_loop(game->mlx_ptr);
    return (0);
}
