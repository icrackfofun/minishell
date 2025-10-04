/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:46:35 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/04 15:51:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	env_count(t_env *list)
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

// static char	*env_str(t_env *node, t_info *info)
// {
// 	char	*str;
// 	size_t	len;

// 	len = ft_strlen(node->key) + ft_strlen(node->value) + 2;
// 	str = malloc(len);
// 	if (!str)
// 		child_exit("malloc", 1, info, "");
// 	ft_strlcpy(str, node->key, ft_strlen(node->key) + 1);
// 	str[ft_strlen(node->key)] = '=';
// 	ft_strlcpy(str + ft_strlen(node->key) + 1, node->value,
// 		ft_strlen(node->value) + 1);
// 	return (str);
// }

static char	*env_str(t_env *node, t_info *info)
{
	char	*str;
	size_t	key_len;
	size_t	val_len;
	size_t	len;

	key_len = ft_strlen(node->key);
	val_len = 0;
	if (node->value)
		val_len = ft_strlen(node->value);
	len = key_len + val_len + 2;
	str = malloc(len);
	if (!str)
		child_exit("malloc", 1, info, "");

	ft_strlcpy(str, node->key, key_len + 1);
	if (node->value)
	{
		str[key_len] = '=';
		ft_strlcpy(str + key_len + 1, node->value, val_len + 1);
	}
	else
	{
		str[key_len] = '\0';
	}
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
