/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:23:01 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/19 00:23:01 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsing.h"

static void print_rgb(const char *name, t_rgb c)
{
	printf("%s = %d,%d,%d\n", name, c.r, c.g, c.b);
}

int main(int argc, char **argv)
{
	t_config cfg;
	char *first_map;
	const char *names[TEX_MAX] = {"NO", "SO", "WE", "EA"};
	int i;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s path/to/file.cub\n", argv[0]);
		return (1);
	}
	first_map = NULL;
	if (parse_header_file(argv[1], &cfg, &first_map))
		return (1);
	for (i = 0; i < TEX_MAX; ++i)
		printf("%s = %s\n", names[i], cfg.tex[i]);
	print_rgb("F", cfg.floor_rgb);
	print_rgb("C", cfg.ceil_rgb);
	if (first_map)
	{
		printf("First map line after header: %s", first_map);
		free(first_map);
	}
	free_config(&cfg);
	return (0);
}