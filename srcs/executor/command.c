/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:07:55 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 22:00:32 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

#include <stdio.h>

void	debug_print_cmds(t_ast **cmds, int count)
{
	int i;
	t_redir *redir;

	if (!cmds)
	{
		printf("cmds array is NULL\n");
		return;
	}

	for (i = 0; i < count; i++)
	{
		if (!cmds[i])
		{
			printf("cmds[%d] is NULL\n", i);
			continue;
		}

		printf("cmds[%d]: argv[0] = '%s'\n", i, cmds[i]->argv ? cmds[i]->argv[0] : "(NULL argv)");
		redir = cmds[i]->redirs;
		while (redir)
		{
			printf("  redir fd = %d, type = %d, target = %s\n",
				redir->fd, redir->type, redir->target ? redir->target : "(NULL target)");
			redir = redir->next;
		}
	}
}

static void	exec_builtin(t_ast *cmd, t_info *info, int root)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		builtin_echo(cmd, root, info);
	else if (!ft_strcmp(cmd->argv[0], "cd"))
		builtin_cd(cmd, info, root);
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		builtin_pwd(info, root);
	else if (!ft_strcmp(cmd->argv[0], "export"))
		builtin_export(root, cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		builtin_unset(root, cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		builtin_env(root, cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		builtin_exit(cmd, info, root);
}

// static void exec_sh(char *path, t_info *info)
// {
//     size_t	len;
// 	char	*args[3];

//     len = ft_strlen(path);
//     if (len > 3 && !ft_strcmp(path + len - 3, ".sh"))
//     {
//         args[0] = "sh";
//         args[1] = path;
//         args[2] = NULL;
//         env_list_to_array(info);
//         execve("/bin/sh", args, info->env_array);
// 		exit_exec_error("", info, path);
//     }
// }

static void	exec_child(t_ast *cmd, t_info *info, int root)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->argv[0][0])
		child_exit("", 0, info, "");
	if (cmd->redirs)
		handle_redirections(cmd->redirs, info);
	if (!root)
		close_heredocs(info->cmds, info->cmd_count);
	path = get_path(info, cmd);
	if (path[0] && path[0] != '.' && path[0] != '/')
		child_exit("", 127, info, path);
	//exec_sh(path, info);
	env_list_to_array(info);
	execve(path, cmd->argv, info->env_array);
	exit_exec_error(cmd->argv[0], info, path);
}

static void	exec_external(t_ast *cmd, t_info *info, int root)
{
	pid_t	pid;

	if (!root)
		exec_child(cmd, info, root);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
		exec_child(cmd, info, root);
	info->child_pids[info->child_count++] = pid;
}

void	exec_command(t_ast *cmd, t_info *info, int root)
{
	if (root && cmd)
	{
		if (prepare_heredocs(&cmd, info, 1))
			return ;
	}
	if (!root)
	{
		info->last_pipe_read_fd = -1;
		info->last_pipe_write_fd = -1;
	}
	if (cmd->is_builtin == 1)
		exec_builtin(cmd, info, root);
	else
		exec_external(cmd, info, root);
}
