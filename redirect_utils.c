/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:36:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/27 17:36:43 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_redir(t_redir *redir, char *filename, t_info *info)
{
	free(redir->target);
	redir->target = ft_strdup(filename);
	if (!redir->target)
		parent_exit("malloc", info);
}

static char	*heredoc_filename(t_info *info, int *j)
{
	char	*filename;
	char	*count;

	count = ft_itoa(*j);
	if (!count)
		parent_exit("malloc", info);
	filename = ft_strjoin(".minishell_heredoc_", count);
	free(count);
	if (!filename)
		parent_exit("malloc", info);
	(*j)++;
	return (filename);
}

static void	write_heredoc_to_tmp(const char *delimiter, char *filename,
								t_info *info)
{
	int		fd;
	char	*expanded;

	while (1)
	{
		free(info->line);
		info->line = readline("> ");
		if (!info->line)
			return ;
		if (ft_strcmp(info->line, delimiter) == 0)
			return ;
		expanded = expand_inside_quotes(info, info->line);
		if (!expanded)
			child_exit("malloc", 1, info, "");
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			child_exit("heredoc", 1, info, "");
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
		close (fd);
	}
}

int	child_heredocs(t_redir *redir, int *j, t_info *info)
{
	pid_t	pid;
	int		status;

	info->heredoc_filename = heredoc_filename(info, j);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		write_heredoc_to_tmp(redir->target, info->heredoc_filename, info);
		child_exit("", 0, info, "");
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WTERMSIG(status) == 1)
			parent_exit("", info);
		if (WTERMSIG(status) == SIGINT)
			return (write(1, "\n", 1));
		add_redir(redir, info->heredoc_filename, info);
	}
	return (0);
}
