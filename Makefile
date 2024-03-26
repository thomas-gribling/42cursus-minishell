NAME = minishell

SRCS = ./
INCLUDE = ./

CC = gcc
CFLAGS = -Wall -Wextra -Werror

FILES = main.c parsing.c instruct.c paths.c exec.c \
		builtins.c builtins_env_1.c builtins_env_2.c \
		utils_1.c utils_2.c utils_3.c ft_split.c ft_itoa.c
OBJECTS = $(addprefix $(SRCS), $(FILES:.c=.o))

define print_color
	@echo "\033[$1m$2\033[0m"
endef

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lreadline -I $(INCLUDE)
	@printf "\033[1;34mCompiling:\033[0m %-30s\033[K\r" $<

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lreadline -I $(INCLUDE)
	$(call print_color,32,"Executable created: $@")

clean:
	@rm -rf $(OBJECTS)
	$(call print_color,31,"Deleted object files")


fclean: clean
	@rm -rf $(NAME)
	$(call print_color,31,"Deleted executable")

re: fclean all