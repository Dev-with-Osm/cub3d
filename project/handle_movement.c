/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:42:25 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/08 17:53:15 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

// normenette is readey for session test

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

void	check_keys(t_game *game, float *move_x, float *move_y)
{
	if (game->keys->w_pressed)
	{
		*move_x += cos(game->map->player_angle) * MOVE_SPEED;
		*move_y += sin(game->map->player_angle) * MOVE_SPEED;
	}
	if (game->keys->s_pressed)
	{
		*move_x -= cos(game->map->player_angle) * MOVE_SPEED;
		*move_y -= sin(game->map->player_angle) * MOVE_SPEED;
	}
	if (game->keys->d_pressed)
	{
		*move_x += cos(game->map->player_angle + PI / 2.0f) * MOVE_SPEED;
		*move_y += sin(game->map->player_angle + PI / 2.0f) * MOVE_SPEED;
	}
	if (game->keys->a_pressed)
	{
		*move_x -= cos(game->map->player_angle + PI / 2.0f) * MOVE_SPEED;
		*move_y -= sin(game->map->player_angle + PI / 2.0f) * MOVE_SPEED;
	}
}

void	handle_movement(t_game *game, float delta_time)
{
	float (move_x), (move_y), (new_x), (new_y);
	move_x = 0;
	move_y = 0;
	(void)delta_time;
	check_keys(game, &move_x, &move_y);
	// Apply movement with collision
	new_x = game->map->pp_x + move_x;
	new_y = game->map->pp_y + move_y;
	if (!is_wall(game, new_x, game->map->pp_y))
		game->map->pp_x = new_x;
	if (!is_wall(game, game->map->pp_x, new_y))
		game->map->pp_y = new_y;
	// Rotation
	if (game->keys->left_pressed)
		game->map->player_angle -= ROT_SPEED;
	if (game->keys->right_pressed)
		game->map->player_angle += ROT_SPEED;
	// Normalize angle
	while (game->map->player_angle < 0)
		game->map->player_angle += 2.0f * PI;
	while (game->map->player_angle >= 2.0f * PI)
		game->map->player_angle -= 2.0f * PI;
}
