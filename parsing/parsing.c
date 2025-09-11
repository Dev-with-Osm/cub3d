/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhourss <okhourss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:12:34 by okhourss          #+#    #+#             */
/*   Updated: 2025/09/11 18:25:45 by okhourss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_blank(const char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (s[i] == '\0' || s[i] == '\n');
}

int	is_map_line(const char *s)
{
	int i;
	int has_sym;

	if (!s)
		return (0);
	i = 0;
	has_sym = 0;
	while (s[i] && s[i] != '\n')
	{
		char c = s[i++];
		if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			has_sym = 1;
		else if (c == ' ' || c == '\t')
			;
		else
			return (0);
	}
	return (has_sym);
}


//! i will update it later
void	ft_exit(char *err_msg, int exit_flag)
{

	write(2, "Error\n", 6);
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
	if (exit_flag)
		exit(1);
	//TODO i need to add GC
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((i < n) && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

static int	ft_isvalid_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	return (ft_strncmp(&filename[len - 4], ".cub", 5) == 0);
}

int check_file_perm(char *file_name)
{
	if (!ft_isvalid_extension(file_name))
		ft_exit(EXTENSION_ERR, 1);
	int fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		ft_exit("Cannot open file", 1);
		return -1;
	}
	return (fd);
}

void parse_header_line(char *line){
	
	int i = 0;
	char *new_str = NULL;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			new_str = line + i;
			break;
		}
			i++;
	}
	
	printf("%s", new_str);
};


int ft_parsing(char *file_name)
{
    int     fd;
    char    *line;
    int     line_num;
    t_phase phase;

    fd = check_file_perm(file_name);
    if (fd == -1)
        return (1);
    phase = PH_HEADER;
    line_num = 1;
    while ((line = get_next_line(fd)))
    {
        if (phase == PH_HEADER)
        {
            if (is_blank(line)) { 
				free(line);
				line_num++;
				continue;
			}
            if (is_map_line(line))
                phase = PH_MAP;
            else
            {
                parse_header_line(line); 
                free(line);
                line_num++;
                continue;
            }
        }
		else{

		}
        // printf("%d- %s", line_num, line);
        // if (is_map_line(line))
        //     printf("%d - > is map\n", line_num);
        // else
        //     printf("%d - > not a map\n", line_num);
        free(line);
        line_num++;
    }
    close(fd);
    return (0);
}


int main(int argc, char *argv[])
{
	if (argc != 2)
		return 1;
	if (ft_parsing(argv[1]))
		return 1;

	return 0;
}

