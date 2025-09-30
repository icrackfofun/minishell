/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:36:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/30 13:53:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_terminal	*terminal(void)
{
	static t_terminal	t;

	return (&t);
}

static void	heredoc_filename(t_info *info, int *j, char *filename)
{
	char	*tmp;
	char	*count;

	count = ft_itoa(*j);
	if (!count)
		parent_exit("malloc", info);
	tmp = ft_strjoin(".minishell_heredoc_", count);
	free(count);
	if (!tmp)
		parent_exit("malloc", info);
	(*j)++;
	ft_strlcpy(filename, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

static void	add_redir(t_redir *redir, char *filename, t_info *info)
{
	free(redir->target);
	redir->target = ft_strdup(filename);
	if (!redir->target)
		parent_exit("malloc", info);
}

static int	write_heredoc_to_tmp(const char *delimiter, char *filename,
								t_info *info)
{
	int		fd;
	char	*expanded;

	while (1)
	{
		free(info->line);
		info->line = readline("> ");
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			exit(0);
		if (!info->line)
			return (close(fd));
		if (ft_strcmp(info->line, delimiter) == 0)
			return (close(fd));
		populate_env(terminal()->envp, info);
		expanded = expand_inside_quotes(info, info->line);
		free_env(info->env_list);
		if (!expanded)
			exit(1);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
		close (fd);
	}
	return (0);
}

static int	wait_heredoc_child(pid_t pid, t_redir *redir, t_info *info, char *f)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		return (1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		parent_exit("malloc", info);
	add_redir(redir, f, info);
	return (0);
}

int	child_heredocs(t_redir *redir, int *j, t_info *info)
{
	pid_t	pid;
	char	filename[60];
	char	delimiter[10000];

	heredoc_filename(info, j, filename);
	ft_strlcpy(delimiter, redir->target, ft_strlen(redir->target) + 1);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		clean_loop(info);
		write_heredoc_to_tmp(delimiter, filename, info);
		if (info->line)
			free(info->line);
		exit(0);
	}
	else if (pid > 0)
		return (wait_heredoc_child(pid, redir, info, filename));
	return (0);
}
