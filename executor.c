/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-vda <jose-vda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:56:53 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/22 18:11:07 by jose-vda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_last_child_status(int status, t_info *info)
{
	int	sig;

	if (WIFEXITED(status))
		info->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		info->last_status = 128 + sig;
		if (sig == SIGINT)
			printf("\n");
		else if (sig == SIGQUIT)
			printf("Quit: %d\n", sig);
	}
	else
		info->last_status = 1;
}

void	reap_children(t_info *info, int i)
{
	int		status;
	pid_t	ret;

	if (info->child_count <= 0)
		return ;
	ret = waitpid(info->child_pids[i], &status, 0);
	if (ret > 0 && info->child_count == 1)
		handle_last_child_status(status, info);
	i++;
	info->child_count--;
	reap_children(info, i);
}

void	executor(t_ast *node, t_info *info)
{
	int	count;

	//enable_ctrl_echo();
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (node->type == NODE_COMMAND)
	{
		info->child_pids = malloc(sizeof(pid_t));
		if (!info->child_pids)
			parent_exit("malloc", info);
		exec_command(node, info, 1);
	}
	else if (node->type == NODE_PIPE)
	{
		info->cmds = flatten_pipeline(node, &count);
		info->child_pids = malloc(sizeof(pid_t) * (count));
		if (!info->child_pids || !info->cmds)
			parent_exit("malloc", info);
		exec_pipeline(info->cmds, count, info, -1);
	}
	reap_children(info, 0);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	//disable_ctrl_echo();
}
