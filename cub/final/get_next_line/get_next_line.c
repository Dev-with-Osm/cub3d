/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:00:00 by okhourss          #+#    #+#             */
/*   Updated: 2024/12/05 10:26:21 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*saved_data;
	char		*read_buffer;
	char		*line_to_return;
	size_t		size;

	line_to_return = NULL;
	size = (size_t)BUFFER_SIZE + 1;
	read_buffer = malloc(size);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_resources(&saved_data, &read_buffer));
	if (!read_buffer)
		return (NULL);
	saved_data = read_and_store(fd, saved_data, read_buffer);
	if (saved_data && saved_data[0] == '\0')
	{
		free(saved_data);
		saved_data = NULL;
		return (NULL);
	}
	line_to_return = extract_line_to_return(saved_data, line_to_return);
	saved_data = trim_saved_data(saved_data);
	return (line_to_return);
}

void	*free_resources(char **saved_data, char **read_buffer)
{
	free(*saved_data);
	free(*read_buffer);
	*saved_data = NULL;
	*read_buffer = NULL;
	return (NULL);
}

char	*read_and_store(int fd, char *saved_data, char *read_buffer)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	if (!saved_data)
		saved_data = ft_strdup("");
	while (bytes_read > 0)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(read_buffer);
			free(saved_data);
			return (NULL);
		}
		read_buffer[bytes_read] = '\0';
		saved_data = ft_strjoin(saved_data, read_buffer);
		if (ft_strchr(read_buffer, '\n') || !saved_data)
			break ;
	}
	free(read_buffer);
	return (saved_data);
}

char	*extract_line_to_return(char *saved_data, char *line_to_return)
{
	size_t	len;
	size_t	i;

	if (!saved_data)
		return (NULL);
	len = 0;
	while (saved_data[len] != '\n' && saved_data[len])
		len++;
	if (saved_data[len] == '\n')
		len++;
	line_to_return = malloc(len + 1);
	if (!line_to_return)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line_to_return[i] = saved_data[i];
		i++;
	}
	line_to_return[i] = '\0';
	return (line_to_return);
}

char	*trim_saved_data(char *saved_data)
{
	size_t	len;
	char	*trimmed_str;

	if (!saved_data)
		return (NULL);
	len = 0;
	while (saved_data[len] != '\n' && saved_data[len])
		len++;
	if (saved_data[len] == '\n')
		len++;
	if (!saved_data[len])
	{
		free(saved_data);
		return (NULL);
	}
	trimmed_str = malloc(ft_strlen(saved_data) - len + 1);
	if (!trimmed_str)
		return (NULL);
	ft_strcpy(trimmed_str, saved_data + len);
	free(saved_data);
	return (trimmed_str);
}
