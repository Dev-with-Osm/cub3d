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

void process_movement(t_game *game)
{
    static long last_frame_time = 0;
    float delta_time;
    float move_speed, rotation_speed;
    float move_angle;
    int forward, strafe;
    
    // Calculate delta time (time since last frame)
    if (last_frame_time == 0)
    {
        last_frame_time = game->tms->elapsed_microseconds;
        delta_time = 0.016; // Assume 60 FPS for first frame
    }
    else
    {
        long current_time = game->tms->elapsed_microseconds;
        delta_time = (float)(current_time - last_frame_time) / 1000000.0f;
        last_frame_time = current_time;
    }
    
    // Safety clamp for delta time
    if (delta_time > 0.1f || delta_time < 0.0f)
        delta_time = 0.016f; // Cap at 100ms or reset if negative
    
    // Scale movement by delta time
    move_speed = 5.0f * delta_time;      // 5 units per second
    rotation_speed = 3.0f * delta_time;  // 3 radians per second (~172 degrees/sec)
    
    // Calculate movement angle
    forward = game->keys->w_pressed - game->keys->s_pressed;
    strafe = game->keys->d_pressed - game->keys->a_pressed;
    
    move_angle = -1;
    if (forward == 1 && strafe == 0)
        move_angle = game->player->player_angle;
    else if (forward == 1 && strafe == 1)
        move_angle = game->player->player_angle + PI/4;
    else if (forward == 0 && strafe == 1)
        move_angle = game->player->player_angle + PI/2;
    else if (forward == -1 && strafe == 1)
        move_angle = game->player->player_angle + 3*PI/4;
    else if (forward == -1 && strafe == 0)
        move_angle = game->player->player_angle + PI;
    else if (forward == -1 && strafe == -1)
        move_angle = game->player->player_angle + 5*PI/4;
    else if (forward == 0 && strafe == -1)
        move_angle = game->player->player_angle - PI/2;
    else if (forward == 1 && strafe == -1)
        move_angle = game->player->player_angle - PI/4;
    
    // Apply movement with wall sliding
    if (move_angle != -1)
    {
        float new_x = game->player->pp_x + cos(move_angle) * move_speed;
        float new_y = game->player->pp_y + sin(move_angle) * move_speed;
        
        // Separate X and Y collision for wall sliding
        if (!is_wall(game, new_x, game->player->pp_y))
            game->player->pp_x = new_x;
        if (!is_wall(game, game->player->pp_x, new_y))
            game->player->pp_y = new_y;
    }
    
    // Handle rotation
    if (game->keys->left_pressed)
    {
        game->player->player_angle -= rotation_speed;
        if (game->player->player_angle < 0)
            game->player->player_angle += 2 * PI;
    }
    if (game->keys->right_pressed)
    {
        game->player->player_angle += rotation_speed;
        if (game->player->player_angle >= 2 * PI)
            game->player->player_angle -= 2 * PI;
    }
}

int key_press(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
        close_window(game);
    else if (keycode == KEY_W && !game->keys->w_pressed)
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

int key_release(int keycode, t_game *game)
{
    if (keycode == KEY_W)
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