/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:36:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 22:28:43 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	wait_heredoc(pid_t pid, t_info *info)
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

static void	write_heredoc_fd(const char *delimiter, int fd, t_info *info)
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
	close(info->heredoc_in);
	close(info->heredoc_out);
	info->heredoc_in = -1;
	info->heredoc_out = -1;
}

int	child_heredocs(t_redir *redir, t_info *info)
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
		signal(SIGINT, child_sigint_handler);
		if (info->heredoc_in >= 0)
			close(info->heredoc_in);
		info->heredoc_out = pipefd[0];
		info->heredoc_in = pipefd[1];
		write_heredoc_fd(redir->target, pipefd[1], info);
		child_exit("", 0, info, "");
	}
	//close(pipefd[1]);
	info->heredoc_in = pipefd[0];
	ret = wait_heredoc(pid, info);
	redir->fd = pipefd[0];
	return (ret);
}

int	prepare_heredocs(t_ast **cmds, t_info *info, int count)
{
	int		i;
	t_redir	*redir;

	i = 0;
	while (i < count)
	{
		redir = cmds[i]->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (child_heredocs(redir, info))
				{
					info->last_status = 130;
					return (1);
				}
			}
			redir = redir->next;
		}
		i++;
	}
	info->heredoc_in = -1;
	return (0);
}
