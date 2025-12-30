#include "cub3d.h" 

// ready for session norminette test

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
	if (game->wid_ptr)
		mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
	free_mlx_textures(game); 
	ft_lstc(get_garbage_collecter());
	exit(0);
}

// int	is_wall(t_game *game, float new_x, float new_y)
// {
// 	int	x;
// 	int	y;

// 	y = (int)new_y;
// 	x = (int)new_x;
// 	if (x < 0 || y < 0 || x >= game->map->map_W || y >= game->map->map_H)
// 		return 1; 
// 	return(game->map->map[y][x] == '1');
// }

// t_img	*create_image(t_game *game)
// {
// 	t_img	*img;
    
// 	img = w_malloc(sizeof(t_img));
// 	if (!img)
// 		return NULL;
// 	img->img_ptr = mlx_new_image(game->mlx_ptr , game->screenWidth, game->screenHeight);
// 	if (!img->img_ptr) {
// 		return NULL;
// 	}
// 	img->img_data = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
// 										&img->line_length, &img->endian);
// 	if (!img->img_data)
// 	{
// 		mlx_destroy_image(game->mlx_ptr, img->img_ptr);
// 		return NULL;
// 	}
// 	return (img);
// }