NAME = minishell

SRCS = ./
INCLUDE = ./

CC = gcc
CFLAGS = -Wall -Wextra -Werror

FILES = main.c parsing.c instruct.c paths.c exec.c \
		builtins.c utils_1.c utils_2.c utils_3.c ft_split.c
OBJECTS = $(addprefix $(SRCS), $(FILES:.c=.o))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lreadline -I $(INCLUDE)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lreadline -I $(INCLUDE)

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all