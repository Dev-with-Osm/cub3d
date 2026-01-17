/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:15:42 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/17 18:07:40 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

void	extra_game_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->max_distance = sqrt(game->screen_height * game->screen_height
			+ game->screen_width * game->screen_width);
	game->wid_ptr = mlx_new_window(game->mlx_ptr, game->screen_width,
			game->screen_height, "CUB3D");
	game->img = create_image(game);
}

void	mlx_launch(t_game *game)
{
	mlx_hook(game->wid_ptr, 2, 1L << 0, key_press, game);
	mlx_hook(game->wid_ptr, 3, 1L << 1, key_release, game);
	mlx_hook(game->wid_ptr, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx_ptr, game_loop, game);
	mlx_loop(game->mlx_ptr);
}

int	main(int ac, char **av)
{
	t_config	cfg;
	t_game		*game;
	t_addr_lst	**gc_list;

	gc_list = get_garbage_collecter();
	game = w_malloc(sizeof(t_game));
	if (!init_structs(game))
	{
		printf("Error: Memory allocation failed\n");
		return (ft_lstc(gc_list), 1);
	}
	if (parsing(ac, av, game, &cfg) == 1)
		return (ft_lstc(gc_list), 1);
	extra_game_init(game);
	if (!load_textures(game, cfg.tex))
	{
		printf("Error: Failed to load textures\n");
		free_mlx_textures(game);
		return (close_window(game), 1);
	}
	mlx_launch(game);
	return (0);
}
