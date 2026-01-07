#include "ft_sort.h"
#include "ft_symbol.h"

static int	compare_alnum_only(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 || *s2)
	{
		while (*s1 && !ft_isalnum(*s1))
			s1++;
		while (*s2 && !ft_isalnum(*s2))
			s2++;
		if (!*s1 && !*s2)
			return (0);
		if (!*s1)
			return (-1);
		if (!*s2)
			return (1);
		c1 = ft_tolower(*s1);
		c2 = ft_tolower(*s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	return (0);
}

static int	compare_symbols(void *a, void *b)
{
	t_symbol	*sym_a;
	t_symbol	*sym_b;
	int			result;

	sym_a = (t_symbol *)a;
	sym_b = (t_symbol *)b;
	result = compare_alnum_only(sym_a->name, sym_b->name);
	if (result == 0)
		result = ft_strcmp(sym_a->name, sym_b->name);
	return (result);
}

static int	compare_symbols_reverse(void *a, void *b)
{
	return (-compare_symbols(a, b));
}

void	sort_symbols(t_nm_node *node, t_nm_options *opts)
{
	if (!node || !node->symbols || opts->no_sort)
		return ;
	if (opts->reverse_sort)
		ft_lstsort(&node->symbols, compare_symbols_reverse);
	else
		ft_lstsort(&node->symbols, compare_symbols);
}
