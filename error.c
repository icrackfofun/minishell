/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:47:46 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/23 15:18:03 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_exec_error(const char *cmd, t_info *info)
{
	int	code;

	if (errno == ENOENT || errno == EACCES || errno == EISDIR)
		write(2, cmd, ft_strlen(cmd));
	if (errno == ENOENT)
	{
		code = 127;
		write(2, ": command not found\n", 20);
	}
	else if (errno == EACCES)
	{
		code = 126;
		write(2, ": Permission denied\n", 20);
	}
	else if (errno == EISDIR)
	{
		code = 126;
		write(2, ": is a directory\n", 17);
	}
	else
		code = 1;
	clean_loop(info);
	clean_shell(info);
	exit (code);
}

void	kill_all_children(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->child_count)
	{
		if (kill(info->child_pids[i], 0) == 0 || errno == EPERM)
			kill(info->child_pids[i], SIGTERM);
		i++;
	}
}

void	child_exit(char *message, int code, t_info *info, char *file)
{
	if (message && file[0] == 0)
		perror(message);
	else if (message && file[0] != 0)
	{
		write(2, message, ft_strlen(message));
		write(2, ": ", 2);
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
		perror("");
	}
	clean_loop(info);
	clean_shell(info);
	exit(code);
}

void	parent_exit(char *message, t_info *info)
{
	perror(message);
	kill_all_children(info);
	reap_children(info, 0);
	clean_loop(info);
	clean_shell(info);
	exit(1);
}

void	parent_return(char *message, t_info *info, int status, char *f)
{
	if (message && f[0] == 0)
		perror(message);
	else if (message && f[0] != 0)
	{
		write(2, message, ft_strlen(message));
		write(2, ": ", 2);
		write(2, f, ft_strlen(f));
		write(2, ": ", 2);
		perror("");
	}
	info->last_status = status;
}
