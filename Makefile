NAME = minishell

SRCS = ./
INCLUDE = ./

CC = gcc
CFLAGS = -Wall -Wextra -Werror 

FILES = main.c parse.c utils.c ft_split.c
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