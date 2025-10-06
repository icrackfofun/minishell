/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:47:46 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/06 01:21:18 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static void	handle_enoent(const char *cmd, int *code)
// {
// 	if (cmd[0])
// 		write(2, cmd, ft_strlen(cmd));
// 	if (ft_strchr (cmd, '/'))
// 		write(2, ": No such file or directory\n", 28);
// 	else if (cmd[0])
// 		write(2, ": command not found\n", 20);
// 	if (cmd[0])
// 		*code = 127;
// 	else
// 		code = 0;
// }

// void	exit_exec_error(const char *cmd, t_info *info, char *path)
// {
// 	struct stat	st;
// 	int			code;

// 	if (path)
// 		free(path);
// 	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
// 	{
// 		write(2, cmd, ft_strlen(cmd));
// 		write(2, ": Is a directory\n", 17);
// 		code = 126;
// 	}
// 	else if (errno == ENOENT)
// 		handle_enoent(cmd, &code);
// 	else if (errno == EACCES)
// 	{
// 		write(2, cmd, ft_strlen(cmd));
// 		write(2, ": Permission denied\n", 20);
// 		code = 126;
// 	}
// 	else
// 		code = 1;
// 	clean_loop(info);
// 	clean_shell(info);
// 	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
// 	exit(code);
// }

// void	child_exit(char *message, int code, t_info *info, char *file)
// {
// 	if (message[0] != 0 && file[0] == 0 && ft_strncmp(message, "exit", 4) != 0)
// 		perror(message);
// 	else if (message[0] != 0 && file[0] != 0)
// 	{
// 		write(2, message, ft_strlen(message));
// 		write(2, ": ", 2);
// 		write(2, file, ft_strlen(file));
// 		write(2, ": ", 2);
// 		if (ft_strcmp(message, "exit") == 0)
// 			write(2, "numeric argument required\n", 26);
// 		else
// 			perror("");
// 	}
// 	else if (message[0] == 0 && file[0] != 0)
// 	{
// 		write(2, file, ft_strlen(file));
// 		write(2, ": ", 2);
// 		write(2, "command not found\n", 18);
// 	}
// 	if (file && file[0] != 0)
// 		free(file);
// 	clean_loop(info);
// 	clean_shell(info);
// 	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
// 	exit(code);
// }

// void	parent_return(char *message, t_info *info, int status, char *f)
// {
// 	if (message[0] != 0 && f[0] == 0)
// 		perror(message);
// 	else if (message[0] != 0 && f[0] != 0)
// 	{
// 		write(2, message, ft_strlen(message));
// 		write(2, ": ", 2);
// 		write(2, f, ft_strlen(f));
// 		write(2, ": ", 2);
// 		if (ft_strcmp(message, "export") == 0)
// 			write(2, "not a valid identifier\n", 23);
// 		else
// 			perror("");
// 	}
// 	if (f && f[0] != 0)
// 		free(f);
// 	info->last_status = status;
// }

static void	handle_enoent(char *cmd, int *code, t_info *info, int *printed)
{
	if (cmd[0])
	{
		if (ft_strchr(cmd, '/'))
			*printed = print_error(cmd, ": No such file or directory\n",
					NULL, NULL);
		else
			*printed = print_error(cmd, ": command not found\n", NULL, NULL);
		*code = 127;
	}
	else
		*code = 0;
}

void	exit_exec_error(char *cmd, t_info *info, char *path)
{
	struct stat	st;
	int			code;
	int			printed;

	free(path);
	printed = 0;
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		printed = print_error(cmd, ": Is a directory\n", NULL, NULL);
		code = 126;
	}
	else if (errno == ENOENT)
		handle_enoent(cmd, &code, info, &printed);
	else if (errno == EACCES)
	{
		printed = print_error(cmd, ": Permission denied\n", NULL, NULL);
		code = 126;
	}
	else
		code = 1;
	if (printed)
		child_exit("malloc", 1, info, "");
	(clean_loop(info), clean_shell(info));
	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
	exit(code);
}

void	child_exit(char *message, int code, t_info *info, char *file)
{
	int	printed;

	printed = 0;
	if (message[0] != 0 && file[0] == 0 && ft_strncmp(message, "exit", 4) != 0)
		perror(message);
	else if (message[0] != 0 && file[0] != 0)
	{
		if (ft_strcmp(message, "exit") == 0)
			printed = print_error(message, ": ", file,
					": numeric argument required\n");
		else
		{
			printed = print_error(message, ": ", file, ": ");
			perror("");
		}
	}
	else if (message[0] == 0 && file[0] != 0)
		printed = print_error(file, ": command not found\n", NULL, NULL);
	if (file && file[0] != 0)
		free(file);
	if (printed)
		child_exit("malloc", 1, info, "");
	(clean_loop(info), clean_shell(info));
	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
	exit(code);
}

void	parent_exit(char *message, t_info *info)
{
	perror(message);
	kill_all_children(info);
	reap_children(info, 0);
	clean_loop(info);
	clean_shell(info);
	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
	exit(1);
}

void	parent_return(char *message, t_info *info, int status, char *f)
{
	int	printed;

	printed = 0;
	if (message[0] != 0 && f[0] == 0)
		perror(message);
	else if (message[0] != 0 && f[0] != 0)
	{
		if (ft_strcmp(message, "export") == 0)
			printed = print_error(message, ": ", f,
					": not a valid identifier\n");
		else
		{
			printed = print_error(message, ": ", f, ": ");
			perror("");
		}
	}
	if (f && f[0] != 0)
		free(f);
	if (printed)
		child_exit("malloc", 1, info, "");
	info->last_status = status;
}
