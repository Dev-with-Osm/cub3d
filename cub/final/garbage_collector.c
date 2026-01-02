/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:08:29 by hoel-mos          #+#    #+#             */
/*   Updated: 2026/01/02 20:49:19 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

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

void	ft_lstadd_f(t_addr_lst **lst, t_addr_lst *ne)
{
	if (!lst || !ne)
		return ;
	ne->next = *lst;
	*lst = ne;
}

void	ft_lstc(t_addr_lst **lst)
{
	t_addr_lst	*tmp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->data)
			free((*lst)->data);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	*w_malloc(size_t size)
{
	void		*ptr;
	t_addr_lst	**lst;
	t_addr_lst	*new_node;

	lst = get_garbage_collecter();
	ptr = malloc(size);
	if (ptr == NULL)
	{
		ft_lstc(lst);
		perror("Error: allocation failed");
		exit(EXIT_FAILURE);
	}
	new_node = ft_lstn(ptr);
	if (new_node == NULL)
	{
		free(ptr);
		ft_lstc(lst);
		perror("Error: GC node creation failed");
		exit(EXIT_FAILURE);
	}
	ft_lstadd_f(lst, new_node);
	return (ptr);
}
