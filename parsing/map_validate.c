/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:50:52 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/15 17:54:51 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ff_alloc(t_ff *f, char **rows, int w, int h)
{
	int	i;

	i = 0;
	f->w = w;
	f->h = h;
	f->ww = w + 2;
	f->hh = h + 2;
	f->grid = (char *)malloc((size_t)f->ww * f->hh);
	f->vis = (unsigned char *)calloc((size_t)f->ww * f->hh, 1);
	f->qx = (int *)malloc(sizeof(int) * f->ww * f->hh);
	f->qy = (int *)malloc(sizeof(int) * f->ww * f->hh);
	f->qh = 0;
	f->qt = 0;
	if (!f->grid || !f->vis || !f->qx || !f->qy)
		return (1);
	memset(f->grid, ' ', (size_t)f->ww * f->hh);
	while (i < h)
	{
		memcpy(f->grid + (i + 1) * f->ww + 1, rows[i], (size_t)w);
		i++;
	}
	return (0);
}

static void	seed_top_bottom(t_ff *f)
{
	int	i;

	i = 0;
	while (i < f->ww)
	{
		f->vis[i] = 1;
		f->qy[f->qt] = 0;
		f->qx[f->qt++] = i;
		f->vis[(f->hh - 1) * f->ww + i] = 1;
		f->qy[f->qt] = f->hh - 1;
		f->qx[f->qt++] = i;
		i++;
	}
}

static void	seed_left_right(t_ff *f)
{
	int	i;
	int	idx;

	i = 0;
	while (i < f->hh)
	{
		idx = i * f->ww;
		if (!f->vis[idx])
		{
			f->vis[idx] = 1;
			f->qy[f->qt] = i;
			f->qx[f->qt++] = 0;
		}
		idx = i * f->ww + (f->ww - 1);
		if (!f->vis[idx])
		{
			f->vis[idx] = 1;
			f->qy[f->qt] = i;
			f->qx[f->qt++] = (f->ww - 1);
		}
		i++;
	}
}

void	ff_seed_border(t_ff *f)
{
	seed_top_bottom(f);
	seed_left_right(f);
}
