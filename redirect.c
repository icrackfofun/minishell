/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:36:30 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/15 16:03:32 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_filename(t_info *info)
{
	char	*filename;
	char	*count;

	count = ft_itoa(info->child_count);
	if (!count)
		parent_exit("malloc", info);
	filename = ft_strjoin(".minishell_heredoc_", count);
	if (!filename)
		parent_exit("malloc", info);
	free(count);
	return (filename);
}

static int	write_heredoc_to_tmp(const char *delimiter, const char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (close(fd), 1);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

void	prepare_heredocs(t_ast *cmd, t_info *info)
{
	t_redir	*redir;
	char	*filename;
	int		ret;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			filename = heredoc_filename(info);
			ret = write_heredoc_to_tmp(redir->target, filename);
			if (ret == -1)
			{
				free(filename);
				parent_exit("heredoc", info);
			}
			free(redir->target);
			redir->target = filename;
		}
		redir = redir->next;
	}
}

static void	change_fd(t_redir *redir, int fd)
{
	if (fd < 0)
		child_exit(redir->target, 1);
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			child_exit("dup2", 1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			child_exit("dup2", 1);
		}
	}
}

void	handle_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			fd = open(redir->target, O_RDONLY);
		else if (redir->type == REDIR_OUTPUT)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_HEREDOC)
			fd = open(redir->target, O_RDONLY);
		change_fd(redir, fd);
		close(fd);
		redir = redir->next;
	}
}
