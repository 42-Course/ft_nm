#include "ft_nm.h"
#include "ft_extract.h"
#include "ft_filter.h"
#include "ft_sort.h"
#include "ft_print.h"

void	nm(t_nm_options *opts, t_list *nm_nodes)
{
	ft_lstiter(nm_nodes, (void (*)(void *))extract_nm_symbols);
	ft_lstiter_param(nm_nodes, (void (*)(void *, void *))filter_symbols, opts);
	ft_lstiter_param(nm_nodes, (void (*)(void *, void *))sort_symbols, opts);
	ft_lstiter_param(nm_nodes, (void (*)(void *, void *))print_nm_symbols, opts);
}
