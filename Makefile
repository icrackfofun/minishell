# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/22 16:36:16 by jose-vda          #+#    #+#              #
#    Updated: 2025/10/01 16:28:00 by psantos-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = 	main.c  \
		clean.c \
		clean2.c \
		error.c \
		env.c \
		env_ops.c \
		executor.c \
		flatten.c \
		pipe.c \
       	command.c \
		path.c \
		redirect.c \
		redirect_utils.c \
		builtin1.c \
		builtin2.c \
		builtin_utils.c \
		signals.c \
		tokens.c \
		lexing.c \
	   	lexing_utils.c \
		lexing_utils2.c \
		expand.c \
		parsing.c \
		parsing_utils.c \
		parsing_utils2.c

OBJS = $(SRCS:.c=.o)

MAKE = make -C
LIBFT_PATH = libft
LIBFT = -L $(LIBFT_PATH) -lft

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    READLINE_DIR = $(shell brew --prefix readline)
    CFLAGS += -I$(READLINE_DIR)/include
    LDFLAGS = -L$(READLINE_DIR)/lib -lreadline -lncurses $(LIBFT)
else
    LDFLAGS = -lreadline $(LIBFT)
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) $(LIBFT_PATH) all
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) $(LIBFT_PATH) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

test: re
	@git clone https://github.com/LucasKuhn/minishell_tester
	@cd minishell_tester && (./tester || true) && cd ..
	@rm -rf minishell_tester

val:
	valgrind --show-leak-kinds=all --leak-check=full --track-fds=all --suppressions=readline.supp ./minishell

.PHONY: all clean fclean re

# {
#     leak readline
#     Memcheck:Leak
#     ...
#     fun:readline
# }
# {
#     leak add_history
#     Memcheck:Leak
#     ...
#     fun:add_history
# }
# {
#    leak rl_extend_line_buffer
#    Memcheck:Leak
#    ...
#    fun:readline_internal_char
# }

#