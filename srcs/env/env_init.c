/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:23:42 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/03 16:22:18 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	check_shlvl(t_env *env)
{
	int		level;
	char	*new_lvl;

	level = ft_atoi(get_env_value(env, "SHLVL"));
	new_lvl = ft_itoa(level + 1);
	set_env_value(&env, "SHLVL", new_lvl);
	free(new_lvl);
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;
	char	*eq;

	head = NULL;
	i = 0;
	//printf("%p\n", envp);
	//printf("%s\n", envp[i]);
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
	return (head);
}

void	populate_env(char **envp, t_info *info)
{
	t_env	*head;
	char	*cwd;
	
	info->env_list = env_init(envp);
	head = info->env_list;
	if (!get_env_value(head, "PATH") || get_env_value(head, "PATH")[0] == 0)
		set_env_value(&head, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	if (!get_env_value(head, "SHLVL") || get_env_value(head, "SHLVL")[0] == 0)
		set_env_value(&head, "SHLVL", "0");
	if (!get_env_value(head, "OLDPWD") || get_env_value(head, "OLDPWD")[0] == 0)
		set_env_value(&head, "OLDPWD", NULL);
	if (!get_env_value(head, "PWD") || get_env_value(head, "PWD")[0] == 0)
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
			set_env_value(&head, "PWD", cwd);
		else	
			set_env_value(&head, "PWD", "/");
		free(cwd);
	}
	check_shlvl(info->env_list);
}
