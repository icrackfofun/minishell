/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:47:25 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 09:37:54 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
