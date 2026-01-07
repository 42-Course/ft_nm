#include "ft_filter.h"
#include "ft_symbol.h"
#include <elf.h>

static int	should_display_symbol(void *content, void *param)
{
	t_symbol		*symbol;
	t_nm_options	*opts;
	int				bind;
	int				type;

	symbol = (t_symbol *)content;
	opts = (t_nm_options *)param;
	if (!symbol->name || !symbol->name[0])
	{
		if (symbol->shndx == SHN_UNDEF)
			return (0);
		if (!opts->debug_syms)
			return (0);
	}
	type = ELF64_ST_TYPE(symbol->info);
	if (!opts->debug_syms && (type == STT_FILE || type == STT_SECTION))
		return (0);
	if (opts->undefined_only && symbol->shndx != SHN_UNDEF)
		return (0);
	if (opts->extern_only)
	{
		bind = ELF64_ST_BIND(symbol->info);
		if (bind == STB_LOCAL)
			return (0);
	}
	return (1);
}

void	filter_symbols(t_nm_node *node, t_nm_options *opts)
{
	if (!node || !node->symbols)
		return ;
	ft_lstfilter_param(&node->symbols, should_display_symbol,
		destroy_symbol, opts);
}
