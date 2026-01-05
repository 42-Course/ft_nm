#ifndef FT_OPTIONS_H
# define FT_OPTIONS_H

# include "libft.h"

typedef struct s_nm_options
{
	int		help;
	int		debug_syms;
	int		extern_only;
	int		undefined_only;
	int		reverse_sort;
	int		no_sort;
	t_list	*files;
}	t_nm_options;

void	init_options(t_nm_options *opts);
int		parse_arguments(t_nm_options *opts, int argc, char **argv);

#endif
