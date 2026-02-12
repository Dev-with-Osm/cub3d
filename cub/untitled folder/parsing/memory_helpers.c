/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:20:02 by okhourss          #+#    #+#             */
/*   Updated: 2025/12/18 19:00:55 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
