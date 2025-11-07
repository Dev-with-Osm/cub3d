#include "cube.h"

int close_window(t_game *game)
{
    if (game->img && game->img->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
    if (game->wid_ptr)
        mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
    
    free_textures(game); 

    ft_lstc(get_garbage_collecter());
    
    exit(0);
}

int is_wall(t_game *game, float new_x, float new_y)
{
    int x = (int)new_x;
    int y = (int)new_y;

    if (x < 0 || y < 0 || x >= game->map->map_W || y >= game->map->map_H)
        return 1; 
    return(game->map->map[y][x] == '1');
}

int is_wall_with_margin(t_game *game, float new_x, float new_y, float margin)
{
    float check_points[4][2] = {
        {new_x + margin, new_y + margin},
        {new_x - margin, new_y + margin},
        {new_x + margin, new_y - margin},
        {new_x - margin, new_y - margin}
    };
    
    int i;
    i = 0;
    while (i < 4)
    {
        if (is_wall(game, check_points[i][0], check_points[i][1]))
            return (1);
        i++;
    }
    return (0);
}

void movement(t_game *game, float *new_x, float *new_y, int check_dir)
{
    if(check_dir == 1)
    {
        *new_x = game->map->pp_x + cos(game->map->player_angle) * MOVE_SPEED;
        *new_y = game->map->pp_y + sin(game->map->player_angle) * MOVE_SPEED;
    }
    else if(check_dir == 2)
    {
        *new_x = game->map->pp_x - cos(game->map->player_angle) * MOVE_SPEED;
        *new_y = game->map->pp_y - sin(game->map->player_angle) * MOVE_SPEED;
    }
    else if(check_dir == 3)
    {
        *new_x = game->map->pp_x + cos(game->map->player_angle - PI/2) * MOVE_SPEED;
        *new_y = game->map->pp_y + sin(game->map->player_angle - PI/2) * MOVE_SPEED;
    }
    else if(check_dir == 4)
    {
        *new_x = game->map->pp_x + cos(game->map->player_angle + PI/2) * MOVE_SPEED;
        *new_y = game->map->pp_y + sin(game->map->player_angle + PI/2) * MOVE_SPEED;
    }
    
    if (!is_wall(game, *new_x, *new_y)) 
    {
        game->map->pp_x = *new_x;
        game->map->pp_y = *new_y;
    }
}

void process_movement(t_game *game, float delta_time)
{
    const float BASE_MOVE_SPEED = 4.0f;
    const float BASE_ROTATION_SPEED = 2.0f;
    const float COLLISION_MARGIN = 0.15f;
    
    float move_speed, rotation_speed;
    float move_angle;
    int forward, strafe;
    
    if (delta_time > 0.1f)
        delta_time = 0.1f;
    else if (delta_time <= 0.0f)
        delta_time = 0.001f;
        
    move_speed = BASE_MOVE_SPEED * delta_time;
    rotation_speed = BASE_ROTATION_SPEED * delta_time;
    
    forward = game->keys->w_pressed - game->keys->s_pressed;
    strafe = game->keys->d_pressed - game->keys->a_pressed;
    
    move_angle = -1.0f;
    
    if (forward != 0 || strafe != 0)
    {
        move_angle = game->map->player_angle;

        if (forward == 1 && strafe == 0)
            move_angle += 0;
        else if (forward == 1 && strafe == 1)
            move_angle += PI/4;
        else if (forward == 0 && strafe == 1)
            move_angle += PI/2;
        else if (forward == -1 && strafe == 1)
            move_angle += 3*PI/4;
        else if (forward == -1 && strafe == 0)
            move_angle += PI;
        else if (forward == -1 && strafe == -1)
            move_angle += 5*PI/4;
        else if (forward == 0 && strafe == -1)
            move_angle -= PI/2; 
        else if (forward == 1 && strafe == -1)
            move_angle -= PI/4;
    }
    
    if (move_angle != -1.0f)
    {
        float new_x = game->map->pp_x + cos(move_angle) * move_speed;
        float new_y = game->map->pp_y + sin(move_angle) * move_speed;
        
        if (!is_wall_with_margin(game, new_x, game->map->pp_y, COLLISION_MARGIN))
            game->map->pp_x = new_x;
        if (!is_wall_with_margin(game, game->map->pp_x, new_y, COLLISION_MARGIN))
            game->map->pp_y = new_y;
    }
    
    if (game->keys->left_pressed)
        game->map->player_angle -= rotation_speed;
    if (game->keys->right_pressed)
        game->map->player_angle += rotation_speed;

    while (game->map->player_angle < 0)
        game->map->player_angle += 2 * PI;
    while (game->map->player_angle >= 2 * PI)
        game->map->player_angle -= 2 * PI;
}

int key_press(int keycode, t_game *game)
{
    if (keycode == LINUX_KEY_ESC || keycode == MAC_KEY_ESC) 
        close_window(game);
    else if ((keycode == KEY_W || keycode == MAC_KEY_W) && !game->keys->w_pressed)
        game->keys->w_pressed = 1;
    else if ((keycode == KEY_S || keycode == MAC_KEY_S) && !game->keys->s_pressed)
        game->keys->s_pressed = 1;
    else if ((keycode == KEY_A || keycode == MAC_KEY_A) && !game->keys->a_pressed)
        game->keys->a_pressed = 1;
    else if ((keycode == KEY_D || keycode == MAC_KEY_D) && !game->keys->d_pressed)
        game->keys->d_pressed = 1;
    else if ((keycode == KEY_LEFT || keycode == MAC_KEY_LEFT) && !game->keys->left_pressed)
        game->keys->left_pressed = 1;
    else if ((keycode == KEY_RIGHT || keycode == MAC_KEY_RIGHT) && !game->keys->right_pressed)
        game->keys->right_pressed = 1;
    return 0;
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
    else if (keycode == KEY_LEFT || keycode == MAC_KEY_LEFT)
        game->keys->left_pressed = 0;
    else if (keycode == KEY_RIGHT || keycode == MAC_KEY_RIGHT)
        game->keys->right_pressed = 0;
    return 0;
}

t_img *create_image(t_game *game)
{
    t_img *img;
    
    img = w_malloc(sizeof(t_img));
    if (!img)
        return NULL;
    img->img_ptr = mlx_new_image(game->mlx_ptr , game->screenWidth, game->screenHeight);
    if (!img->img_ptr) {
        return NULL;
    }
    img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
                                      &img->line_length, &img->endian);
    if (!img->img_data)
    {
        mlx_destroy_image(game->mlx_ptr, img->img_ptr);
        return NULL;
    }
    return (img);
}

void set_player_direction(char c, t_game *game)
{
    if(c == 'E')
        game->map->player_angle = DEG_TO_RAD(0);
    else if(c == 'W')
        game->map->player_angle = DEG_TO_RAD(180);
    else if(c == 'S')
        game->map->player_angle = DEG_TO_RAD(90);
    else if(c == 'N')
        game->map->player_angle = DEG_TO_RAD(270);
}