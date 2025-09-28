/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:47:58 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 00:46:37 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_child(t_ast *cmd, t_info *info)
{
	int	newline;
	int	i;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	i = 1;
	newline = 1;
	if (cmd->argv[i] && ft_strcmp(cmd->argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	if (cmd->redirs)
		handle_redirections(cmd->redirs, info);
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	child_exit("", 0, info, "");
}

void	builtin_echo(t_ast *ast, int root, t_info *info)
{
	pid_t	pid;

	if (!root)
		echo_child(ast, info);
	pid = fork();
	if (pid < 0)
		parent_exit("fork", info);
	if (pid == 0)
		echo_child(ast, info);
	info->child_pids[info->child_count++] = pid;
}

void	builtin_cd(t_ast *ast, t_info *info, int root)
{
	char	*path;

	if (ast->argv[1] != NULL)
		path = ft_strdup(ast->argv[1]);
	else
		path = ft_strdup(get_env_value(info->env_list, "HOME"));
	if (!path)
	{
		write(2, "cd: HOME not set\n", 17);
		if (!root)
			child_exit("", 1, info, "");
		return ;
	}
	if (update_env(info, path, root))
		return ;
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}

void	builtin_pwd(t_info *info, int root)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (!root)
			child_exit("pwd", 1, info, "");
		return (parent_return("pwd", info, 1, ""));
	}
	printf("%s\n", cwd);
	free(cwd);
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}

void	builtin_unset(int root, t_ast *ast, t_info *info)
{
	int		i;
	char	*key;

	i = 1;
	while (ast->argv[i])
	{
		if (ft_is_valid(ast->argv[i], 0, 0))
		{
			key = ast->argv[i];
			unset_env(&info->env_list, key);
		}
		else
		{
			info->last_status = 1;
			write(2, "unset: `", 9);
			write(2, ast->argv[i], ft_strlen(ast->argv[i]));
			write(2, "\': not a valid identifier\n", 26);
		}
		i++;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
