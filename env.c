/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:58:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/14 14:23:26 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_array(char **arr)
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

static char	*env_str(t_env *node)
{
	char	*str;
	size_t	len;

	len = ft_strlen(node->key) + ft_strlen(node->value) + 2;
	str = malloc(len);
	if (!str)
		child_exit("malloc", 1);
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

	free_env_array(info->env_array);
	size = env_count(info->env_list);
	info->env_array = malloc(sizeof(char *) * (size + 1));
	if (!info->env_array)
		child_exit("malloc", 1);
	tmp = info->env_list;
	i = 0;
	while (tmp)
	{
		info->env_array[i++] = env_str(tmp);
		tmp = tmp->next;
	}
	info->env_array[i] = NULL;
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;
	char	*eq;

	head = NULL;
	i = 0;
	while (envp[i])
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
	return (head);
}
