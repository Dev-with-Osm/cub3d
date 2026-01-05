/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:59:02 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/04 18:51:12 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

void	process_movement(t_game *game, float delta_time)
{
	float	move_speed;
	float	rotation_speed;
	float	move_angle;
	int		forward;
	int		strafe;

	delta_time = normalize_delta(delta_time);
	move_speed = 4.0f * delta_time;
	rotation_speed = 2.0f * delta_time;
	forward = game->keys->w_pressed - game->keys->s_pressed;
	strafe = game->keys->d_pressed - game->keys->a_pressed;
	move_angle = calculate_move_angle(game, forward, strafe);
	if (move_angle != -1.0f)
		apply_movement(game, move_angle, move_speed);
	apply_rotation(game, rotation_speed);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == LINUX_KEY_ESC)
		close_window(game);
	else if ((keycode == KEY_W) && !game->keys->w_pressed)
		game->keys->w_pressed = 1;
	else if ((keycode == KEY_S) && !game->keys->s_pressed)
		game->keys->s_pressed = 1;
	else if ((keycode == KEY_A) && !game->keys->a_pressed)
		game->keys->a_pressed = 1;
	else if ((keycode == KEY_D) && !game->keys->d_pressed)
		game->keys->d_pressed = 1;
	else if ((keycode == KEY_LEFT) && !game->keys->left_pressed)
		game->keys->left_pressed = 1;
	else if ((keycode == KEY_RIGHT) && !game->keys->right_pressed)
		game->keys->right_pressed = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
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
	return (0);
}
