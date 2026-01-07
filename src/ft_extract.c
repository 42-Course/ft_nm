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

static char	*get_shstrtab(t_nm_node *node, void *section_headers)
{
	uint32_t	shstrndx;
	char		*shstrtab;

	shstrtab = NULL;
	if (node->is_64bit)
	{
		Elf64_Shdr *shdrs = (Elf64_Shdr *)section_headers;
		shstrndx = node->elf64_header->e_shstrndx;
		shstrtab = node->file_content + shdrs[shstrndx].sh_offset;
	}
	else
	{
		Elf32_Shdr *shdrs = (Elf32_Shdr *)section_headers;
		shstrndx = node->elf32_header->e_shstrndx;
		shstrtab = node->file_content + shdrs[shstrndx].sh_offset;
	}
	return (shstrtab);
}

static char	*get_symbol_name(char *strtab, char *shstrtab,
				void *section_headers, void *sym_entry, int is_64bit)
{
	uint32_t	st_name;
	uint32_t	sh_name;
	int			type;
	uint16_t	shndx;

	if (is_64bit)
	{
		Elf64_Sym *sym = (Elf64_Sym *)sym_entry;
		st_name = sym->st_name;
		type = ELF64_ST_TYPE(sym->st_info);
		shndx = sym->st_shndx;
		if (type == STT_SECTION && shndx < SHN_LORESERVE)
		{
			Elf64_Shdr *shdr = &((Elf64_Shdr *)section_headers)[shndx];
			sh_name = shdr->sh_name;
			return (shstrtab + sh_name);
		}
	}
	else
	{
		Elf32_Sym *sym = (Elf32_Sym *)sym_entry;
		st_name = sym->st_name;
		type = ELF32_ST_TYPE(sym->st_info);
		shndx = sym->st_shndx;
		if (type == STT_SECTION && shndx < SHN_LORESERVE)
		{
			Elf32_Shdr *shdr = &((Elf32_Shdr *)section_headers)[shndx];
			sh_name = shdr->sh_name;
			return (shstrtab + sh_name);
		}
	}
	return (strtab + st_name);
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
				char *strtab, void *section_headers, char *shstrtab)
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
			get_symbol_name(strtab, shstrtab, section_headers,
				&symtab[i], 1),
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
				char *strtab, void *section_headers, char *shstrtab)
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
			get_symbol_name(strtab, shstrtab, section_headers,
				&symtab[i], 0),
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
	char	*shstrtab;

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
	shstrtab = get_shstrtab(node, section_headers);
	if (node->is_64bit)
		node->symbols = extract_symbols_64(node, symtab_section, strtab,
				section_headers, shstrtab);
	else
		node->symbols = extract_symbols_32(node, symtab_section, strtab,
				section_headers, shstrtab);
}
