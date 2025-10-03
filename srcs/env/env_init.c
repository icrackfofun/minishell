/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:23:42 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/03 17:43:03 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	*new_env_node(char *entry)
{
	t_env	*node;
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strndup(entry, eq - entry);
	if (eq[1] != '\0')
		node->value = ft_strdup(eq + 1);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;

	head = NULL;
	i = 0;
	while (envp && envp[i] && envp[i][0])
	{
		node = new_env_node(envp[i]);
		if (node)
		{
			node->next = head;
			head = node;
		}
		i++;
	}
	return (head);
}

static void	check_shlvl(t_env *env)
{
	int		level;
	char	*new_lvl;

	level = ft_atoi(get_env_value(env, "SHLVL"));
	new_lvl = ft_itoa(level + 1);
	set_env_value(&env, "SHLVL", ft_strdup(new_lvl));
	free(new_lvl);
}

void	populate_env(char **envp, t_info *info)
{
	t_env	*head;
	char	*cwd;

	head = env_init(envp);
	if (!get_env_value(head, "PATH"))
		set_env_value(&head, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	if (!get_env_value(head, "SHLVL"))
		set_env_value(&head, "SHLVL", "0");
	if (!get_env_value(head, "OLDPWD"))
		set_env_value(&head, "OLDPWD", NULL);
	if (!get_env_value(head, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
			set_env_value(&head, "PWD", cwd);
		else
			set_env_value(&head, "PWD", "/");
		free(cwd);
	}
	check_shlvl(head);
	info->env_list = head;
}
