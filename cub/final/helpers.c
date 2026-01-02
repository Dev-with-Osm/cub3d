/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:03:09 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/02 21:00:22 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

void	delta_distance(t_dda *dda, float angle)
{
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

void	side_dist(t_game *game, t_dda *dda, int sign)
{
	if (sign == 0)
	{
		dda->side_dist_x = (game->map->player_x - dda->map_x)
			* dda->delta_dist_x;
	}
	else if (sign == 1)
	{
		dda->side_dist_x = (dda->map_x + 1.0f - game->map->player_x)
			* dda->delta_dist_x;
	}
	else if (sign == 2)
	{
		dda->side_dist_y = (game->map->player_y - dda->map_y)
			* dda->delta_dist_y;
	}
	else if (sign == 3)
	{
		dda->side_dist_y = (dda->map_y + 1.0f - game->map->player_y)
			* dda->delta_dist_y;
	}
}

void	decide_side(int *side, int face)
{
	if (face == EAST || face == WEST)
		*side = 0;
	else
		*side = 1;
}

int	close_window(t_game *game)
{
	if (game->img && game->img->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
	free_mlx_textures(game);
	if (game->wid_ptr && game->mlx_ptr)
		mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
	if (game->mlx_ptr)
	{
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	ft_lstc(get_garbage_collecter());
	exit(0);
}
