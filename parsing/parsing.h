/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:40:11 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/18 11:40:11 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>

#include "../get_next_line/get_next_line.h"

typedef struct s_rgb
{
	int r;
	int g;
	int b;
} t_rgb;

typedef enum e_tex_id
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_MAX
} t_tex_id;

typedef struct s_config
{
	char *tex[TEX_MAX];
	int has_tex[TEX_MAX];
	int floor_set;
	int ceil_set;
	t_rgb floor_rgb;
	t_rgb ceil_rgb;
	int header_done;
} t_config;

int parse_tex_line(const char *s, char **out);
int parse_rgb_line(const char *s, t_rgb *out);

int dispatch_header_line(char *line, t_config *cfg);

int parse_header_file(const char *filename, t_config *cfg, char **first_map_line);

void free_config(t_config *cfg);

#endif