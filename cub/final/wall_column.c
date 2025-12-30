/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_column.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:36:43 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/30 16:39:32 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

t_texture	*get_wall_texture(t_game *game)
{
	if (game->wall->wall_face == NORTH)
		return (game->textures->north);
	if (game->wall->wall_face == SOUTH)
		return (game->textures->south);
	if (game->wall->wall_face == EAST)
		return (game->textures->east);
	return (game->textures->west);
}

int	get_texture_x(t_game *game, t_texture *tex)
{
	int	tex_x;

	tex_x = (int)(game->wall->wall_x * tex->width); // Convert the wall hitting point (0.0-1.0) to pixel coordinate
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	return (tex_x);
}

void	get_draw_boundaries(t_game *game, int *draw_start, int *draw_end)
{
	*draw_start = (int)game->wall->wall_start;
	*draw_end = (int)game->wall->wall_end;
	if (*draw_start < 0)
		*draw_start = 0;
	if (*draw_end > (int)game->screenHeight)
		*draw_end = (int)game->screenHeight;
}

static float	init_texture_pos(t_game *game, t_texture *tex)
{
	float	tex_step;
	float	tex_pos;

	tex_step = (float)tex->height / game->wall->wall_height;
	tex_pos = 0;
	if ((int)game->wall->wall_start < 0)
		tex_pos = -(int)game->wall->wall_start * tex_step;
	return (tex_pos);
}

void	draw_wall_pixels(t_game *game, int x, int tex_x, int draw_start, int draw_end) // to handle later
{
	t_texture	*tex;

	int (y), (tex_y), (color), (side);
	float (tex_step), (tex_pos);
	tex = get_wall_texture(game);
	tex_step = (float)tex->height / game->wall->wall_height;
	tex_pos = init_texture_pos(game, tex);
	if (game->wall->wall_face == EAST || game->wall->wall_face == WEST)
		side = 0;
	else
		side = 1;
	y = draw_start;
	while (y < draw_end) // For each screen pixel from top to bottom of wall
	{
		tex_y = (int)tex_pos % tex->height;// Which texture row to use
		if (tex_y < 0)
			tex_y += tex->height;
		color = get_pixel_color(tex, tex_x, tex_y);// Get color from texture
		color = apply_shading(color, game->wall->wall_distance, side);// Darken if far
		put_pixel_safe(game, x, y, color);// Draw the pixel
		tex_pos += tex_step;// Move to next texture row
		y++;// Move to next screen row
	}
}
