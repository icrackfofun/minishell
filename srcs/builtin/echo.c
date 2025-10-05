/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:50:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/05 11:46:02 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_n(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (0);
	}
	return (1);
}

static void	echo_child(t_ast *cmd, t_info *info, int root)
{
	//int	newline;
	int	i;

	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
	if (cmd->redirs)
		handle_redirections(cmd->redirs, info);
	if (!root)
		close_heredocs(info->tree);
	i = 1;
	//newline = 1;
	while (cmd->argv[i] && is_n(cmd->argv[i]))
	{
		//newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		if (cmd->argv[1 + i++])
			printf(" ");
	}
	// if (newline)
	// 	printf("\n");
	child_exit("", 0, info, "");
}

void	builtin_echo(t_ast *ast, int root, t_info *info)
{
	pid_t	pid;

	if (!root)
		echo_child(ast, info, root);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
		echo_child(ast, info, root);
	info->child_pids[info->child_count++] = pid;
}
