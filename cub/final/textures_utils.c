/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:40:06 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/30 16:44:52 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

int	calculate_shaded_color(int base_color, float distance, int side)
{
	float	brightness;
	int		r;
	int		g;
	int		b;

	brightness = 1.0 - (distance / 12.0);
	if (brightness < 0.3)
		brightness = 0.3;
	else if (brightness > 1.0)
		brightness = 1.0;
	if (side == 1)
		brightness *= 0.8;
	r = (base_color >> 16) & 0xFF;
	g = (base_color >> 8) & 0xFF;
	b = base_color & 0xFF;
	r = (int)(r * brightness);
	g = (int)(g * brightness);
	b = (int)(b * brightness);
	return ((r << 16) | (g << 8) | b);
}

t_texture	*load_one_texture(void *mlx_ptr, char *path, t_texture *tex)
{
	if (!tex)
		return (NULL);
	tex->img_ptr = mlx_xpm_file_to_image(mlx_ptr, path,
			&tex->width, &tex->height);
	if (!tex->img_ptr)
	{
		fprintf(stderr, "Error: Failed to load texture from %s\n", path);
		return (NULL);
	}
	tex->img_data = mlx_get_data_addr(tex->img_ptr, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
	if (!tex->img_data)
	{
		mlx_destroy_image(mlx_ptr, tex->img_ptr);
		return (NULL);
	}
	return (tex);
}

int	get_texel_color(t_texture *tex, int tex_x, int tex_y)
{
	char	*dst;
	int		bpp_bytes;

	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= tex->height)
		tex_y = tex->height - 1;
	bpp_bytes = tex->bits_per_pixel / 8;
	dst = tex->img_data + (tex_y * tex->line_length) + (tex_x * bpp_bytes);
	return (*(unsigned int *)dst);
}

float	ft_calculate_texture_x_y_step(t_game *game, t_texture *tex, int *tex_x)
{
	float	step;
	float	tex_pos_y;

	*tex_x = (int)(game->wall->wall_x * (float)tex->width);
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= tex->width)
		*tex_x = tex->width - 1;
	step = 1.0f * tex->height / game->wall->wall_height;
	if (game->wall->wall_start < 0)
		tex_pos_y = (float)(-game->wall->wall_start) * step;
	else
		tex_pos_y = 0.0f;
	return (tex_pos_y);
}

t_texture	*ft_handle_texture_selection(t_game *game,
	int x, int wall_start, int wall_end)
{
	t_texture	*tex;

	int (color), (side), (y);
	if (game->wall->wall_face == NORTH)
		tex = game->textures->north;
	else if (game->wall->wall_face == SOUTH)
		tex = game->textures->south;
	else if (game->wall->wall_face == EAST)
		tex = game->textures->east;
	else
		tex = game->textures->west;
	if (!tex || !tex->img_data)
	{
		decide_side(&side, game->wall->wall_face);
		color = calculate_shaded_color(0x808080, game->wall->wall_distance,
				side);
		y = wall_start;
		while (y < wall_end)
		{
			put_pixel_safe(game, x, y, color);
			y++;
		}
		return (NULL);
	}
	return (tex);
}
