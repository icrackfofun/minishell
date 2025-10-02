/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:36:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 14:11:25 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	add_redir(t_redir *redir, char *filename, t_info *info)
// {
// 	free(redir->target);
// 	redir->target = ft_strdup(filename);
// 	if (!redir->target)
// 		parent_exit("malloc", info);
// }

// static char	*heredoc_filename(t_info *info, int *j)
// {
// 	char	*filename;
// 	char	*count;

// 	count = ft_itoa(*j);
// 	if (!count)
// 		parent_exit("malloc", info);
// 	filename = ft_strjoin(".minishell_heredoc_", count);
// 	free(count);
// 	if (!filename)
// 		parent_exit("malloc", info);
// 	(*j)++;
// 	return (filename);
// }

// static int	write_heredoc_to_tmp(const char *delimiter, char *filename,
// 								t_info *info)
// {
// 	int		fd;
// 	char	*expanded;

// 	while (1)
// 	{
// 		free_string(&info->line);
// 		info->line = readline("> ");
// 		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd < 0)
// 			child_exit("heredoc", 1, info, "");
// 		if (!info->line)
// 			return (close(fd));
// 		if (ft_strcmp(info->line, delimiter) == 0)
// 			return (close(fd));
// 		expanded = expand_inside_quotes(info, info->line);
// 		if (!expanded)
// 			child_exit("malloc", 4, info, "");
// 		write(fd, expanded, ft_strlen(expanded));
// 		write(fd, "\n", 1);
// 		free(expanded);
// 		close (fd);
// 	}
// 	return (0);
// }

// int	child_heredocs(t_redir *redir, int *j, t_info *info)
// {
// 	pid_t	pid;
// 	int		status;

// 	info->heredoc_filename = heredoc_filename(info, j);
// 	pid = fork();
// 	if (pid < 0)
// 		parent_exit("fork", info);
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, child_sigint_handler);
// 		write_heredoc_to_tmp(redir->target, info->heredoc_filename, info);
// 		child_exit("", 0, info, "");
// 	}
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 	{
// 		if (WEXITSTATUS(status) == 4)
// 			parent_exit("", info);
// 		else if (WEXITSTATUS(status) == 2)
// 			return (write(1, "\n", 1));
// 	}
// 	add_redir(redir, info->heredoc_filename, info);
// 	return (0);
// }

static int wait_heredoc(pid_t pid, t_info *info)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 4)
			parent_exit("", info);
		else if (WEXITSTATUS(status) == 2)
			return (write(1, "\n", 1));
	}
	return (0);
}


static void write_heredoc_fd(const char *delimiter, int fd, t_info *info)
{
	char	*expanded;

	while (1)
	{
		free_string(&info->line);
		info->line = readline("> ");
		if (!info->line || ft_strcmp(info->line, delimiter) == 0)
			break ;
		expanded = expand_inside_quotes(info, info->line);
		if (!expanded)
			child_exit("malloc", 4, info, "");
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	if (close(fd) == 0)
		info->heredoc_fd = -1;
}

int child_heredocs(t_redir *redir, t_info *info)
{
	int		pipefd[2];
	pid_t	pid;
	int		ret;

	if (pipe(pipefd) < 0)
		parent_exit("pipe", info);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		close(pipefd[0]);
		info->heredoc_fd = pipefd[1];
		signal(SIGINT, child_sigint_handler);
		write_heredoc_fd(redir->target, pipefd[1], info);
		child_exit("", 0, info, "");
	}
	close(pipefd[1]);
	ret = wait_heredoc(pid, info);
	redir->fd = pipefd[0];
	return (ret);
}
