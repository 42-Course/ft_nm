#include "ft_symbol.h"
#include <stdlib.h>

t_symbol	*new_symbol(char *name, Elf64_Addr value, unsigned char info,
				unsigned char other, uint16_t shndx, char type)
{
	t_symbol	*symbol;

	symbol = (t_symbol *)malloc(sizeof(t_symbol));
	if (!symbol)
		return (NULL);
	symbol->name = name;
	symbol->value = value;
	symbol->info = info;
	symbol->other = other;
	symbol->shndx = shndx;
	symbol->type = type;
	return (symbol);
}

void	destroy_symbol(void *symbol)
{
	if (symbol)
		free(symbol);
}
