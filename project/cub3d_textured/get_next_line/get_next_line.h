

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

// main functions
char	*get_next_line(int fd);

// utils
t_list	*lastnode(t_list *lst);
int		ft_getn(t_list *lst);
int		lenton(t_list *lst);
void	copy_str(t_list *lst, char *buf);
void	ft_free(t_list **lst, char *buf, t_list *node);

#endif
