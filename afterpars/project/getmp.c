#include "cube.h"

static void	ft_add(const char *str, char *buff, int *b)
{
	int	i;

	i = 0;
	while (str[i])
	{
		buff[*b] = str[i];
		i++;
		(*b)++;
	}
}

static char	*ft_dup(char *s1)
{
	char	*str;

	int (l), (i);
	l = strlen(s1);
	str = (char *)w_malloc((l + 1) * sizeof(char));
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*ft_join(char *s1, char *s2)
{
	char		*buff;

	int (total_len), (b);
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_dup(s2));
	if (!s2)
		return (ft_dup(s1));
	total_len = strlen((char *)s1) + strlen((char *)s2);
	buff = (char *)w_malloc(total_len + 1);
	b = 0;
	ft_add(s1, buff, &b);
	ft_add(s2, buff, &b);
	buff[b] = '\0';
	return (buff);
}

char	*get_map(int fd)
{
	char *(map), *(line);
	map = NULL;
	line = get_next_line(fd);
	while (line)
	{
		map = ft_join(map, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (map);
}
