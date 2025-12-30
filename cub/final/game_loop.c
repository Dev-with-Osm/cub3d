/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:09:55 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/30 18:13:42 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	game_loop(t_game *game)
{
	long			current_time;
	float			delta_time;
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	current_time = start.tv_sec * 1000000L + start.tv_usec;
	if (game->tms->last_frame_time == 0)
		game->tms->last_frame_time = current_time;
	delta_time = (float)(current_time - game->tms->last_frame_time)
		/ 1000000.0f;
	game->tms->last_frame_time = current_time;
	handle_movement(game, delta_time);
	clear_image(game, BLACK);
	rendering(game);
	mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr,
		game->img->img_ptr, 0, 0);
	gettimeofday(&end, NULL);
	game->tms->elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec);
	return (0);
}
