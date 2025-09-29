/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:07:55 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 16:55:16 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	exec_child(t_ast *cmd, t_info *info)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (!cmd->argv[0][0])
		child_exit("", 0, info, "");
	if (cmd->redirs)
		handle_redirections(cmd->redirs, info);
	path = get_path(info, cmd);
	if (path[0] && path[0] != '.' && path[0] != '/')
		child_exit("", 127, info, path);
	env_list_to_array(info);
	execve(path, cmd->argv, info->env_array);
	exit_exec_error(cmd->argv[0], info, path);
}

static void	exec_external(t_ast *cmd, t_info *info, int root)
{
	pid_t	pid;
	char	*path;

	if (!root)
		exec_child(cmd, info);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		if (!cmd->argv[0][0])
			child_exit("", 0, info, "");
		if (cmd->redirs)
			handle_redirections(cmd->redirs, info);
		path = get_path(info, cmd);
		if (path[0] && path[0] != '.' && path[0] != '/')
			child_exit("", 127, info, path);
		env_list_to_array(info);
		execve(path, cmd->argv, info->env_array);
		exit_exec_error(cmd->argv[0], info, path);
	}
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
