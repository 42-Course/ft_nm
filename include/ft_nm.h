#ifndef FT_NM_H
# define FT_NM_H

# include <fcntl.h> // open
# include <stdio.h> // perror, printf
# include <stdlib.h> // exit, EXIT_FAILURE
# include <sys/mman.h> // mmap, munmap
# include <sys/stat.h> // fstat
# include <unistd.h> // close
# include <elf.h> // elf headers
# include "libft.h"
# include "ft_options.h"

/**
open(2) close(2) mmap(2) munmap(2) write(2)
fstat(2) malloc(3) free(3) exit(3) perror(3)
strerror(1) getpagesize(2)
*/

void	nm(t_nm_options *opts, t_list *nm_nodes);

#endif
