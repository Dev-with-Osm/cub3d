#include "cube.h"

int close_window(t_game *game)
{
    // 1. Destroy the current window image and window
    if (game->img && game->img->img_ptr)
        mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
    if (game->wid_ptr)
        mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
    
    // 2. Destroy all texture images
    free_mlx_textures(game); 

    // 3. Clean up all tracked memory
    ft_lstc(get_garbage_collecter());
    
    exit(0);
}

int is_wall(t_game *game, float new_x, float new_y)
{
    int x = new_x;
    int y = new_y;

    if (x < 0 || y < 0 || x >= game->map_W || y >= game->map_H)
        return 1; 
    return(game->map[y][x] == '1');
}

void movement(t_game *game, float *new_x, float *new_y, int check_dir)
{
    // Calculate new position based on direction
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
    
    // Only move if new position is not a wall
    if (!is_wall(game, *new_x, *new_y)) 
    {
        game->player->pp_x = *new_x;
        game->player->pp_y = *new_y;
    }
}

// Change function signature to match what the game_loop passes
void process_movement(t_game *game, float delta_time)
{
    // Define base speeds (Units per Second and Radians per Second)
    const float BASE_MOVE_SPEED = 4.0f;
    const float BASE_ROTATION_SPEED = 2.0f;
    
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
        move_angle = game->player->player_angle;

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
        float new_x = game->player->pp_x + cos(move_angle) * move_speed;
        float new_y = game->player->pp_y + sin(move_angle) * move_speed;
        
        if (!is_wall(game, new_x, game->player->pp_y))
            game->player->pp_x = new_x;
        if (!is_wall(game, game->player->pp_x, new_y))
            game->player->pp_y = new_y;
    }
    
    if (game->keys->left_pressed)
        game->player->player_angle -= rotation_speed;
    if (game->keys->right_pressed)
        game->player->player_angle += rotation_speed;

    while (game->player->player_angle < 0)
        game->player->player_angle += 2 * PI;
    while (game->player->player_angle >= 2 * PI)
        game->player->player_angle -= 2 * PI;
}

int key_press(int keycode, t_game *game)
{
    if (keycode == KEY_ESC) // Now KEY_ESC is 65307
        close_window(game);
    else if (keycode == KEY_W && !game->keys->w_pressed) // Now KEY_W is 119
        game->keys->w_pressed = 1;
    else if (keycode == KEY_S && !game->keys->s_pressed)
        game->keys->s_pressed = 1;
    else if (keycode == KEY_A && !game->keys->a_pressed)
        game->keys->a_pressed = 1;
    else if (keycode == KEY_D && !game->keys->d_pressed)
        game->keys->d_pressed = 1;
    else if (keycode == KEY_LEFT && !game->keys->left_pressed)
        game->keys->left_pressed = 1;
    else if (keycode == KEY_RIGHT && !game->keys->right_pressed)
        game->keys->right_pressed = 1;
    return 0;
}

// Similarly, simplify key_release:
int key_release(int keycode, t_game *game)
{
    if (keycode == KEY_W) // Now KEY_W is 119
        game->keys->w_pressed = 0;
    else if (keycode == KEY_S)
        game->keys->s_pressed = 0;
    else if (keycode == KEY_A)
        game->keys->a_pressed = 0;
    else if (keycode == KEY_D)
        game->keys->d_pressed = 0;
    else if (keycode == KEY_LEFT)
        game->keys->left_pressed = 0;
    else if (keycode == KEY_RIGHT)
        game->keys->right_pressed = 0;
    return 0;
}

t_img *create_image(t_game *game)
{
    t_img *img;
    // --- USE W_MALLOC ---
    img = w_malloc(sizeof(t_img));
    if (!img)
        return NULL;
    
    img->img_ptr = mlx_new_image(game->mlx_ptr , game->screenWidth, game->screenHeight);
    if (!img->img_ptr) {
        // Since img is w_malloc'd, no explicit free is needed here. GC handles it.
        return NULL;
    }
    
    img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
                                      &img->line_length, &img->endian);
    if (!img->img_data)
    {
        mlx_destroy_image(game->mlx_ptr, img->img_ptr);
        // img is w_malloc'd. No explicit free.
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