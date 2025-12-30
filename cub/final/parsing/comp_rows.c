/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:01:39 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/16 12:18:57 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	drop_leading_blanks(char ***buf, int n, int *start)
{
	int			i;
	const char	*s;

	i = 0;
	while (i < n)
	{
		s = (*buf)[i];
		p_skip_spaces(&s);
		if (*s)
			break ;
		free((*buf)[i]);
		(*buf)[i] = NULL;
		i++;
	}
	if (i == n)
	{
		free(*buf);
		return (p_err("empty map"));
	}
	*start = i;
	return (0);
}

static int	find_block_end_and_check_tail(char ***buf, int n, int i, int *j_out)
{
	int			j;
	const char	*s;

	j = i;
	while (j < n)
	{
		s = (*buf)[j];
		p_skip_spaces(&s);
		if (!*s)
			break ;
		j++;
	}
	while (j < n)
	{
		s = (*buf)[j++];
		p_skip_spaces(&s);
		if (*s)
			return (p_err("something wrong inside map"));
	}
	*j_out = j;
	return (0);
}

static int	count_nonblank_from(char ***buf, int n, int i)
{
	int			j;
	int			c;
	const char	*s;

	j = i;
	c = 0;
	while (j < n)
	{
		s = (*buf)[j++];
		p_skip_spaces(&s);
		if (*s)
			c++;
	}
	return (c);
}

static void	free_lines_buf_local(char **buf, int n)
{
	int	k;

	if (!buf)
		return ;
	k = 0;
	while (k < n)
	{
		free(buf[k]);
		k++;
	}
	free(buf);
}

int	comp_map(char ***map, int *h, char ***buf, int n)
{
	const char	*s;

	int (i), (j), (c), (k);
	if (drop_leading_blanks(buf, n, &i))
		return (1);
	if (find_block_end_and_check_tail(buf, n, i, &j))
		return (free_lines_buf_local(*buf, n), 1);
	c = count_nonblank_from(buf, n, i);
	*map = (char **)w_malloc(sizeof(char *) * c);
	if (!*map)
		return (p_err("malloc failed"));
	*h = c;
	k = 0;
	while (i < n)
	{
		s = (*buf)[i];
		p_skip_spaces(&s);
		if (*s)
			(*map)[k++] = (*buf)[i];
		else
			free((*buf)[i]);
		i++;
	}
	free(*buf);
	return (0);
}
