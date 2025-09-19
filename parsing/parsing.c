/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:40:45 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/18 11:40:45 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int is_space(char c)
{
	return (c == ' ' || c == '\t');
}

static void skip_spaces(const char **p)
{
	while (**p && is_space(**p))
		(*p)++;
}

static int is_blank(const char *s)
{
	while (*s && is_space(*s))
		s++;
	return (*s == '\0' || *s == '\n');
}

static int errmsg(const char *msg)
{
	write(2, "Error\n", 6);
	if (msg)
		write(2, msg, (int)strlen(msg));
	write(2, "\n", 1);
	return (1);
}

static int dup_token(const char *beg, size_t len, char **out)
{
	char *dst;

	dst = (char *)malloc(len + 1);
	if (!dst)
		return (errmsg("malloc failed (texture path)"));
	memcpy(dst, beg, len);
	dst[len] = '\0';
	*out = dst;
	return (0);
}

static int take_token(const char **p, const char **beg, size_t *len)
{
	const char *s;

	skip_spaces(p);
	s = *p;
	while (*s && !is_space(*s) && *s != '\n')
		s++;
	if (s == *p)
		return (errmsg("missing texture path"));
	*beg = *p;
	*len = (size_t)(s - *p);
	*p = s;
	return (0);
}

static int expect_eol(const char **p)
{
	skip_spaces(p);
	return ((**p == '\0' || **p == '\n') ? 0 : errmsg("trailing garbage"));
}

static int read_uint_255(const char **p, int *out)
{
	int val;
	int d;

	skip_spaces(p);
	if (!isdigit((unsigned char)**p))
		return (errmsg("expected number"));
	val = 0;
	while (isdigit((unsigned char)**p))
	{
		d = **p - '0';
		if (val > 25 || (val == 25 && d > 5))
			return (errmsg("color out of range (0..255)"));
		val = val * 10 + d;
		(*p)++;
	}
	*out = val;
	return (0);
}

static int expect_char(const char **p, char c)
{
	skip_spaces(p);
	if (**p != c)
		return (errmsg("expected separator"));
	(*p)++;
	return (0);
}

int parse_tex_line(const char *s, char **out)
{
	const char *beg;
	size_t len;

	if (take_token(&s, &beg, &len))
		return (1);
	if (expect_eol(&s))
		return (1);
	return (dup_token(beg, len, out));
}

int parse_rgb_line(const char *s, t_rgb *out)
{
	int r;
	int g;
	int b;

	if (read_uint_255(&s, &r))
		return (1);
	if (expect_char(&s, ','))
		return (1);
	if (read_uint_255(&s, &g))
		return (1);
	if (expect_char(&s, ','))
		return (1);
	if (read_uint_255(&s, &b))
		return (1);
	if (expect_eol(&s))
		return (1);
	out->r = r;
	out->g = g;
	out->b = b;
	return (0);
}

static int validate_tex_path(const char *path)
{
	int fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (errmsg("cannot open texture path"));
	close(fd);
	return (0);
}

static int set_texture(t_config *cfg, t_tex_id id, char *path)
{
	if (cfg->has_tex[id])
	{
		free(path);
		return (errmsg("duplicate texture directive"));
	}
	if (validate_tex_path(path))
	{
		free(path);
		return (1);
	}
	cfg->tex[id] = path;
	cfg->has_tex[id] = 1;
	return (0);
}

static int handle_tex_line(const char *s, t_config *cfg, t_tex_id id)
{
	char *path;

	if (parse_tex_line(s, &path))
		return (1);
	if (set_texture(cfg, id, path))
		return (1);
	return (0);
}

static int handle_rgb_line(const char *s, t_config *cfg, int is_floor)
{
	t_rgb tmp;

	if (parse_rgb_line(s, &tmp))
		return (1);
	if (is_floor && cfg->floor_set)
		return (errmsg("duplicate F color"));
	if (!is_floor && cfg->ceil_set)
		return (errmsg("duplicate C color"));
	if (is_floor)
	{
		cfg->floor_rgb = tmp;
		cfg->floor_set = 1;
	}
	else
	{
		cfg->ceil_rgb = tmp;
		cfg->ceil_set = 1;
	}
	return (0);
}

static int all_header_present(const t_config *cfg)
{
	int i;

	if (!cfg->floor_set || !cfg->ceil_set)
		return (0);
	i = 0;
	while (i < TEX_MAX)
	{
		if (!cfg->has_tex[i])
			return (0);
		i++;
	}
	return (1);
}

int dispatch_header_line(char *line, t_config *cfg)
{
	const char *p;

	p = line;
	skip_spaces(&p);
	if (*p == '\0' || *p == '\n')
		return (0);
	if (!strncmp(p, "NO", 2) && is_space(p[2]))
		return (handle_tex_line(p + 3 - 1, cfg, TEX_NO));
	if (!strncmp(p, "SO", 2) && is_space(p[2]))
		return (handle_tex_line(p + 3 - 1, cfg, TEX_SO));
	if (!strncmp(p, "WE", 2) && is_space(p[2]))
		return (handle_tex_line(p + 3 - 1, cfg, TEX_WE));
	if (!strncmp(p, "EA", 2) && is_space(p[2]))
		return (handle_tex_line(p + 3 - 1, cfg, TEX_EA));
	if (*p == 'F' && is_space(p[1]))
		return (handle_rgb_line(p + 2, cfg, 1));
	if (*p == 'C' && is_space(p[1]))
		return (handle_rgb_line(p + 2, cfg, 0));
	return (errmsg("unknown header directive"));
}

static int check_cub_extension(const char *name)
{
	size_t n;

	n = strlen(name);
	if (n < 5)
		return (errmsg("invalid file name"));
	if (strcmp(name + (n - 4), ".cub") != 0)
		return (errmsg("invalid file extension (expected .cub)"));
	return (0);
}

static void init_config(t_config *cfg)
{
	int i;

	memset(cfg, 0, sizeof(*cfg));
	i = 0;
	while (i < TEX_MAX)
	{
		cfg->tex[i] = NULL;
		cfg->has_tex[i] = 0;
		i++;
	}
	cfg->floor_set = 0;
	cfg->ceil_set = 0;
	cfg->header_done = 0;
}

void free_config(t_config *cfg)
{
	int i;

	i = 0;
	while (i < TEX_MAX)
	{
		free(cfg->tex[i]);
		cfg->tex[i] = NULL;
		i++;
	}
	memset(cfg->has_tex, 0, sizeof(cfg->has_tex));
	cfg->floor_set = 0;
	cfg->ceil_set = 0;
	cfg->header_done = 0;
}

static int is_header_directive_line(const char *line)
{
	const char *p;

	p = line;
	skip_spaces(&p);
	if (!p || *p == '\0' || *p == '\n')
		return (0);
	if (!strncmp(p, "NO", 2) && is_space(p[2]))
		return (1);
	if (!strncmp(p, "SO", 2) && is_space(p[2]))
		return (1);
	if (!strncmp(p, "WE", 2) && is_space(p[2]))
		return (1);
	if (!strncmp(p, "EA", 2) && is_space(p[2]))
		return (1);
	if (*p == 'F' && is_space(p[1]))
		return (1);
	if (*p == 'C' && is_space(p[1]))
		return (1);
	return (0);
}

// TODO need to fix norminette later
int parse_header_file(const char *filename, t_config *cfg, char **first_map_line)
{
	int fd;
	char *line;
	int ret;

	if (first_map_line)
		*first_map_line = NULL;
	init_config(cfg);
	if (check_cub_extension(filename))
		return (1);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (errmsg("cannot open .cub file"));
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		if (!cfg->header_done)
		{
			ret = dispatch_header_line(line, cfg);
			if (ret)
			{
				free(line);
				close(fd);
				return (1);
			}
			if (all_header_present(cfg))
				cfg->header_done = 1;
		}
		else if (!is_blank(line))
		{
			if (is_header_directive_line(line))
			{
				free(line);
				close(fd);
				return (errmsg("unexpected header directive after header complete"));
			}
			if (first_map_line)
				*first_map_line = strdup(line);
			free(line);
			break;
		}
		free(line);
	}
	close(fd);
	if (!cfg->header_done)
		return (errmsg("incomplete header (missing F/C or texture)"));
	return (0);
}
