/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:39:21 by okhourss          #+#    #+#             */
/*   Updated: 2025/12/30 11:07:45 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	validate_tex_path(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (p_err("cannot open texture path"));
	close(fd);
	return (0);
}

static int	set_texture(t_config *cfg, t_tex_id id, char *path)
{
	if (cfg->has_tex[id])
	{
		return (p_err("duplicate texture directive"));
	}
	if (validate_tex_path(path))
	{
		return (1);
	}
	cfg->tex[id] = path;
	cfg->has_tex[id] = 1;
	return (0);
}

int	parse_tex_line(const char *s, char **out)
{
	const char	*beg;
	size_t		len;

	if (p_take_token(&s, &beg, &len))
		return (1);
	if (p_expect_eol(&s))
		return (1);
	return (p_dup_token(beg, len, out));
}

int	parse_rgb_line(const char *s, t_rgb *out)
{
	if (p_read_uint_255(&s, &out->r))
		return (1);
	if (p_expect_char(&s, ','))
		return (1);
	if (p_read_uint_255(&s, &out->g))
		return (1);
	if (p_expect_char(&s, ','))
		return (1);
	if (p_read_uint_255(&s, &out->b))
		return (1);
	return (p_expect_eol(&s));
}

int	dispatch_header_line(char *line, t_config *cfg)
{
	const char	*p = line;
	char		*path;

	p_skip_spaces(&p);
	if (*p == '\0' || *p == '\n')
		return (0);
	if (!ft_strncmp(p, "NO", 2) && p_is_space(p[2]))
		return (parse_tex_line(p + 2, &path) || set_texture(cfg, TEX_NO, path));
	if (!ft_strncmp(p, "SO", 2) && p_is_space(p[2]))
		return (parse_tex_line(p + 2, &path) || set_texture(cfg, TEX_SO, path));
	if (!ft_strncmp(p, "WE", 2) && p_is_space(p[2]))
		return (parse_tex_line(p + 2, &path) || set_texture(cfg, TEX_WE, path));
	if (!ft_strncmp(p, "EA", 2) && p_is_space(p[2]))
		return (parse_tex_line(p + 2, &path) || set_texture(cfg, TEX_EA, path));
	if (*p == 'F' && p_is_space(p[1]))
		return (handle_color_line(p, cfg, 1));
	if (*p == 'C' && p_is_space(p[1]))
		return (handle_color_line(p, cfg, 0));
	return (p_err("unknown header directive"));
}
