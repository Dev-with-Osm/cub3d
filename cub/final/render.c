/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:45:16 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/02 21:02:44 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

void	draw_wall_column(t_game *game, int x)
{
	t_texture	*tex;
	int			tex_x;

	tex = get_wall_texture(game);
	if (!tex || !tex->img_data)
		return ;
	tex_x = get_texture_x(game, tex);
	get_draw_boundaries(game);
	draw_wall_pixels(game, x, tex_x);
}

void	rendering(t_game *game)
{
	int		x;

	float (cos_correction), (camera_x), (ray_angle);
	x = 0;
	while (x < (int)game->screenWidth)
	{
		camera_x = 2.0f * x / game->screenWidth - 1.0f;
		ray_angle = game->map->player_angle + atanf(camera_x
				* tanf(game->fov / 2.0f));
		game->ray->ray_angle = ray_angle;
		game->wall->wall_distance = cast_ray(game, ray_angle);
		cos_correction = cos(game->ray->ray_angle - game->map->player_angle);
		game->wall->wall_distance *= cos_correction;
		if (game->wall->wall_distance < 0.1f)
			game->wall->wall_distance = 0.1f;
		game->wall->wall_height = game->screenHeight
			/ game->wall->wall_distance;
		game->wall->wall_start = (game->screenHeight - game->wall->wall_height)
			/ 2.0f;
		game->wall->wall_end = game->wall->wall_start + game->wall->wall_height;
		draw_wall_column(game, x);
		draw_floor_ceiling(game, x);
		x++;
	}
}
