/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:23:42 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/02 18:36:20 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

static int	env_count(t_env *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

static char	*env_str(t_env *node, t_info *info)
{
	char	*str;
	size_t	len;

	len = ft_strlen(node->key) + ft_strlen(node->value) + 2;
	str = malloc(len);
	if (!str)
		child_exit("malloc", 1, info, "");
	ft_strlcpy(str, node->key, ft_strlen(node->key) + 1);
	str[ft_strlen(node->key)] = '=';
	ft_strlcpy(str + ft_strlen(node->key) + 1, node->value,
		ft_strlen(node->value) + 1);
	return (str);
}

void	env_list_to_array(t_info *info)
{
	t_env	*tmp;
	int		i;
	int		size;

	size = env_count(info->env_list);
	info->env_array = malloc(sizeof(char *) * (size + 1));
	if (!info->env_array)
		child_exit("malloc", 1, info, "");
	tmp = info->env_list;
	i = 0;
	while (tmp)
	{
		info->env_array[i++] = env_str(tmp, info);
		tmp = tmp->next;
	}
	info->env_array[i] = NULL;
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;
	char	*cwd;
	char	*eq;

	head = NULL;
	i = 0;
	while (envp && *envp && **envp && envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		node = malloc(sizeof(t_env));
		if (!node)
			return (free_env(head));
		node->key = ft_strndup(envp[i], eq - envp[i]);
		node->value = ft_strdup(eq + 1);
		node->next = head;
		head = node;
		i++;
	}
	if (!get_env_value(head, "PATH"))
		set_env_value(&head, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	if (!get_env_value(head, "SHLVL"))
		set_env_value(&head, "SHLVL", "0");
	if (!get_env_value(head, "OLDPWD"))
		set_env_value(&head, "OLDPWD", "");
	if (!get_env_value(head, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
			set_env_value(&head, "PWD", cwd);
		else	
			set_env_value(&head, "PWD", "/");
		free(cwd);
	}
	int	level = ft_atoi(get_env_value(head, "SHLVL"));
	char *new_lvl_str = ft_itoa(level + 1);
	set_env_value(&head, "SHLVL", new_lvl_str);
	free(new_lvl_str);
	return (head);
}
