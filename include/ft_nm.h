#ifndef FT_NM_H
# define FT_NM_H

# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include "ft_options.h"

/**
open(2) close(2) mmap(2) munmap(2) write(2)
fstat(2) malloc(3) free(3) exit(3) perror(3)
strerror(1) getpagesize(2)
*/

void nm(t_nm_options *opts);

#endif
