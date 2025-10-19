
#include "cube.h"

t_addr_lst	**get_garbage_collecter(void)
{
	static t_addr_lst	*lst;

	return (&lst);
}

t_addr_lst	*ft_lstn(void *content)
{
	t_addr_lst	*node;

	node = (t_addr_lst *)malloc(sizeof(t_addr_lst));
	if (!node)
	{
		return (NULL);
	}
	node->data = content;
	node->next = NULL;
	return (node);
}

void	ft_lstadd_b(t_addr_lst **lst, t_addr_lst *ne)
{
	t_addr_lst	*temp;

	if (!lst)
		return ;
	if (*lst == NULL)
	{
		*lst = ne;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = ne;
}

void	ft_lstc(t_addr_lst **lst)
{
	t_addr_lst	*tmp;

	if (!lst || !(*lst))
		return ;
	while ((*lst))
	{
		tmp = (*lst)->next;
		free((*lst)->data);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
	exit(-1);
}

void	*w_malloc(size_t size)
{
	void		*ptr;
	t_addr_lst	**lst;

	lst = get_garbage_collecter();
	ptr = malloc(size);
	if (ptr == NULL)
	{
		ft_lstc(lst);
		perror("Error\n");
		exit(1);
	}
	ft_lstadd_b(lst, ft_lstn(ptr));
	return (ptr);
}
