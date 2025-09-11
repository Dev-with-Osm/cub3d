/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:46:43 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/11 17:19:42 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

# include "../get_next_line/get_next_line.h"
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

typedef struct s_file
{
	char *filename;
	int	fd;
	
} t_file;

typedef enum e_phase
{
    PH_HEADER,
    PH_MAP
}   t_phase;

typedef enum e_tex_id
{
	TEX_NO,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	// F_RGB,
	// C_RGB
}	t_tex_id;

typedef struct s_texmap
{
	t_tex_id id;
	char *path;
}	t_texmap;

typedef struct s_config
{
	int fd;
	t_texmap *north;
	// t_texmap *north;
	// t_texmap *north;
	// t_texmap *north;
	// todo complete later
} t_config;


#define EXTENSION_ERR "Invalid file extension (expected .cub)" 
// #define PERMISION_ERR 1


#endif