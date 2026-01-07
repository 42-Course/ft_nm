#include "ft_extract.h"
#include "ft_symbol.h"
#include <elf.h>

static uint16_t	get_section_count(t_nm_node *node)
{
	if (node->is_64bit)
		return (node->elf64_header->e_shnum);
	else
		return (node->elf32_header->e_shnum);
}

static void	*get_section_headers(t_nm_node *node)
{
	void	*section_headers;

	if (node->is_64bit)
		section_headers = node->file_content + node->elf64_header->e_shoff;
	else
		section_headers = node->file_content + node->elf32_header->e_shoff;
	if (!section_headers)
		node->error = NM_ERR_SECTION_HEADERS;
	return (section_headers);
}

static void	*find_symtab_section(t_nm_node *node, void *section_headers)
{
	uint16_t	i;
	uint16_t	shnum;

	shnum = get_section_count(node);
	if (node->is_64bit)
	{
		Elf64_Shdr *shdrs = (Elf64_Shdr *)section_headers;
		for (i = 0; i < shnum; i++)
			if (shdrs[i].sh_type == SHT_SYMTAB)
				return (&shdrs[i]);
	}
	else
	{
		Elf32_Shdr *shdrs = (Elf32_Shdr *)section_headers;
		for (i = 0; i < shnum; i++)
			if (shdrs[i].sh_type == SHT_SYMTAB)
				return (&shdrs[i]);
	}
	node->error = NM_ERR_SYMTAB_SECTION;
	return (NULL);
}

static char	*get_strtab(t_nm_node *node, void *section_headers,
				void *symtab_section)
{
	uint32_t	link;
	char		*strtab;

	strtab = NULL;
	if (node->is_64bit)
	{
		Elf64_Shdr *shdrs = (Elf64_Shdr *)section_headers;
		Elf64_Shdr *symtab = (Elf64_Shdr *)symtab_section;
		link = symtab->sh_link;
		strtab = node->file_content + shdrs[link].sh_offset;
	}
	else
	{
		Elf32_Shdr *shdrs = (Elf32_Shdr *)section_headers;
		Elf32_Shdr *symtab = (Elf32_Shdr *)symtab_section;
		link = symtab->sh_link;
		strtab = node->file_content + shdrs[link].sh_offset;
	}
	if (!strtab)
		node->error = NM_ERR_STRTAB;
	return (strtab);
}

static char	get_symbol_type_from_section(t_symbol *symbol, void *section_headers,
				t_nm_node *node)
{
	if (node->is_64bit)
	{
		Elf64_Shdr	*shdr = &((Elf64_Shdr *)section_headers)[symbol->shndx];
		if (shdr->sh_type == SHT_NOBITS)
			return ('B');
		if (shdr->sh_flags & SHF_EXECINSTR)
			return ('T');
		if (shdr->sh_flags & SHF_WRITE)
			return ('D');
		if (shdr->sh_flags & SHF_ALLOC)
			return ('R');
	}
	else
	{
		Elf32_Shdr	*shdr = &((Elf32_Shdr *)section_headers)[symbol->shndx];
		if (shdr->sh_type == SHT_NOBITS)
			return ('B');
		if (shdr->sh_flags & SHF_EXECINSTR)
			return ('T');
		if (shdr->sh_flags & SHF_WRITE)
			return ('D');
		if (shdr->sh_flags & SHF_ALLOC)
			return ('R');
	}
	return ('?');
}

static char	get_symbol_type(t_symbol *symbol, void *section_headers,
				t_nm_node *node)
{
	char	c;
	int		bind;
	int		type;

	bind = node->is_64bit ? ELF64_ST_BIND(symbol->info)
		: ELF32_ST_BIND(symbol->info);
	type = node->is_64bit ? ELF64_ST_TYPE(symbol->info)
		: ELF32_ST_TYPE(symbol->info);
	if (symbol->shndx == SHN_UNDEF)
	{
		if (bind == STB_WEAK)
		{
			if (type == STT_OBJECT)
				return ('v');
			return ('w');
		}
		return ('U');
	}
	if (symbol->shndx == SHN_ABS)
		c = 'A';
	else if (symbol->shndx == SHN_COMMON)
		c = 'C';
	else
		c = get_symbol_type_from_section(symbol, section_headers, node);
	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)
			return ('V');
		return ('W');
	}
	if (bind == STB_LOCAL && c != 'U')
		c = c + ('a' - 'A');
	return (c);
}

static t_list	*extract_symbols_64(t_nm_node *node, void *symtab_section,
				char *strtab, void *section_headers)
{
	Elf64_Shdr	*symtab_shdr;
	Elf64_Sym	*symtab;
	t_list		*symbols;
	t_symbol	*symbol;
	t_symbol	temp_sym;
	size_t		i;
	size_t		num_symbols;

	symbols = NULL;
	symtab_shdr = (Elf64_Shdr *)symtab_section;
	symtab = (Elf64_Sym *)(node->file_content + symtab_shdr->sh_offset);
	num_symbols = symtab_shdr->sh_size / sizeof(Elf64_Sym);
	for (i = 0; i < num_symbols; i++)
	{
		temp_sym.info = symtab[i].st_info;
		temp_sym.shndx = symtab[i].st_shndx;
		symbol = new_symbol(
			strtab + symtab[i].st_name,
			symtab[i].st_value,
			symtab[i].st_info,
			symtab[i].st_other,
			symtab[i].st_shndx,
			get_symbol_type(&temp_sym, section_headers, node)
		);
		if (symbol)
			ft_lstadd_back(&symbols, ft_lstnew(symbol));
	}
	return (symbols);
}

static t_list	*extract_symbols_32(t_nm_node *node, void *symtab_section,
				char *strtab, void *section_headers)
{
	Elf32_Shdr	*symtab_shdr;
	Elf32_Sym	*symtab;
	t_list		*symbols;
	t_symbol	*symbol;
	t_symbol	temp_sym;
	size_t		i;
	size_t		num_symbols;

	symbols = NULL;
	symtab_shdr = (Elf32_Shdr *)symtab_section;
	symtab = (Elf32_Sym *)(node->file_content + symtab_shdr->sh_offset);
	num_symbols = symtab_shdr->sh_size / sizeof(Elf32_Sym);
	for (i = 0; i < num_symbols; i++)
	{
		temp_sym.info = symtab[i].st_info;
		temp_sym.shndx = symtab[i].st_shndx;
		symbol = new_symbol(
			strtab + symtab[i].st_name,
			symtab[i].st_value,
			symtab[i].st_info,
			symtab[i].st_other,
			symtab[i].st_shndx,
			get_symbol_type(&temp_sym, section_headers, node)
		);
		if (symbol)
			ft_lstadd_back(&symbols, ft_lstnew(symbol));
	}
	return (symbols);
}

void	extract_nm_symbols(t_nm_node *node)
{
	void	*section_headers;
	void	*symtab_section;
	char	*strtab;

	if (!node || node->error != NM_OK)
		return ;
	section_headers = get_section_headers(node);
	if (node->error != NM_OK)
		return ;
	symtab_section = find_symtab_section(node, section_headers);
	if (node->error != NM_OK)
		return ;
	strtab = get_strtab(node, section_headers, symtab_section);
	if (node->error != NM_OK)
		return ;
	if (node->is_64bit)
		node->symbols = extract_symbols_64(node, symtab_section, strtab,
				section_headers);
	else
		node->symbols = extract_symbols_32(node, symtab_section, strtab,
				section_headers);
}
