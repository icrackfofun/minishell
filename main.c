/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/30 14:23:42 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

static void	info_init(t_info *info, char **envp)
{
	populate_env(envp, info);
	info->env_array = NULL;
	info->had_space = 0;
	info->last_status = 0;
	info->pipe_count = 0;
	info->tree = NULL;
	info->tokens = NULL;
	info->child_pids = NULL;
	info->child_count = 0;
	info->cmds = NULL;
	info->heredoc_filename = NULL;
	info->last_pipe_read_fd = -1;
	info->last_pipe_write_fd = -1;
	info->leftover_read_fd = -1;
	g_last_signal = 0;
	terminal()->envp = envp;
	terminal()->info = info;
	signal(SIGINT, prompt_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	((void)argc, (void)argv);
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
		expand_variables(&info);
		parsing(&info, info.pipe_count);
		executor(info.tree, &info);
		clean_loop(&info);
		cleanup_heredoc_files();
		g_last_signal = 0;
	}
	clean_shell(&info);
}
