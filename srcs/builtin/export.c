/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:51:41 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 18:36:03 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//list has to be in order
//var has no ""(NULL), var= has ""(0)
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
		if (!ft_is_valid(arg, eq[0]))
			return (1);
		*eq = '\0';
		key = arg;
		value = eq + 1;
		set_env_value(env_list, key, value);
		*eq = '=';
	}
	else
	{
		if (!ft_is_valid(arg, 0))
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
			write(2, "export: `", 9);
			write(2, cmd->argv[i], ft_strlen(cmd->argv[i]));
			write(2, "\': not a valid identifier\n", 26);
			if (!root)
				child_exit("", 1, info, "");
			return (parent_return("", info, 1, ""));
		}
		i++;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
