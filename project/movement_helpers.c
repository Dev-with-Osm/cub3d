/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:42:40 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/08 18:00:14 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

// normenette is readey for session test

float	normalize_delta(float delta_time)
{
	if (delta_time > 0.1f)
		return (0.1f);
	if (delta_time <= 0.0f)
		return (0.001f);
	return (delta_time);
}

float	calculate_move_angle(t_game *game, int forward, int strafe)
{
	float	angle;

	if (forward == 0 && strafe == 0)
		return (-1.0f);
	angle = game->map->player_angle;
	if (forward == 1 && strafe == 1)
		angle += PI / 4;
	else if (forward == 0 && strafe == 1)
		angle += PI / 2;
	else if (forward == -1 && strafe == 1)
		angle += 3 * PI / 4;
	else if (forward == -1 && strafe == 0)
		angle += PI;
	else if (forward == -1 && strafe == -1)
		angle += 5 * PI / 4;
	else if (forward == 0 && strafe == -1)
		angle -= PI / 2;
	else if (forward == 1 && strafe == -1)
		angle -= PI / 4;
	return (angle);
}

void	apply_movement(t_game *game, float angle, float speed)
{
	float	new_x;
	float	new_y;

	new_x = game->map->pp_x + cos(angle) * speed;
	new_y = game->map->pp_y + sin(angle) * speed;
	if (!is_wall(game, new_x, game->map->pp_y))
		game->map->pp_x = new_x;
	if (!is_wall(game, game->map->pp_x, new_y))
		game->map->pp_y = new_y;
}

void	apply_rotation(t_game *game, float rotation_speed)
{
	if (game->keys->left_pressed)
		game->map->player_angle -= rotation_speed;
	if (game->keys->right_pressed)
		game->map->player_angle += rotation_speed;
	while (game->map->player_angle < 0)
		game->map->player_angle += 2 * PI;
	while (game->map->player_angle >= 2 * PI)
		game->map->player_angle -= 2 * PI;
}
