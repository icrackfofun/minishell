CC = cc 
CFLAGS = -Wall -Werror -Wextra
NAME = libft.a

SRC = 	ft_strlcpy.c ft_strdup.c ft_strjoin.c ft_strlen.c

OBJ = $(SRC:.c=.o)

AR = ar rcs

.PHONY: all bonus fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all