#include "cube.h"


void getpp(t_game *game)
{
    int x, y;
    int max_width = 0;

    game->player = malloc(sizeof(t_player));
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

    game->map_H = y;
    game->map_W = max_width;
    game->screenHeight = y * tile;
    game->screenWidth = max_width * tile;
    game->fov = DEG_TO_RAD(60);
}



int game_loop(t_game *game)
{
    // Clear screen
    // clear_image(game, GRAY);
    
    // Draw map
    draw_map(game);
    
    // Draw rays
    draw_all_rays(game);
    
    // Draw player
    draw_player(game);
    
    // Display the image
    mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
    
    return 0;
}

int main()
{
    t_game game;
    char *m;

    m = "111111111111 100000000001 10000W000001 100011100001 100000000101 100000000001 100000001001 111111111111";
    // int fd;

    // if(ac <= 1)
    //     return 0;
    // fd = open(av[1], O_RDONLY);
    
    // tmap()->map = ft_split_n(get_map(fd), '\n');
    game.map = ft_split(m, ' ');
    for(int i = 0; game.map[i]; i++)
        printf("%s\n", game.map[i]);
	game.mlx_ptr = mlx_init();
    getpp(&game);
    game.max_distance = sqrt((game.screenHeight * game.screenHeight) + (game.screenWidth * game.screenWidth));
    game.wid_ptr = mlx_new_window(game.mlx_ptr, game.screenWidth, game.screenHeight, "cub3D Ray Cagameing");
    game.img = create_image(&game);

    mlx_hook(game.wid_ptr, 2, 1L<<0, key_handler, &game);       // Key press
    mlx_hook(game.wid_ptr, 17, 0, close_window, &game);         // Window close
    mlx_loop_hook(game.mlx_ptr, game_loop, &game);           // Main loop

    mlx_loop(game.mlx_ptr);

}