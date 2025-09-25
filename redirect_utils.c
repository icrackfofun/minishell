/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:36:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 23:57:15 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_filename(t_info *info, int *j)
{
	char	*filename;
	char	*count;

	count = ft_itoa(*j);
	if (!count)
		child_exit("malloc", 1, info, "");
	filename = ft_strjoin(".minishell_heredoc_", count);
	free(count);
	if (!filename)
		child_exit("malloc", 1, info, "");
	(*j)++;
	return (filename);
}

static void	write_heredoc_to_tmp(const char *delimiter, char *filename,
								t_info *info)
{
	int		fd;
	char	*line;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		child_exit("heredoc", 1, info, "");
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close (fd);
	free (filename);
}

int	child_heredocs(t_redir *redir, int *j, char *filename, t_info *info)
{
	pid_t	pid;
	int		status;

	filename = heredoc_filename(info, j);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		write_heredoc_to_tmp(redir->target, filename, info);
		child_exit("", 0, info, "");
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WTERMSIG(status) == 1)
			parent_exit("", info);
		if (WTERMSIG(status) == SIGINT)
			return (write(1, "\n", 1));
		free(redir->target);
		redir->target = filename;
	}
	return (0);
}
