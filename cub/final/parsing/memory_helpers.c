/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:20:02 by okhourss          #+#    #+#             */
/*   Updated: 2025/12/30 10:13:37 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_lines_buf(char **buf, int n)
{
	int	i;

	if (!buf)
		return ;
	i = 0;
	while (i < n)
	{
		free(buf[i]);
		i++;
	}
	free(buf);
}

void	free_map_n(char **map, int h)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < h)
	{
		free(map[i]);
		i++;
	}
	free(map);
}
