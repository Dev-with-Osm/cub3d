/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 13:53:03 by okhourss          #+#    #+#             */
/*   Updated: 2026/01/17 18:07:40 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

unsigned int	rgb_to_hex(t_rgb c)
{
	return ((c.r << 16) | (c.g << 8) | c.b);
}

int	parsing(int argc, char **argv, t_game *game, t_config *cfg)
{
	if (argc != 2)
		return (printf("Usage: %s file.cub\n", argv[0]), 1);
	if (parse_cub_file(argv[1], cfg, game->map))
		return (1);
	game->screen_height = SCREEN_H;
	game->screen_width = SCREEN_W;
	game->fov = FOV * PI / 180.0;
	game->textures->ceiling_color = rgb_to_hex(cfg->ceil_rgb);
	game->textures->floor_color = rgb_to_hex(cfg->floor_rgb);
	set_player_direction(game->map->player_direction, game);
	return (0);
}
