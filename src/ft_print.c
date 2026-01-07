#include "ft_print.h"
#include "ft_symbol.h"
#include <stdio.h>
#include <elf.h>

static void	print_error(t_nm_node *node)
{
	static const char	*error_messages[] = {
		[NM_OK] = NULL,
		[NM_ERR_NO_FILE] = "No such file",
		[NM_ERR_PERMISSION] = "Permission denied",
		[NM_ERR_FSTAT] = "fstat failed",
		[NM_ERR_MMAP] = "mmap failed",
		[NM_ERR_SECTION_HEADERS] = "invalid section headers",
		[NM_ERR_SYMTAB_SECTION] = "no symbols",
		[NM_ERR_STRTAB] = "invalid string table",
		[NM_ERR_NOT_ELF] = "file format not recognized",
		[NM_ERR_MALLOC] = "malloc failed"
	};

	ft_putstr_fd("ft_nm: '", 2);
	ft_putstr_fd(node->filename, 2);
	ft_putstr_fd("': ", 2);
	if (node->error < sizeof(error_messages) / sizeof(error_messages[0])
		&& error_messages[node->error])
		ft_putstr_fd((char *)error_messages[node->error], 2);
	else
		ft_putstr_fd("unknown error", 2);
	ft_putstr_fd("\n", 2);
}

static void	print_symbol(t_symbol *symbol, t_nm_node *node)
{
	if (!symbol)
		return ;
	if (symbol->shndx == SHN_UNDEF)
	{
		if (node->is_64bit)
			printf("                 ");
		else
			printf("         ");
	}
	else
	{
		if (node->is_64bit)
			printf("%016lx ", symbol->value);
		else
			printf("%08lx ", (unsigned long)symbol->value);
	}
	printf("%c %s\n", symbol->type, symbol->name);
}

void	print_nm_symbols(t_nm_node *node, t_nm_options *opts)
{
	if (!node)
		return ;
	if (node->error != NM_OK)
	{
		print_error(node);
		return ;
	}
	if (!opts->single_file_mode)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(node->filename, 1);
		ft_putstr_fd(":\n", 1);
	}
	if (node->symbols)
		ft_lstiter_param(node->symbols, (void (*)(void *, void *))print_symbol,
			node);
}
