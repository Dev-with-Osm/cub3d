/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:17:27 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/18 13:43:15 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

// normenette is readey for session test

static void	delta_distance(t_dda *dda, float angle, t_game *game) // go for another explanation for this func
{
	dda->map_x = (int)game->map->pp_x;
	dda->map_y = (int)game->map->pp_y;
	dda->ray_dir_x = cos(angle);
	dda->ray_dir_y = sin(angle);
	if (dda->ray_dir_x == 0)
		dda->delta_dist_x = 1e30;
	else
		dda->delta_dist_x = fabs(1.0f / dda->ray_dir_x);
	if (dda->ray_dir_y == 0)
		dda->delta_dist_y = 1e30;
	else
		dda->delta_dist_y = fabs(1.0f / dda->ray_dir_y);
}

static void	init_ray(t_game *game, float angle, t_dda *dda)
{
	delta_distance(dda, angle, game);
	if (dda->ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (game->map->pp_x - dda->map_x) * dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0f - game->map->pp_x)
			* dda->delta_dist_x;
	}
	if (dda->ray_dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (game->map->pp_y - dda->map_y) * dda->delta_dist_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0f - game->map->pp_y)
			* dda->delta_dist_y;
	}
}

static float	perform_dda(t_game *game, t_dda *dda)
{
	dda->dda_hit = 0;
	while (!dda->dda_hit)
	{
		if (dda->side_dist_x < dda->side_dist_y) // Jump to next grid square
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
			|| dda->map_y < 0 || dda->map_y >= game->map->map_H) // Check boundaries
			return (game->max_distance);
		if (game->map->map[dda->map_y][dda->map_x] == '1') // Check if we hit a wall
			dda->dda_hit = 1;
	}
	if (dda->side == 0) // Calculate perpendicular distance
		return (dda->side_dist_x - dda->delta_dist_x);
	return (dda->side_dist_y - dda->delta_dist_y);
}

static void	calculate_wall_data(t_game *game, t_dda *dda, float distance)
{
	float	wall_hit;

	if (dda->side == 0) // We hit a VERTICAL wall
	{
		wall_hit = game->map->pp_y + distance * dda->ray_dir_y;
		if (dda->step_x > 0)
			game->wall->wall_face = EAST;
		else
			game->wall->wall_face = WEST;
	}
	else // We hit a HORIZONTAL wall
	{
		wall_hit = game->map->pp_x + distance * dda->ray_dir_x;
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

	// Initialize ray
	init_ray(game, angle, &dda);
	// Perform DDA
	distance = perform_dda(game, &dda);
	// Calculate wall hit data
	if (distance < game->max_distance) // this max distance might needs to be modified or removed
		calculate_wall_data(game, &dda, distance);
	return (distance);
}
