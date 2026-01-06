#include "ft_nm.h"

#define BYTES_PER_LINE 16

static void handle_error(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

static void parse_file_content(char *file_content, size_t file_size)
{
  size_t i;
  size_t line;

  // Placeholder for parsing logic
  // In a real implementation, you would parse the file content here
  (void)file_content;
  (void)file_size;
  i = 0;
  line = 0;
  while (i < file_size)
  {
    printf("%08x  ", (unsigned int)(line * BYTES_PER_LINE)); // Print offset
    for (int j = 0; j < BYTES_PER_LINE; j++)
    {
      if (i + j < file_size)
        printf("%02x ", (unsigned char)file_content[i + j]); // Print byte in hex
      else
        printf("   ");
    }
    printf(" ");
    for (int j = 0; j < BYTES_PER_LINE; j++)
    {
      if (i + j < file_size)
      {
        char c = file_content[i + j];
        if (c >= 32 && c <= 126)
          printf("%c", c);
        else
          printf(".");
      }
    }
    printf("\n");
    i += BYTES_PER_LINE;
    line++;
  }
}

static int init_file(char *filename)
{
  int fd;
  struct stat  sb;
  char *file_content;
  size_t file_size;

  fd = open(filename, O_RDONLY);
  if (fd < 0)
    return (ft_putstr_fd("ft_nm: '", 2), ft_putstr_fd(filename, 2), 
            ft_putstr_fd("': No such file\n", 2), -1);
  if (fstat(fd, &sb) == -1)
    handle_error("fstat");

  file_size = sb.st_size;
  file_content = mmap(NULL, file_size, PROT_READ,
                       MAP_PRIVATE, fd, 0);
  if (file_content == MAP_FAILED)
    handle_error("mmap");
  parse_file_content(file_content, file_size);
  munmap(file_content, file_size);
  close(fd);
  return fd;
}

static void process_file(char *filename, t_nm_options *opts)
{
  int fd;

  if ((fd = init_file(filename) < 0))
  {
    return ;
  }
  (void) opts;
  // Placeholder for file processing logic
  printf("Processing file: %s\n", filename);
  // Here you would add the logic to open the file, read symbols, etc.
}

void nm(t_nm_options *opts)
{
  ft_lstiter_param(opts->files, (void (*)(void *, void *))process_file, opts);  
}