/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:47:25 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 17:11:10 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cd_get_path(t_ast *ast, t_info *info, int root)
{
	if (ast->argv[1])
		return (ft_strdup(ast->argv[1]));
	if (!get_env_value(info->env_list, "HOME"))
	{
		write(2, "cd: HOME not set\n", 17);
		if (!root)
			child_exit("", 1, info, "");
		parent_return("", info, 1, "");
		return (NULL);
	}
	return (ft_strdup(get_env_value(info->env_list, "HOME")));
}

int	update_env(t_info *info, char *path, int root)
{
	char	*cwd;
	char	*oldwd;

	oldwd = ft_strdup(get_env_value(info->env_list, "PWD"));
	if (chdir(path) != 0)
	{
		free(oldwd);
		if (!root)
			child_exit("cd", 1, info, path);
		return (parent_return("cd", info, 1, path), 1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		free(oldwd);
		free(path);
		if (!root)
			child_exit("cd", 1, info, "");
		return (parent_return("cd", info, 1, ""), 1);
	}
	set_env_value(&info->env_list, "PWD", cwd);
	set_env_value(&info->env_list, "OLDPWD", oldwd);
	free(oldwd);
	free(path);
	return (free(cwd), 0);
}
