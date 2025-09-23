/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:50:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/23 15:03:51 by psantos-         ###   ########.fr       */
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

static void	handle_export_arg(t_env **env_list, char *arg)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq != NULL)
	{
		*eq = '\0';
		key = arg;
		value = eq + 1;
		set_env_value(env_list, key, value);
		*eq = '=';
	}
	else
	{
		key = arg;
		value = "";
		set_env_value(env_list, key, value);
	}
}

void	builtin_export(int root, t_ast *cmd, t_info *info)
{
	int	i;

	i = 1;
	if (cmd->argv[i] == NULL)
		return (print_env_list(root, info->env_list, info));
	while (cmd->argv[i] != NULL)
		handle_export_arg(&info->env_list, cmd->argv[i++]);
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

	if (!ast->argv[1])
		status = info->last_status;
	else if (ast->argv[2])
	{
		printf("exit\nbash: exit: too many arguments\n");
		if (!root)
			child_exit("", 1, info, "");
		info->last_status = 1;
		return ;
	}
	else
		status = ft_atoi(ast->argv[1]);
	if (!root)
		child_exit("", status, info, "");
	printf("exit\n");
	clean_loop(info);
	clean_shell(info);
	exit(status);
}
