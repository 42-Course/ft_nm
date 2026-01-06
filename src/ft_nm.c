#include "ft_nm.h"
#include "ft_nm_node.h"

static void	print_error(t_nm_node *node)
{
	ft_putstr_fd("ft_nm: '", 2);
	ft_putstr_fd(node->filename, 2);
	ft_putstr_fd("': ", 2);
	if (node->error == NM_ERR_NO_FILE)
		ft_putstr_fd("No such file\n", 2);
	else if (node->error == NM_ERR_PERMISSION)
		ft_putstr_fd("Permission denied\n", 2);
	else if (node->error == NM_ERR_FSTAT)
		ft_putstr_fd("fstat failed\n", 2);
	else if (node->error == NM_ERR_MMAP)
		ft_putstr_fd("mmap failed\n", 2);
	else if (node->error == NM_ERR_NOT_ELF)
		ft_putstr_fd("file format not recognized\n", 2);
}

static void	process_node(t_nm_node *node, t_nm_options *opts)
{
	if (!node)
		return ;
	if (node->error != NM_OK)
	{
		print_error(node);
		return ;
	}
	(void)opts;
	printf("Processing file: %s\n", node->filename);
}

void	nm(t_nm_options *opts)
{
	ft_lstiter_param(opts->files, (void (*)(void *, void *))process_node, opts);
}
