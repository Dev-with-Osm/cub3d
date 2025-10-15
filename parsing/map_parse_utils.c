/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:03:35 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/13 12:31:06 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_header_directive(const char *p)
{
	return ((*p == 'N' && p[1] == 'O' && p_is_space((unsigned char)p[2]))
		|| (*p == 'S' && p[1] == 'O' && p_is_space((unsigned char)p[2]))
		|| (*p == 'W' && p[1] == 'E' && p_is_space((unsigned char)p[2]))
		|| (*p == 'E' && p[1] == 'A' && p_is_space((unsigned char)p[2]))
		|| (*p == 'F' && p_is_space((unsigned char)p[1])) || (*p == 'C'
			&& p_is_space((unsigned char)p[1])));
}

int	append_line(char ***buf, int *n, char *line)
{
	char	**tmp2;
	int		k;
	size_t	len;

	len = strlen(line);
	if (len && line[len - 1] == '\n')
		line[len - 1] = '\0';
	tmp2 = (char **)malloc(sizeof(char *) * (*n + 1));
	if (!tmp2)
		return (free(line), p_err("malloc failed"));
	k = 0;
	while (k < *n)
	{
		tmp2[k] = (*buf)[k];
		k++;
	}
	tmp2[*n] = line;
	free(*buf);
	*buf = tmp2;
	*n += 1;
	return (0);
}

void	mark_header_done(t_config *cfg)
{
	if (cfg->floor_set && cfg->ceil_set && cfg->has_tex[TEX_NO]
		&& cfg->has_tex[TEX_SO] && cfg->has_tex[TEX_WE] && cfg->has_tex[TEX_EA])
		cfg->header_done = 1;
}

int	load_lines(const char *name, t_config *cfg, char ***buf, int *n)
{
	int	fd;

	fd = open(name, O_RDONLY);
	if (fd < 0)
		return (p_err("cannot open .cub file"));
	if (collect_map_lines(fd, buf, n, cfg))
	{
		close(fd);
		return (1);
	}
	close(fd);
	if (!cfg->header_done)
		return (p_err("incomplete header (missing F/C or texture)"));
	return (0);
}
