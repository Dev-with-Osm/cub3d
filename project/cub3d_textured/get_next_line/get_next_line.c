

#include "get_next_line.h"

static void	ft_prepare(t_list **lst)
{
	t_list	*node;
	t_list	*last_node;
	char	*buf;
	int		u;
	int		i;

	if (*lst == NULL)
		return ;
	buf = malloc(((size_t)BUFFER_SIZE) + 1);
	node = malloc(sizeof(t_list));
	if (buf == NULL || node == NULL)
		return ;
	last_node = lastnode(*lst);
	i = 0;
	while (last_node->content[i] && last_node->content[i] != '\n')
		i++;
	u = 0;
	while (last_node->content[i] && last_node->content[i++])
		buf[u++] = last_node->content[i];
	buf[u] = '\0';
	node->content = buf;
	node->next = NULL;
	ft_free(lst, buf, node);
}

static char	*get_line(t_list *lst)
{
	int		len;
	char	*str;

	if (lst == NULL)
		return (NULL);
	len = lenton(lst);
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	copy_str(lst, str);
	return (str);
}

static void	ft_link(t_list **lst, char *buf)
{
	t_list	*last_node;
	t_list	*new_node;

	last_node = lastnode(*lst);
	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	if (last_node == NULL)
		*lst = new_node;
	else
		last_node->next = new_node;
	new_node->content = buf;
	new_node->next = NULL;
}

static void	ft_creat(t_list **lst, int fd)
{
	char	*buf;
	int		index;

	while (!ft_getn(*lst))
	{
		buf = malloc(((size_t)BUFFER_SIZE) + 1);
		if (buf == NULL)
			return ;
		index = read(fd, buf, (BUFFER_SIZE));
		if (!index)
		{
			free(buf);
			return ;
		}
		buf[index] = '\0';
		ft_link(lst, buf);
	}
}

char	*get_next_line(int fd)
{
	static t_list	*lst;
	char			*buf;

	if ((fd < 0) || BUFFER_SIZE <= 0 || (read(fd, &buf, 0) < 0))
		return (NULL);
	ft_creat(&lst, fd);
	if (lst == NULL)
		return (NULL);
	buf = get_line(lst);
	ft_prepare(&lst);
	return (buf);
}
