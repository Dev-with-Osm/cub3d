#include "cub3d.h"


// int	close_window(t_game *game)
// {
// 	if (game->img && game->img->img_ptr)
// 		mlx_destroy_image(game->mlx_ptr, game->img->img_ptr);
// 	if (game->wid_ptr)
// 		mlx_destroy_window(game->mlx_ptr, game->wid_ptr);
// 	free_textures(game);
// 	ft_lstc(get_garbage_collecter());
// 	exit(0);
// 	return (0);
// }

int	game_loop(t_game *game)
{
	long   	current_time;
	float  	delta_time;
    
	struct timeval (start), (end);
	gettimeofday(&start, NULL);
	current_time = start.tv_sec * 1000000L + start.tv_usec;
	if (game->tms->last_frame_time == 0)
		game->tms->last_frame_time = current_time;
	delta_time = (float)(current_time - game->tms->last_frame_time) / 1000000.0f;
	game->tms->last_frame_time = current_time;
	handle_movement(game, delta_time);
	clear_image(game, BLACK);
	rendering(game);
	mlx_put_image_to_window(game->mlx_ptr, game->wid_ptr, game->img->img_ptr, 0, 0);
	gettimeofday(&end, NULL);
	game->tms->elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L +
										(end.tv_usec - start.tv_usec);
	return (0);
}