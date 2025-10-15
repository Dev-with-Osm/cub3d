/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_norm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:11:22 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/13 11:11:23 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	max_width(char **rows, int h)
{
	int	i;
	int	mw;
	int	len;

	i = 0;
	mw = 0;
	while (i < h)
	{
		len = (int)strlen(rows[i]);
		if (len > mw)
			mw = len;
		i++;
	}
	return (mw);
}

static int	pad_to_width(char **row, int w)
{
	int		len;
	char	*p;

	len = (int)strlen(*row);
	if (len >= w)
		return (0);
	p = (char *)malloc((size_t)w + 1);
	if (!p)
		return (1);
	memcpy(p, *row, (size_t)len);
	memset(p + len, ' ', (size_t)(w - len));
	p[w] = '\0';
	free(*row);
	*row = p;
	return (0);
}

int	pad_rows(char ***rows, int *h, int *w)
{
	int	i;

	*w = max_width(*rows, *h);
	i = 0;
	while (i < *h)
	{
		if (pad_to_width(&(*rows)[i], *w))
			return (1);
		i++;
	}
	return (0);
}

int	handle_color_line(const char *p, t_config *cfg, int is_floor)
{
	if (is_floor)
	{
		if (cfg->floor_set)
			return (p_err("duplicate F color"));
		if (parse_rgb_line(p + 2, &cfg->floor_rgb))
			return (1);
		cfg->floor_set = 1;
		return (0);
	}
	if (cfg->ceil_set)
		return (p_err("duplicate C color"));
	if (parse_rgb_line(p + 2, &cfg->ceil_rgb))
		return (1);
	cfg->ceil_set = 1;
	return (0);
}
