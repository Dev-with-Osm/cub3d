

#include "get_next_line.h"


int	ft_getn(t_list *lst)
{
	int	i;

	if (lst == NULL)
		return (0);
	while (lst)
	{
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i] == '\n')
				return (1);
			i++;
		}
		lst = lst->next;
	}
	return (0);
}

t_list	*lastnode(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

int	lenton(t_list *lst)
{
	int	len;
	int	i;

	len = 0;
	while (lst)
	{
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i] == '\n')
			{
				len++;
				return (len);
			}
			i++;
			len++;
		}
		lst = lst->next;
	}
	return (len);
}

void	copy_str(t_list *lst, char *buf)
{
	int	i;
	int	u;

	u = 0;
	while (lst)
	{
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i] == '\n')
			{
				buf[u++] = '\n';
				buf[u] = '\0';
				return ;
			}
			buf[u++] = lst->content[i++];
		}
		lst = lst->next;
	}
	buf[u] = '\0';
}

void	ft_free(t_list **lst, char *buf, t_list *node)
{
	t_list	*temp;

	if (*lst == NULL)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
	if (node != NULL && node->content != NULL && node->content[0] != '\0')
	{
		*lst = node;
	}
	else
	{
		free(buf);
		free(node);
	}
}
