/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:47:46 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/24 23:32:34 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exit_exec_error(const char *cmd, t_info *info)
{
	struct stat	st;
	int			code;

	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		write(2, cmd, strlen(cmd));
		write(2, ": is a directory\n", 17);
		code = 126;
	}
	else if (errno == ENOENT)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": command not found\n", 20);
		code = 127;
	}
	else if (errno == EACCES)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": Permission denied\n", 20);
		code = 126;
	}
	else
		code = 1;
	clean_loop(info);
	clean_shell(info);
	exit(code);
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
	if (message[0] != 0 && file[0] == 0 && ft_strncmp(message, "exit", 4) != 0)
		perror(message);
	else if (message[0] != 0 && file[0] != 0)
	{
		write(2, message, ft_strlen(message));
		write(2, ": ", 2);
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
		if (ft_strcmp(message, "export") == 0)
			write(2, "not a valid identifier", 22);
		if (ft_strcmp(message, "exit\nexit") == 0)
			write(2, "numeric argument required\n", 26);
		else
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
	if (message[0] != 0 && f[0] == 0)
		perror(message);
	else if (message[0] != 0 && f[0] != 0)
	{
		write(2, message, ft_strlen(message));
		write(2, ": ", 2);
		write(2, f, ft_strlen(f));
		write(2, ": ", 2);
		if (ft_strcmp(message, "export") == 0)
			write(2, "not a valid identifier\n", 23);
		else
			perror("");
	}
	info->last_status = status;
}
