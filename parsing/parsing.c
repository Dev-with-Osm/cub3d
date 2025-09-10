/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:12:34 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/10 15:06:59 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int check_file_perm(char *file_name)
{
	// int fd = open(file_name, )
}

int ft_parsing(char *file_name)
{
	check_file_perm(file_name);
	return 0;
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
		return 1;
	ft_parsing(argv[1]);
	int i = 0;
	while (argv[i])
	{
		printf("%s\n", argv[i]);
		i++;
	}
	
	// int fd = open()
	return 0;
}

