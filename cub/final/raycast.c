/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:49:40 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/16 16:18:03 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

static void	init_ray(t_game *game, float angle, t_dda *dda)
{
	delta_distance(dda, angle);
	dda->map_x = (int)game->map->player_x;
	dda->map_y = (int)game->map->player_y;
	if (dda->move_rate_x < 0)
	{
		dda->step_x = -1;
		side_dist(game, dda, 0);
	}
	else
	{
		dda->step_x = 1;
		side_dist(game, dda, 1);
	}
	if (dda->move_rate_y < 0)
	{
		dda->step_y = -1;
		side_dist(game, dda, 2);
	}
	else
	{
		dda->step_y = 1;
		side_dist(game, dda, 3);
	}
}

static float	perform_dda(t_game *game, t_dda *dda)
{
	dda->dda_hit = 0;
	while (!dda->dda_hit)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_dist_x;
			dda->map_x += dda->step_x;
			dda->side = 0;
		}
		else
		{
			dda->side_dist_y += dda->delta_dist_y;
			dda->map_y += dda->step_y;
			dda->side = 1;
		}
		if (dda->map_x < 0 || dda->map_x >= game->map->map_W
			|| dda->map_y < 0 || dda->map_y >= game->map->map_H)
			return (game->max_distance);
		if (game->map->map[dda->map_y][dda->map_x] == '1')
			dda->dda_hit = 1;
	}
	if (dda->side == 0)
		return (dda->side_dist_x - dda->delta_dist_x);
	return (dda->side_dist_y - dda->delta_dist_y);
}

static void	calculate_wall_data(t_game *game, t_dda *dda, float distance)
{
	float	wall_hit;

	if (dda->side == 0)
	{
		wall_hit = game->map->player_y + distance * dda->move_rate_y;
		if (dda->step_x > 0)
			game->wall->wall_face = EAST;
		else
			game->wall->wall_face = WEST;
	}
	else
	{
		wall_hit = game->map->player_x + distance * dda->move_rate_x;
		if (dda->step_y > 0)
			game->wall->wall_face = SOUTH;
		else
			game->wall->wall_face = NORTH;
	}
	game->wall->wall_x = wall_hit - floor(wall_hit);
	if (game->wall->wall_x < 0.0f)
		game->wall->wall_x += 1.0f;
	if (game->wall->wall_x >= 1.0f)
		game->wall->wall_x = 0.99999f;
	game->wall->wall_distance = distance;
}

float	cast_ray(t_game *game, float angle)
{
	t_dda	dda;
	float	distance;

	init_ray(game, angle, &dda);
	distance = perform_dda(game, &dda);
	if (distance < game->max_distance)
		calculate_wall_data(game, &dda, distance);
	return (distance);
}
