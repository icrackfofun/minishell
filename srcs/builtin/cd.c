/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:39:44 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/06 14:15:49 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	cd_error(t_info *info, char *path, int root, char *oldwd)
{
	if (oldwd)
		free(oldwd);
	if (!root)
	{
		if (path)
			child_exit("cd", 1, info, path);
		else
			child_exit("cd", 1, info, "");
	}
	if (path)
		return (parent_return("cd", info, 1, path), 1);
	else
		return (parent_return("cd", info, 1, ""), 1);
}

int	update_env(t_info *info, char *path, int root)
{
	char	*cwd;
	char	*oldwd;

	if (get_env_value(info->env_list, "PWD"))
		oldwd = ft_strdup(get_env_value(info->env_list, "PWD"));
	else
	{
		oldwd = getcwd(NULL, 0);
		if (!oldwd)
			return (cd_error(info, path, root, NULL));
	}
	if (!oldwd)
		child_exit("malloc", 1, info, path);
	if (chdir(path) != 0)
		return (cd_error(info, path, root, oldwd));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (cd_error(info, NULL, root, oldwd));
	set_env_value(&info->env_list, "PWD", cwd);
	set_env_value(&info->env_list, "OLDPWD", oldwd);
	free(path);
	free(oldwd);
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
	return (ft_strdup(home));
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
			malloc_fail_exit(info);
		return (parent_exit("malloc", info));
	}
	if (update_env(info, path, root))
		return ;
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
