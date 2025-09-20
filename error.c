/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:47:46 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/21 00:16:21 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_exec_error(const char *cmd)
{
	int	code;

	if (errno == ENOENT)
	{
		code = 127;
		printf("bash: %s: command not found\n", cmd);
	}
	else if (errno == EACCES)
	{
		code = 126;
		printf("bash: %s: Permission denied\n", cmd);
	}
	else if (errno == EISDIR)
	{
		code = 126;
		printf("bash: %s: is a directory\n", cmd);
	}
	else
		code = 1;
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

void	child_exit(char *message, int code)
{
	perror(message);
	exit(code);
}

void	parent_exit(char *message, t_info *info)
{
	perror(message);
	kill_all_children(info);
	reap_children(info, 0);
	clean_shell(info);
	exit(1);
}

void	parent_return(char *message, t_info *info, int status)
{
	info->last_status = status;
	if (message[0] != 0)
		perror(message);
}
