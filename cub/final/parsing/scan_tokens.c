/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:48:02 by okhourss          #+#    #+#             */
/*   Updated: 2025/12/30 10:13:22 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	p_dup_token(const char *beg, size_t len, char **out)
{
	char	*dst;

	dst = (char *)malloc(len + 1);
	if (!dst)
		return (p_err("malloc failed (token)"));
	ft_memcpy(dst, beg, len);
	dst[len] = '\0';
	*out = dst;
	return (0);
}

int	p_take_token(const char **p, const char **beg, size_t *len)
{
	const char	*s;

	p_skip_spaces(p);
	s = *p;
	while (*s && !p_is_space(*s) && *s != '\n')
		s++;
	if (s == *p)
		return (p_err("missing texture path"));
	*beg = *p;
	*len = (size_t)(s - *p);
	*p = s;
	return (0);
}

int	p_expect_eol(const char **p)
{
	p_skip_spaces(p);
	if (**p == '\0' || **p == '\n')
		return (0);
	return (p_err("trailing garbage"));
}

int	p_read_uint_255(const char **p, int *out)
{
	int	v;

	v = 0;
	p_skip_spaces(p);
	if (!ft_isdigit((unsigned char)**p))
		return (p_err("expected number"));
	while (ft_isdigit((unsigned char)**p))
	{
		if (v > 25 || (v == 25 && **p - '0' > 5))
			return (p_err("color out of range (0..255)"));
		v = v * 10 + (**p - '0');
		(*p)++;
	}
	*out = v;
	return (0);
}

int	p_expect_char(const char **p, char c)
{
	p_skip_spaces(p);
	if (**p != c)
		return (p_err("expected separator"));
	(*p)++;
	return (0);
}
