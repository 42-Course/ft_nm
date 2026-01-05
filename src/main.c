#include "ft_nm.h"

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

	init_options(&opts);
	if (!parse_arguments(&opts, argc, argv))
	{
		ft_putstr_fd("Usage: ft_nm [option(s)] [file(s)]\n", 2);
		ft_lstclear(&opts.files, NULL);
		return (1);
	}
	if (opts.help)
	{
		print_help();
		ft_lstclear(&opts.files, NULL);
		return (0);
	}
	if (opts.debug_syms)
		printf("Debug symbols enabled\n");
	if (opts.extern_only)
		printf("Extern-only enabled\n");
	if (opts.undefined_only)
		printf("Undefined-only enabled\n");
	if (opts.reverse_sort)
		printf("Reverse sort enabled\n");
	if (opts.no_sort)
		printf("No sort enabled\n");
	ft_lstclear(&opts.files, NULL);
	return (0);
}
