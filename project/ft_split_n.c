#include "cube.h"

static size_t	ft_strlenv2(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

static char	*ft_strdupv2(const char *s1, char c)
{
	char	*scpy;
	int		slen;
	int		i;

	slen = ft_strlenv2(s1, c);
	scpy = (char *) w_malloc ((slen + 1) * sizeof(char));
	i = 0;
	while (s1[i] && s1[i] != c)
	{
		scpy[i] = s1[i];
		i++;
	}
	scpy[i] = 0;
	return (scpy);
}

static void	done(char **str, const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] == c && c)
		i++;
	while (s[i])
	{
		str[j] = ft_strdupv2(&s[i], c);
		while (s[i] != c && s[i])
			i++;
		while (s[i] == c && c)
			i++;
		j++;
	}
	str[j] = 0;
}

char	**ft_split_n(char *s, char c)
{
	char	**str;
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (s == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	str = (char **) w_malloc ((count + 1) * sizeof(char *));
	done(str, s, c);
	return (str);
}
