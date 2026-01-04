/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:26:35 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/04 15:38:30 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

int	is_wall(t_game *game, float x, float y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map->map_W
		|| map_y < 0 || map_y >= game->map->map_H)
		return (1);
	return (game->map->map[map_y][map_x] == '1');
}

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

	new_x = game->map->player_x + cos(angle) * speed;
	new_y = game->map->player_y + sin(angle) * speed;
	if (!is_wall(game, new_x, game->map->player_y))
		game->map->player_x = new_x;
	if (!is_wall(game, game->map->player_x, new_y))
		game->map->player_y = new_y;
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
