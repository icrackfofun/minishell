/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:36:30 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/26 15:52:12 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_heredocs(t_ast **cmds, t_info *info, int count)
{
	int		i;
	t_redir	*redir;
	char	*filename;
	int		j;

	i = 0;
	j = 0;
	while (i < count)
	{
		redir = cmds[i]->redirs;
		while (redir)
		{
			filename = NULL;
			if (redir->type == REDIR_HEREDOC)
			{
				if (child_heredocs(redir, &j, &filename, info))
					return (free(filename), 1);
			}
			redir = redir->next;
			free (filename);
		}
		i++;
	}
	return (0);
}

static void	change_fd(t_redir *redir, int fd, t_info *info)
{
	if (fd < 0)
		child_exit(redir->target, 1, info, "");
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			child_exit("dup2", 1, info, "");
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			child_exit("dup2", 1, info, "");
		}
	}
}

void	handle_redirections(t_redir *redir, t_info *info)
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
		change_fd(redir, fd, info);
		close(fd);
		redir = redir->next;
	}
}
