#ifndef FT_OPTIONS_H
# define FT_OPTIONS_H

# include "libft.h"

typedef struct s_nm_options
{
	bool		help;
	bool		debug_syms;
	bool		extern_only;
	bool		undefined_only;
	bool		reverse_sort;
	bool		no_sort;
	bool		single_file_mode;
}	t_nm_options;

void	init_options(t_nm_options *opts);
int		parse_arguments(t_nm_options *opts, int argc,
			char **argv, t_list **nm_nodes);

#endif
