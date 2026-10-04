# Makefile 
NAME = minishell

CC = gcc 
CFLAGS = -Wall -Wextra -Werror -g -O2

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Regla principal
all: $(NAME)

# Crear el ejecutable
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

# Compilar cada .c a su correspondiente .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Eliminar objetos
clean:
	rm -rf $(OBJ_DIR)

# Eliminar objetos y ejecutable
fclean: clean
	rm -f $(NAME)

# Recompilar todo
re: fclean all

.PHONY: all clean fclean re