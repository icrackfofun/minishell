/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/23 22:41:35 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

#include <stdio.h>

static const char *node_type_str(t_node_type type)
{
    if (type == NODE_COMMAND)
        return "COMMAND";
    if (type == NODE_PIPE)
        return "PIPE";
    return "UNKNOWN";
}

static const char *redir_type_str(t_redir_type type)
{
    if (type == REDIR_INPUT)
        return "INPUT";
    if (type == REDIR_OUTPUT)
        return "OUTPUT";
    if (type == REDIR_APPEND)
        return "APPEND";
    if (type == REDIR_HEREDOC)
        return "HEREDOC";
    return "UNKNOWN";
}

static void print_redirs(t_redir *redir, int depth)
{
    while (redir)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("redir %s -> '%s'\n",
               redir_type_str(redir->type),
               redir->target ? redir->target : "(null)");
        redir = redir->next;
    }
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("Node type: %s\n", node_type_str(node->type));

    if (node->type == NODE_COMMAND && node->argv)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("argv:");
        for (int i = 0; node->argv[i]; i++)
            printf(" %s", node->argv[i]);
        printf("\n");

        if (node->is_builtin)
        {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("builtin: yes\n");
        }
    }

    if (node->redirs)
        print_redirs(node->redirs, depth + 1);

    if (node->left)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("LEFT:\n");
        print_ast(node->left, depth + 1);
    }

    if (node->right)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("RIGHT:\n");
        print_ast(node->right, depth + 1);
    }
}


static void	info_init(t_info *info, char **envp)
{
	if (!envp | !*envp | !**envp)
	{
		printf("No environment variables found");
		exit(1);
	}
	info->env_list = env_init(envp);
	if (!info->env_list)
		parent_exit("malloc", info);
	info->env_array = NULL;
	info->had_space = 0;
	info->last_status = 0;
	info->pipe_count = 0;
	info->tree = NULL;
	info->tokens = NULL;
	info->child_pids = NULL;
	info->child_count = 0;
	info->cmds = NULL;
	info->last_pipe_read_fd = -1;
	info->last_pipe_write_fd = -1;
	info->leftover_read_fd = -1;
	g_last_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	(void)argc;
	(void)argv;
	info_init(&info, envp);
	while (1)
	{
		info.line = readline("minishell$ ");
		if (!info.line)
		{
			printf("exit\n");
			break ;
		}
		if (*info.line)
			add_history(info.line);
		info.tokens = lexing(&info);
		printf("%p\n", info.tokens);
		expand_variables(&info, info.tokens);
		parsing(&info, info.pipe_count);
		printf("%p\n", info.tree);
		print_ast(info.tree, 0);
		if (info.tree)
			executor(info.tree, &info);
		clean_loop(&info);
	}
	clean_shell(&info);
}
