/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:48:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/03 17:48:59 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	builtin_pwd(t_info *info, int root)
{
	char	*cwd;

	cwd = ft_strdup(get_env_value(info->env_list, "PWD"));
	if (!cwd || cwd[0] == 0)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			if (!root)
				child_exit("pwd", 1, info, "");
			return (parent_return("pwd", info, 1, ""));
		}
	}
	printf("%s\n", cwd);
	free(cwd);
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
