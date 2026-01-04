/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:47:11 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/04 17:34:07 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

void	put_pixel_safe(t_game *game, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= (int)game->screenWidth || y < 0
		|| y >= (int)game->screenHeight)
		return ;
	offset = (y * game->img->line_length)
		+ (x * (game->img->bits_per_pixel / 8));
	*(int *)(game->img->img_data + offset) = color;
}

int	apply_shading(int color, float distance, int side)
{
	float	brightness;
	int		r;
	int		g;
	int		b;

	brightness = 1.0f - (distance / 15.0f);
	if (brightness < 0.2f)
		brightness = 0.2f;
	if (brightness > 1.0f)
		brightness = 1.0f;
	if (side == 1)
		brightness *= 0.7f;
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	r = (int)(r * brightness);
	g = (int)(g * brightness);
	b = (int)(b * brightness);
	return ((r << 16) | (g << 8) | b);
}
