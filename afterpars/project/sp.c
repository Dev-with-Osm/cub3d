#include "cube.h"

static char	*ft_fill(const char *s, int *i, char c)
{
	char	*buff;
	int		o;
	int		count;
	int		y;

	while ((s[*i] == c) && s[*i])
		(*i)++;
	count = 0;
	y = *i;
	while ((s[*i] != c) && s[*i])
	{
		(*i)++;
		count++;
	}
	buff = malloc(count + 1);
	if (!buff)
		return (NULL);
	o = 0;
	while (s[y + o] && s[y + o] != c)
	{
		buff[o] = s[y + o];
		o++;
	}
	buff[o] = '\0';
	return (buff);
}

static int	ft_count(const char *s, char c)
{
	int	index;
	int	count;

	count = 0;
	index = 0;
	while (s[index])
	{
		while (s[index] == c)
			index++;
		if (s[index] != c && s[index])
		{
			count++;
			while (s[index] != c && s[index])
			{
				index++;
			}
		}
	}
	return (count);
}

static void	*ft_free(char **buff, int d)
{
	while (--d >= 0)
		free(buff[d]);
	free(buff);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		d;
	int		index;
	char	**buffer;

	if(s)
	{
		d = ft_count(s, c);
		buffer = malloc(sizeof(char *) * (d + 1));
		if (buffer == NULL)
			return (NULL);
		index = 0;
		d = 0;
		while (s[index])
		{
			while (s[index] == c)
				index++;
			if (s[index] && s[index] != c)
			{
				buffer[d] = ft_fill(s, &index, c);
				if (!buffer[d++])
					return (ft_free(buffer, d));
			}
		}
		buffer[d] = 0;
		return (buffer);
	}
	return (NULL);
}