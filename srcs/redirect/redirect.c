/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:36:30 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 23:18:48 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		{
			fd = redir->fd;
			redir->fd = -1;
		}
		change_fd(redir, fd, info);
		close(fd);
		redir = redir->next;
	}
}
