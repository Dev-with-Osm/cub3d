/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:50:19 by okhourss          #+#    #+#             */
/*   Updated: 2026/01/04 16:47:53 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	scan_row_for_player(const char *row, int y, t_player *out,
		int *found)
{
	int		j;
	char	c;

	j = 0;
	while (row[j])
	{
		c = row[j];
		if (!(c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S'
				|| c == 'E' || c == 'W'))
			return (p_err("invalid character in map"));
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (*found)
				return (p_err("multiple player spawns"));
			*found = 1;
			out->x = j;
			out->y = y;
			out->dir = c;
		}
		j++;
	}
	return (0);
}

int	find_player(char **map, int h, t_player *out)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (i < h)
	{
		if (scan_row_for_player(map[i], i, out, &found))
			return (1);
		i++;
	}
	if (!found)
		return (p_err("missing player spawn"));
	return (0);
}

void	player_to_floor(char **map, int px, int py)
{
	if (map[py][px] == 'N' || map[py][px] == 'S' || map[py][px] == 'E'
		|| map[py][px] == 'W')
		map[py][px] = '0';
}
