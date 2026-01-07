#ifndef FT_NM_NODE_H
# define FT_NM_NODE_H

# include <elf.h>
# include "libft.h"
# include "ft_symbol.h"

typedef enum e_nm_error
{
  NM_OK,
  NM_ERR_NO_FILE,
  NM_ERR_PERMISSION,
  NM_ERR_FSTAT,
  NM_ERR_MMAP,
  NM_ERR_SECTION_HEADERS,
  NM_ERR_SYMTAB_SECTION,
  NM_ERR_STRTAB,
  NM_ERR_NOT_ELF,
  NM_ERR_MALLOC
}            t_nm_error;

typedef struct s_nm_node
{
  char *      filename;
  char *      file_content; // This is a pointer to the mapped file content (mmap)
  bool        is_64bit;
  bool        is_32bit;
  size_t      file_size;
  Elf64_Ehdr *elf64_header;
  Elf32_Ehdr *elf32_header;
  t_nm_error  error;
  t_list     *symbols; // Linked list of t_symbol*
}               t_nm_node;

t_nm_node *new_nm_node(char *filename);
void destroy_node(t_nm_node *node);

#endif
