/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:57:31 by okhourss          #+#    #+#             */
/*   Updated: 2024/12/04 16:07:47 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *s)
{
	char	*copy;

	if (!s)
		return (NULL);
	copy = malloc(ft_strlen(s) + 1);
	if (!copy)
		return (NULL);
	ft_strcpy(copy, s);
	return (copy);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	j;
	char	*new_str;

	if (!s1 || !s2)
	{
		free(s1);
		return (NULL);
	}
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	ft_strcpy(new_str, s1);
	j = 0;
	while (s2[j])
	{
		new_str[ft_strlen(s1) + j] = s2[j];
		j++;
	}
	new_str[ft_strlen(s1) + j] = '\0';
	free(s1);
	return (new_str);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return ((char *)s + i);
		i++;
	}
	if (c == '\0')
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
