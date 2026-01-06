#include "ft_nm_node.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

static void init_node_fields(t_nm_node *node, char *filename)
{
  node->filename = ft_strdup(filename);
  node->file_content = NULL;
  node->is_64bit = false;
  node->is_32bit = false;
  node->file_size = 0;
  node->elf64_header = NULL;
  node->elf32_header = NULL;
  node->error = NM_OK;
}

static t_nm_error map_file(t_nm_node *node)
{
  int fd;
  struct stat sb;

  fd = open(node->filename, O_RDONLY);
  if (fd < 0)
  {
    if (errno == EACCES)
      return (NM_ERR_PERMISSION);
    return (NM_ERR_NO_FILE);
  }
  if (fstat(fd, &sb) == -1)
  {
    close(fd);
    return (NM_ERR_FSTAT);
  }
  node->file_size = sb.st_size;
  node->file_content = mmap(NULL, node->file_size, PROT_READ,
                             MAP_PRIVATE, fd, 0);
  close(fd);
  if (node->file_content == MAP_FAILED)
  {
    node->file_content = NULL;
    return (NM_ERR_MMAP);
  }
  return (NM_OK);
}

static t_nm_error determine_format(t_nm_node *node)
{
  unsigned char *ident;

  if (node->file_size < sizeof(Elf64_Ehdr))
    return (NM_ERR_NOT_ELF);
  ident = (unsigned char *)node->file_content;
  if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 ||
      ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3)
    return (NM_ERR_NOT_ELF);
  if (ident[EI_CLASS] == ELFCLASS64)
  {
    node->is_64bit = true;
    node->elf64_header = (Elf64_Ehdr *)node->file_content;
  }
  else if (ident[EI_CLASS] == ELFCLASS32)
  {
    node->is_32bit = true;
    node->elf32_header = (Elf32_Ehdr *)node->file_content;
  }
  else
    return (NM_ERR_NOT_ELF);
  return (NM_OK);
}

t_nm_node *new_nm_node(char *filename)
{
  t_nm_node *node;

  node = (t_nm_node *)malloc(sizeof(t_nm_node));
  if (!node)
    return (NULL);
  init_node_fields(node, filename);
  node->error = map_file(node);
  if (node->error != NM_OK)
    return (node);
  node->error = determine_format(node);
  return (node);
}

void destroy_node(t_nm_node *node) {
  if (node) {
    if (node->filename)
      free(node->filename);
    if (node->file_content && node->file_size > 0)
      munmap(node->file_content, node->file_size);
    free(node);
  }
}