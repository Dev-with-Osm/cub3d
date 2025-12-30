// #include "parsing/parsing.h"
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
	game->screenHeight = screenH;
	game->screenWidth = screenW;
	game->fov = 60 * PI / 180.0;
	game->textures->ceiling_color = rgb_to_hex(cfg->ceil_rgb);
	game->textures->floor_color = rgb_to_hex(cfg->floor_rgb);
	return (0);
}
