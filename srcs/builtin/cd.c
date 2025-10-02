/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:39:44 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/03 00:39:14 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	*cd_get_path(t_ast *ast, t_info *info, int root)
{
	char	*home;

	if (ast->argv[1])
		return (ft_strdup(ast->argv[1]));
	home = get_env_value(info->env_list, "HOME");
	if (!home)
	{
		write(2, "cd: HOME not set\n", 17);
		if (!root)
			child_exit("", 1, info, "");
		info->last_status = 1;
		return ((char *)-1);
	}
	if (home[0] == 0)
		return (ft_strdup("."));
	return (ft_strdup(get_env_value(info->env_list, "HOME")));
}

void	builtin_cd(t_ast *ast, t_info *info, int root)
{
	char	*path;

	if (ast->argv[1] && ast->argv[2])
	{
		write(2, "cd: too many arguments\n", 23);
		if (!root)
			child_exit("", 1, info, "");
		return (parent_return("", info, 1, ""));
	}
	path = cd_get_path(ast, info, root);
	if (path == (char *)-1)
		return ;
	if (!path)
	{
		if (!root)
			child_exit("malloc", 1, info, "");
		return (parent_return("malloc", info, 1, ""));
	}
	if (update_env(info, path, root))
		return ;
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
