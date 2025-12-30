/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:03:09 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/30 18:03:32 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

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
