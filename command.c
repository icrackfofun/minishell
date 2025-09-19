/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:07:55 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/15 16:08:30 by psantos-         ###   ########.fr       */
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
		builtin_export(cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		builtin_unset(cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		builtin_env(cmd, info);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		builtin_exit(cmd, info, root);
}

static void	exec_child(t_ast *cmd, t_info *info)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	path = get_path(info, cmd);
	env_list_to_array(info);
	if (cmd->redirs)
		handle_redirections(cmd->redirs);
	execve(path, cmd->argv, info->env_array);
	exit_exec_error(cmd->argv[0]);
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
		path = get_path(info, cmd);
		env_list_to_array(info);
		if (cmd->redirs)
			handle_redirections(cmd->redirs);
		execve(path, cmd->argv, info->env_array);
		exit_exec_error(cmd->argv[0]);
	}
	info->child_pids[info->child_count++] = pid;
}

void	exec_command(t_ast *cmd, t_info *info, int root)
{
	if (root && cmd->redirs)
		prepare_heredocs(cmd, info);
	if (cmd->is_builtin == 1)
		exec_builtin(cmd, info, root);
	else
		exec_external(cmd, info, root);
}
