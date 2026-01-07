#include "ft_nm.h"
#include "ft_nm_node.h"

static void	print_help(void)
{
	ft_putstr_fd("Usage: ft_nm [option(s)] [file(s)]\n", 1);
	ft_putstr_fd(" Display information about symbols in [file(s)].\n", 1);
	ft_putstr_fd(" Options:\n", 1);
	ft_putstr_fd("  -a, --debug-syms       Display debugger-only symbols\n", 1);
	ft_putstr_fd("  -g, --extern-only      Display only external symbols\n", 1);
	ft_putstr_fd("  -u, --undefined-only   Display only undefined symbols\n",
		1);
	ft_putstr_fd("  -r, --reverse-sort     Reverse the sense of the sort\n", 1);
	ft_putstr_fd("  -p, --no-sort          Do not sort the symbols\n", 1);
	ft_putstr_fd("  -h, --help             Display this information\n", 1);
}

int	main(int argc, char **argv)
{
	t_nm_options	opts;
	t_list				*nm_nodes;

	nm_nodes = NULL;
	init_options(&opts);
	if (!parse_arguments(&opts, argc, argv, &nm_nodes))
	{
		ft_putstr_fd("Usage: ft_nm [option(s)] [file(s)]\n", 2);
		ft_lstclear(&nm_nodes, (void (*)(void *))destroy_node);
		return (1);
	}
	if (opts.help)
	{
		print_help();
		ft_lstclear(&nm_nodes, (void (*)(void *))destroy_node);
		return (0);
	}
	nm(&opts, nm_nodes);
	ft_lstclear(&nm_nodes, (void (*)(void *))destroy_node);
	return (0);
}
