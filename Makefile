# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/22 16:36:16 by jose-vda          #+#    #+#              #
#    Updated: 2025/10/06 14:21:47 by psantos-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC_DIR = srcs
OBJ_DIR = obj

SRCS =  shell/main.c \
        shell/clean.c \
        shell/clean2.c \
        shell/error.c \
		shell/error_utils.c \
        shell/signals.c \
        env/env_init.c \
		env/env_utils.c \
        env/env_ops.c \
        lexing/lexing.c \
        lexing/lexing_utils.c \
        lexing/lexing_utils2.c \
        lexing/lexing_utils3.c \
        parsing/parse.c \
        parsing/parse_utils.c \
        expand/expand.c \
        expand/expand_utils.c \
		expand/lex_expanded.c \
        pipe/pipe.c \
        pipe/flatten.c \
        redirect/redirect.c \
        redirect/heredoc.c \
        executor/executor.c \
        executor/command.c \
        executor/path.c \
        builtin/echo.c \
        builtin/cd.c \
        builtin/env.c \
        builtin/exit.c \
        builtin/export.c \
        builtin/pwd.c \
        builtin/unset.c

SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)

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

# valgrind --show-leak-kinds=all --leak-check=full --track-fds=all --suppressions=readline.supp ./minishell