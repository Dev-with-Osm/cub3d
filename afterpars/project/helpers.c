#include "cube.h"

int close_window(t_game *game)
{
    if (game->img) {
        if (game->img->img_ptr)
            mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
        free(game->img);
    }
    if (game->wid_ptr)
        mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
    exit(0);
}

int is_wall(t_game *game, double new_x, double new_y)
{
    int x = new_x;
    int y = new_y;

    if (x < 0 || y < 0 || x >= game->map_W || y >= game->map_H)
        return 1; 
    return(game->map[y][x] == '1');
}

void movement(t_game *game, double *new_x, double *new_y, int check_dir)
{
    if(check_dir == 1)
    {
        *new_x = game->player->pp_x + cos(game->player->player_angle) * MOVE_SPEED;
        *new_y = game->player->pp_y + sin(game->player->player_angle) * MOVE_SPEED;
    }
    else if(check_dir == 2)
    {
        *new_x = game->player->pp_x - cos(game->player->player_angle) * MOVE_SPEED;
        *new_y = game->player->pp_y - sin(game->player->player_angle) * MOVE_SPEED;
    }
    else if(check_dir == 3)
    {
        *new_x = game->player->pp_x + cos(game->player->player_angle - PI/2) * MOVE_SPEED;
        *new_y = game->player->pp_y + sin(game->player->player_angle - PI/2) * MOVE_SPEED;
    }
    else if(check_dir == 4)
    {
        *new_x = game->player->pp_x + cos(game->player->player_angle + PI/2) * MOVE_SPEED;
        *new_y = game->player->pp_y + sin(game->player->player_angle + PI/2) * MOVE_SPEED;
    }
    
    if (!is_wall(game, *new_x, *new_y)) 
    {
            game->player->pp_x = *new_x;
            game->player->pp_y = *new_y;
    }
}


int key_handler(int keycode, t_game *game)
{
    double new_x;
    double new_y;

    if (keycode == KEY_ESC)
        close_window(game);
    else if (keycode == KEY_W)
        movement(game, &new_x, &new_y, 1);
    else if (keycode == KEY_S)
        movement(game, &new_x, &new_y, 2);
    else if (keycode == KEY_A)
        movement(game, &new_x, &new_y, 3);     
    else if (keycode == KEY_D)
        movement(game, &new_x, &new_y, 4);
    else if (keycode == KEY_LEFT) {
        game->player->player_angle -= ROTATION_SPEED;
        // Normalize angle
        if (game->player->player_angle < 0)
            game->player->player_angle += 2 * PI;
    }
    else if (keycode == KEY_RIGHT) {
        game->player->player_angle += ROTATION_SPEED;
        // Normalize angle  
        if (game->player->player_angle >= 2 * PI)
            game->player->player_angle -= 2 * PI;
    }
    return 0;
}

t_img *create_image(t_game *game)
{
    t_img *img;
    img = malloc(sizeof(t_img));
    if (!img)
        return NULL;
    
    img->img_ptr = mlx_new_image(game->mlx_ptr , game->screenWidth, game->screenHeight);
    if (!img->img_ptr) {
        free(img);
        return NULL;
    }
    
    img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
                                      &img->line_length, &img->endian);
    if (!img->img_data)
    {
        mlx_destroy_image(game->mlx_ptr, img->img_ptr);
        free(img);
        return NULL;
    }
    return (img);
}

void set_player_direction(char c, t_game *game)
{
    if(c == 'E')
        game->player->player_angle = DEG_TO_RAD(0);
    else if(c == 'W')
        game->player->player_angle = DEG_TO_RAD(180);
    else if(c == 'S')
        game->player->player_angle = DEG_TO_RAD(90);
    else if(c == 'N')
        game->player->player_angle = DEG_TO_RAD(270);
}