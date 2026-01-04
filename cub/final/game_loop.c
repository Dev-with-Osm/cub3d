/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:09:55 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/04 17:01:09 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	game_loop(t_game *game)
{
	struct timeval	now_tv;
	long			now_time;
	float			delta_time;

	gettimeofday(&now_tv, NULL);
	now_time = now_tv.tv_sec * 1000000L + now_tv.tv_usec;
	if (game->tms->last_frame_time == 0)
		game->tms->last_frame_time = now_time;
	delta_time = (float)(now_time - game->tms->last_frame_time) / 1000000.0f;
	game->tms->last_frame_time = now_time;
	process_movement(game, delta_time);
	rendering(game);
	mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr,
		game->img->img_ptr, 0, 0);
	return (0);
}
