#include "ft_options.h"

void	init_options(t_nm_options *opts)
{
	opts->help = 0;
	opts->debug_syms = 0;
	opts->extern_only = 0;
	opts->undefined_only = 0;
	opts->reverse_sort = 0;
	opts->no_sort = 0;
	opts->files = NULL;
}

static int	parse_short_option(t_nm_options *opts, char opt)
{
	if (opt == 'h')
		opts->help = 1;
	else if (opt == 'a')
		opts->debug_syms = 1;
	else if (opt == 'g')
		opts->extern_only = 1;
	else if (opt == 'u')
		opts->undefined_only = 1;
	else if (opt == 'r')
		opts->reverse_sort = 1;
	else if (opt == 'p')
		opts->no_sort = 1;
	else
		return (0);
	return (1);
}

static int	parse_long_option(t_nm_options *opts, char *arg)
{
	if (ft_strcmp(arg, "--help") == 0)
		opts->help = 1;
	else if (ft_strcmp(arg, "--debug-syms") == 0)
		opts->debug_syms = 1;
	else if (ft_strcmp(arg, "--extern-only") == 0)
		opts->extern_only = 1;
	else if (ft_strcmp(arg, "--undefined-only") == 0)
		opts->undefined_only = 1;
	else if (ft_strcmp(arg, "--reverse-sort") == 0)
		opts->reverse_sort = 1;
	else if (ft_strcmp(arg, "--no-sort") == 0)
		opts->no_sort = 1;
	else
		return (0);
	return (1);
}

static int	handle_long_option_arg(t_nm_options *opts, char *arg)
{
	if (!parse_long_option(opts, arg))
	{
		ft_putstr_fd("ft_nm: invalid option '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

static int	handle_short_option_arg(t_nm_options *opts, char *arg)
{
	int	j;

	j = 1;
	while (arg[j])
	{
		if (!parse_short_option(opts, arg[j]))
		{
			ft_putstr_fd("ft_nm: invalid option -- '", 2);
			ft_putchar_fd(arg[j], 2);
			ft_putstr_fd("'\n", 2);
			return (0);
		}
		j++;
	}
	return (1);
}

static int	add_file_to_list(t_nm_options *opts, char *filename)
{
	t_list	*node;

	node = ft_lstnew(filename);
	if (!node)
		return (0);
	ft_lstadd_back(&opts->files, node);
	return (1);
}

static int	add_default_file(t_nm_options *opts)
{
	t_list	*node;

	if (opts->files == NULL)
	{
		node = ft_lstnew("a.out");
		if (!node)
			return (0);
		ft_lstadd_back(&opts->files, node);
	}
	return (1);
}

static int	process_argument(t_nm_options *opts, char *arg, int *parse_opts)
{
	if (*parse_opts && ft_strcmp(arg, "--") == 0)
	{
		*parse_opts = 0;
		return (1);
	}
	if (*parse_opts && arg[0] == '-' && arg[1] != '\0')
	{
		if (arg[1] == '-')
			return (handle_long_option_arg(opts, arg));
		else
			return (handle_short_option_arg(opts, arg));
	}
	return (add_file_to_list(opts, arg));
}

int	parse_arguments(t_nm_options *opts, int argc, char **argv)
{
	int	i;
	int	parse_opts;

	i = 1;
	parse_opts = 1;
	while (i < argc)
	{
		if (!process_argument(opts, argv[i], &parse_opts))
			return (0);
		i++;
	}
	return (add_default_file(opts));
}
