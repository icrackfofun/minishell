/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:37:01 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/30 15:50:10 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredoc_files(void)
{
	DIR				*dir;
	struct dirent	*entry;
	const char		*prefix;

	prefix = ".minishell_heredoc_";
	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)) == 0)
		{
			if (unlink(entry->d_name) < 0)
				perror("unlink heredoc");
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

t_env	*free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
	return (tmp);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->target)
			free(redir->target);
		free(redir);
		redir = tmp;
	}
}

void	free_ast(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			free(node->argv[i]);
			i++;
		}
		free(node->argv);
	}
	if (node->redirs)
		free_redirs(node->redirs);
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
}

void	clean_shell(t_info *info)
{
	if (!info)
		return ;
	if (info->env_array)
		free_array(info->env_array);
	if (info->env_list)
		free_env(info->env_list);
	rl_clear_history();
}
