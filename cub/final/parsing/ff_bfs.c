/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ff_bfs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:48:26 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/15 17:52:58 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	q_pop(t_ff *f, int *y, int *x)
{
	*y = f->qy[f->qh];
	*x = f->qx[f->qh];
	f->qh++;
}

static void	q_push(t_ff *f, int y, int x)
{
	f->qy[f->qt] = y;
	f->qx[f->qt] = x;
	f->qt++;
}

static void	try_push_air(t_ff *f, int ny, int nx)
{
	int	idx;

	if (ny < 0 || ny >= f->hh || nx < 0 || nx >= f->ww)
		return ;
	idx = ny * f->ww + nx;
	if (f->grid[idx] == ' ' && !f->vis[idx])
	{
		f->vis[idx] = 1;
		q_push(f, ny, nx);
	}
}

static void	process_neighbors(t_ff *f, int y, int x)
{
	try_push_air(f, y - 1, x);
	try_push_air(f, y + 1, x);
	try_push_air(f, y, x - 1);
	try_push_air(f, y, x + 1);
}

void	ff_bfs_air(t_ff *f)
{
	int	y;
	int	x;

	while (f->qh < f->qt)
	{
		q_pop(f, &y, &x);
		process_neighbors(f, y, x);
	}
}
