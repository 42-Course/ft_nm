#ifndef FT_SYMBOL_H
# define FT_SYMBOL_H

# include <elf.h>
# include "libft.h"

typedef struct s_symbol
{
	char			*name;
	Elf64_Addr		value;
	unsigned char	info;
	unsigned char	other;
	uint16_t		shndx;
	char			type;
}	t_symbol;

t_symbol	*new_symbol(char *name, Elf64_Addr value, unsigned char info,
				unsigned char other, uint16_t shndx, char type);
void		destroy_symbol(void *symbol);

#endif
