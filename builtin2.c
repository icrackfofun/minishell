/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:50:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/24 22:33:35 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_list(int root, t_env *env_list, t_info *info)
{
	t_env	*node;

	node = env_list;
	while (node)
	{
		printf("declare -x %s=\"", node->key);
		if (node->value != NULL)
			printf("%s", node->value);
		printf("\"\n");
		node = node->next;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}

static int	handle_export_arg(t_env **env_list, char *arg)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq != NULL)
	{
		if (!ft_is_valid(arg, 0, eq[0]))
			return (1);
		*eq = '\0';
		key = arg;
		value = eq + 1;
		set_env_value(env_list, key, value);
		*eq = '=';
	}
	else
	{
		if (!ft_is_valid(arg, 0, 0))
			return (1);
		key = arg;
		value = "";
		set_env_value(env_list, key, value);
	}
	return (0);
}

void	builtin_export(int root, t_ast *cmd, t_info *info)
{
	int	i;

	i = 1;
	if (cmd->argv[i] == NULL)
		return (print_env_list(root, info->env_list, info));
	while (cmd->argv[i] != NULL)
	{
		if (handle_export_arg(&info->env_list, cmd->argv[i]))
		{
			if (!root)
				child_exit("export", 1, info, cmd->argv[i]);
			info->last_status = 1;
			return (parent_return("export", info, 1, cmd->argv[i]));
		}
		i++;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}

void	builtin_env(int root, t_ast *ast, t_info *info)
{
	t_env	*node;

	(void)ast;
	node = info->env_list;
	while (node)
	{
		if (node->value && node->value[0] != '\0')
			printf("%s=%s\n", node->key, node->value);
		node = node->next;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}

void	builtin_exit(t_ast *ast, t_info *info, int root)
{
	int	status;

	status = 0;
	if (!ast->argv[1])
		status = info->last_status;
	else if (!ft_isnum(ast->argv[1]))
	{
		if (!root)
			child_exit("exit\nexit", 255, info, ast->argv[1]);
		child_exit("exit\nexit", 255, info, ast->argv[1]);
	}
	else if (ast->argv[2])
	{
		write(2, "exit: too many arguments", 24);
		if (!root)
			child_exit("", 1, info, "");
		return (parent_return("", info, 1, ""));
	}
	else
		status = ft_atoi(ast->argv[1]);
	if (!root)
		child_exit("", status, info, "");
	write(2, "exit\n", 5);
	child_exit("", status, info, "");
}
