NAME = ft_nm

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
IFLAGS = -I./include -I./lib/Libft/includes

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

LIBFT_DIR = lib/Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = $(SRC_DIR)/main.c \
	$(SRC_DIR)/ft_options.c \
	$(SRC_DIR)/ft_nm.c \
	$(SRC_DIR)/ft_nm_node.c \
	$(SRC_DIR)/ft_symbol.c \
	$(SRC_DIR)/ft_extract.c \
	$(SRC_DIR)/ft_filter.c \
	$(SRC_DIR)/ft_sort.c \
	$(SRC_DIR)/ft_print.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

test: $(NAME)
	cp $(NAME) tests/ft_nm
	cd tests && bash tester.sh

re: fclean all

.PHONY: all clean fclean re
