/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:57:23 by okhourss          #+#    #+#             */
/*   Updated: 2026/01/05 13:50:58 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	ff_touch_outside_air(t_ff *f, char **map)
{
	char	c;

	int (i), (j), (gy), (gx);
	i = 0;
	while (i < f->h)
	{
		j = 0;
		while (j < f->w)
		{
			c = map[i][j];
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				gy = i + 1;
				gx = j + 1;
				if (f->vis[(gy - 1) * f->ww + gx] || f->vis[(gy + 1) * f->ww
						+ gx] || f->vis[gy * f->ww + (gx - 1)]
					|| f->vis[gy * f->ww + (gx + 1)])
					return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int	closed_by_floodfill(char **map, int w, int h)
{
	t_ff	f;

	if (ff_alloc(&f, map, w, h))
		return (p_err("malloc failed"));
	ff_seed_border(&f);
	ff_bfs_air(&f);
	if (ff_touch_outside_air(&f, map))
	{
		free(f.grid);
		free(f.vis);
		free(f.qx);
		free(f.qy);
		return (p_err("map not closed (reachable void)"));
	}
	free(f.grid);
	free(f.vis);
	free(f.qx);
	free(f.qy);
	return (0);
}

int	collect_map_lines(int fd, char ***buf, int *n, t_config *cfg)
{
	char		*line;
	const char	*p;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		p = line;
		p_skip_spaces(&p);
		if (!cfg->header_done)
		{
			if (*p && *p != '\n' && dispatch_header_line(line, cfg))
				return (free(line), 1);
			mark_header_done(cfg);
			free(line);
			continue ;
		}
		if (is_header_directive(p))
			return (free(line),
				p_err("unexpected header directive after header complete"));
		if (append_line(buf, n, line))
			return (1);
	}
	return (0);
}

int	build_map(t_map *m, char ***buf, int n, t_build *o)
{
	t_player	p;

	if (comp_map(&m->map, &o->h, buf, n))
		return (1);
	if (pad_map(&m->map, &o->h, &o->w))
	{
		free_map_n(m->map, o->h);
		return (p_err("malloc failed"));
	}
	if (find_player(m->map, o->h, &p))
	{
		return (1);
	}
	m->player_direction = p.dir;
	o->px = p.x;
	o->py = p.y;
	if (closed_by_floodfill(m->map, o->w, o->h))
	{
		return (1);
	}
	return (0);
}

int	parse_cub_file(const char *name, t_config *cfg, t_map *m)
{
	char	**buf;
	int		n;
	t_build	o;

	buf = NULL;
	n = 0;
	if (p_check_cub_ext(name))
		return (1);
	ft_memset(m, 0, sizeof(*m));
	ft_memset(cfg, 0, sizeof(*cfg));
	if (load_lines(name, cfg, &buf, &n))
	{
		free_lines_buf(buf, n);
		return (1);
	}
	if (build_map(m, &buf, n, &o))
		return (1);
	player_to_floor(m->map, o.px, o.py);
	m->map_W = o.w;
	m->map_H = o.h;
	m->player_x = o.px + 0.5f;
	m->player_y = o.py + 0.5f;
	return (0);
}
