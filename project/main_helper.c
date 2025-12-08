/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:42:31 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/08 17:58:56 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

static void	init_structs_data(t_game *game)
{
	game->keys->w_pressed = 0;
	game->keys->s_pressed = 0;
	game->keys->a_pressed = 0;
	game->keys->d_pressed = 0;
	game->keys->left_pressed = 0;
	game->keys->right_pressed = 0;
	game->tms->last_frame_time = 0;
}

int	init_structs(t_game *game)
{
	game->map = w_malloc(sizeof(t_map));
	game->ray = w_malloc(sizeof(t_ray));
	game->img = w_malloc(sizeof(t_img));
	game->wall = w_malloc(sizeof(t_wall));
	game->keys = w_malloc(sizeof(t_keys));
	game->tms = w_malloc(sizeof(t_time));
	game->textures = w_malloc(sizeof(t_textures));
	if (!game->map || !game->ray || !game->img || !game->wall
		|| !game->keys || !game->tms || !game->textures)
		return (0);
	game->textures->north = w_malloc(sizeof(t_texture));
	game->textures->south = w_malloc(sizeof(t_texture));
	game->textures->east = w_malloc(sizeof(t_texture));
	game->textures->west = w_malloc(sizeof(t_texture));
	game->textures->floor = w_malloc(sizeof(t_texture));
	game->textures->ceiling = w_malloc(sizeof(t_texture));
	if (!game->textures->north || !game->textures->south
		|| !game->textures->east || !game->textures->west
		|| !game->textures->floor || !game->textures->ceiling)
		return (0);
	init_structs_data(game);
	return (1);
}

t_img	*create_image(t_game *game)
{
	t_img	*img;

	img = w_malloc(sizeof(t_img));
	if (!img)
		return (NULL);
	img->img_ptr = mlx_new_image(game->mlx_ptr, game->screenWidth, game->screenHeight);
	if (!img->img_ptr)
		return (NULL);
	img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->img_data)
	{
		mlx_destroy_image(game->mlx_ptr, img->img_ptr);
		return (NULL);
	}
	return (img);
}

void	set_player_direction(char c, t_game *game)
{
	if (c == 'N')
		game->map->player_angle = 270 * PI / 180.0;
	else if (c == 'S')
		game->map->player_angle = 90 * PI / 180.0;
	else if (c == 'E')
		game->map->player_angle = 0 * PI / 180.0;
	else if (c == 'W')
		game->map->player_angle = 180 * PI / 180.0;
}
