/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:51:41 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/04 15:26:33 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	**create_env_arr(t_env *env)
{
	t_env	**arr;
	int		size;
	int		i;

	size = env_count(env);
	arr = malloc(sizeof(t_env *) * (size + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

static void	sort_env_arr(t_env **arr)
{
	int		i;
	int		swapped;
	t_env	*tmp;

	if (!arr)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (arr[i + 1])
		{
			if (ft_strcmp(arr[i]->key, arr[i + 1]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

static void	print_env_list(int root, t_env *env, t_info *info)
{
	t_env	**tmp_arr;
	int		i;

	tmp_arr = create_env_arr(env);
	if (!tmp_arr)
	{
		if (!root)
			child_exit("export", 1, info, "");
		parent_exit("malloc", info);
	}
	sort_env_arr(tmp_arr);
	i = 0;
	while (tmp_arr[i])
	{
		printf("declare -x %s", tmp_arr[i]->key);
		if (tmp_arr[i]->value)
			printf("=\"%s\"", tmp_arr[i]->value);
		printf("\n");
		i++;
	}
	free(tmp_arr);
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
		set_env_value(env_list, key, NULL);
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
