#include "cub3d.h" 

void	free_mlx_textures(t_game *game)
{
	if (!game->mlx_ptr || !game->textures)
		return;
	if (game->textures->north && game->textures->north->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->north->img_ptr);
	if (game->textures->south && game->textures->south->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->south->img_ptr);
	if (game->textures->east && game->textures->east->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->east->img_ptr);
	if (game->textures->west && game->textures->west->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->west->img_ptr);
}

t_texture	*load_texture(void *mlx_ptr, char *path)
{
	t_texture *tex;

	tex = w_malloc(sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->img_ptr = mlx_xpm_file_to_image(mlx_ptr, path, &tex->width, &tex->height);
	if (!tex->img_ptr)
	{
		fprintf(stderr, "Error: Failed to load %s\n", path);
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

int	load_textures(t_game *game)
{
	game->textures->north = load_texture(game->mlx_ptr, "./textures/w.xpm");
	game->textures->south = load_texture(game->mlx_ptr, "./textures/w.xpm");
	game->textures->east = load_texture(game->mlx_ptr, "./textures/w.xpm");
	game->textures->west = load_texture(game->mlx_ptr, "./textures/w.xpm");
	if (!game->textures->north || !game->textures->south ||
		!game->textures->east || !game->textures->west )
		return (0);
	return (1);
}

void	free_textures(t_game *game)
{
	if (!game->mlx_ptr || !game->textures)
		return;
	if (game->textures->north && game->textures->north->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->north->img_ptr);
	if (game->textures->south && game->textures->south->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->south->img_ptr);
	if (game->textures->east && game->textures->east->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->east->img_ptr);
	if (game->textures->west && game->textures->west->img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->textures->west->img_ptr);
}

int	get_pixel_color(t_texture *tex, int x, int y)
{
	char *pixel;
	
	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
		return (0);
	pixel = tex->img_data + (y * tex->line_length) + (x * (tex->bits_per_pixel / 8));
	return (*(int *)pixel);
}