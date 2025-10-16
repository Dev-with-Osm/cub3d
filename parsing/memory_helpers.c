/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:20:02 by okhourss          #+#    #+#             */
/*   Updated: 2025/10/16 12:20:45 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_lines_buf(char **buf, int n)
{
	int	i;

	if (!buf)
		return ;
	i = 0;
	while (i < n)
	{
		free(buf[i]);
		i++;
	}
	free(buf);
}

void	free_rows_n(char **rows, int h)
{
	int	i;

	if (!rows)
		return ;
	i = 0;
	while (i < h)
	{
		free(rows[i]);
		i++;
	}
	free(rows);
}
