/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:17:37 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/24 21:52:16 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_fd(int pipefd[2], t_info *info)
{
	if (pipe(pipefd) < 0)
		parent_exit("pipe", info);
	return (0);
}

static pid_t	fork_command_stage(t_ast *cmd, t_info *info,
									int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		if (input_fd != -1)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != -1)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		if (info->last_pipe_read_fd != -1)
			close(info->last_pipe_read_fd);
		if (info->last_pipe_write_fd != -1)
			close(info->last_pipe_write_fd);
		exec_command(cmd, info, 0);
	}
	return (pid);
}

static void	update_parent_fds(int *input_fd, int pipefd[2], int i, int count)
{
	if (*input_fd != -1)
		close(*input_fd);
	if (i < count - 1)
	{
		close(pipefd[1]);
		*input_fd = pipefd[0];
	}
}

static void	update_struct_fds(t_info *info, int *input_fd)
{
	if (*input_fd != -1)
		info->leftover_read_fd = *input_fd;
	info->last_pipe_read_fd = -1;
	info->last_pipe_write_fd = -1;
}

void	exec_pipeline(t_ast **cmds, int count, t_info *info, int input_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		write_fd;

	i = 0;
	if (cmds)
		prepare_heredocs(cmds, info, count);
	while (i < count)
	{
		write_fd = -1;
		if (i < count - 1)
		{
			create_pipe_fd(pipefd, info);
			write_fd = pipefd[1];
			info->last_pipe_read_fd = pipefd[0];
			info->last_pipe_write_fd = pipefd[1];
		}
		pid = fork_command_stage(cmds[i], info, input_fd, write_fd);
		info->child_pids[info->child_count++] = pid;
		update_parent_fds(&input_fd, pipefd, i++, count);
		update_struct_fds(info, &input_fd);
	}
}
