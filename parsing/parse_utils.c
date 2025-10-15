/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:12:45 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/13 11:12:45 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	p_err(const char *msg)
{
	write(2, "Error\n", 6);
	if (msg)
		write(2, msg, (int)strlen(msg));
	write(2, "\n", 1);
	return (1);
}

int	p_is_space(char c)
{
	return (c == ' ' || c == '\t');
}

void	p_skip_spaces(const char **p)
{
	while (**p && p_is_space(**p))
		(*p)++;
}

int	p_is_blank(const char *s)
{
	while (*s && p_is_space(*s))
		s++;
	return (*s == '\0' || *s == '\n');
}

int	p_check_cub_ext(const char *filename)
{
	size_t	n;

	n = strlen(filename);
	if (n < 4 || strcmp(filename + (n - 4), ".cub"))
		return (p_err("invalid file extension (expected .cub)"));
	return (0);
}
