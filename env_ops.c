/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:48:19 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 09:38:53 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	populate_env(char **envp, t_info *info)
{
	char	*cwd;

	if (!envp || !*envp || !**envp)
	{
		set_env_value(&info->env_list, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		set_env_value(&info->env_list, "SHLVL", "1");
		set_env_value(&info->env_list, "OLDPWD", "");
		cwd = getcwd(NULL, 0);
		if (!cwd)
			return (set_env_value(&info->env_list, "PWD", "/"));
		set_env_value(&info->env_list, "PWD", cwd);
		free(cwd);
	}
	else
	{
		info->env_list = env_init(envp);
		if (!info->env_list)
			parent_exit("malloc", info);
	}
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	node = *env_list;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			free(node->value);
			node->value = ft_strdup(value);
			return ;
		}
		node = node->next;
	}
	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = *env_list;
	*env_list = node;
}

void	unset_env(t_env **env_list, const char *key)
{
	t_env	*prev;
	t_env	*node;

	prev = NULL;
	node = *env_list;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				*env_list = node->next;
			free(node->key);
			free(node->value);
			free(node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}
