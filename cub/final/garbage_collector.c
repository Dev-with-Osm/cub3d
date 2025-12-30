/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-mos <hoel-mos@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 18:08:29 by hoel-mos          #+#    #+#             */
/*   Updated: 2025/12/30 18:09:44 by hoel-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h" 

// Returns the address of the static list pointer (the list head)
t_addr_lst	**get_garbage_collecter(void)
{
	static t_addr_lst	*lst;

	return (&lst);
}

// Creates a new list node and tracks the content pointer
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

// O(1) Add Front: Adds a new node to the start of the list
void	ft_lstadd_f(t_addr_lst **lst, t_addr_lst *ne)
{
	if (!lst || !ne)
		return ;
	ne->next = *lst;
	*lst = ne;
}

// Cleans up all tracked memory. IMPORTANT: Does not call exit.
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

// Malloc wrapper that tracks the pointer and handles critical failure
void	*w_malloc(size_t size)
{
	void		*ptr;
	t_addr_lst	**lst;
	t_addr_lst	*new_node;

	lst = get_garbage_collecter();
	ptr = malloc(size);
	if (ptr == NULL)
	{
		ft_lstc(lst); // 1. Cleanup all previously allocated memory
		perror("Error: w_malloc failed");
		exit(EXIT_FAILURE); // 2. Exit cleanly
	}
	new_node = ft_lstn(ptr);
	if (new_node == NULL)
	{
		free(ptr); // Free the memory allocated just now
		ft_lstc(lst); // 3. Cleanup everything
		perror("Error: GC node creation failed");
		exit(EXIT_FAILURE);
	}
	// O(1) addition
	ft_lstadd_f(lst, new_node);
	return (ptr);
}
