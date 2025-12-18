/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:17:25 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/18 12:38:41 by hoel-mos         ###   ########.fr       */
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

void	clear_image(t_game *game, int color)
{
	int	*pixels;
	int	total;
	int	i;

	i = 0;
	total = game->screenWidth * game->screenHeight;
	pixels = (int *)game->img->img_data;
	while (i < total)
	{
		pixels[i] = color;
		i++;
	}
}

int	apply_shading(int color, float distance, int side)
{
	float	brightness;

	int (r), (g), (b);
	// Calculate brightness based on distance
	brightness = 1.0f - (distance / 15.0f);
	if (brightness < 0.2f)
		brightness = 0.2f;
	if (brightness > 1.0f)
		brightness = 1.0f;
	// Make one side darker
	if (side == 1)
		brightness *= 0.7f;
	// Extract RGB
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	// Apply brightness
	r = (int)(r * brightness);
	g = (int)(g * brightness);
	b = (int)(b * brightness);
	return ((r << 16) | (g << 8) | b);
}
