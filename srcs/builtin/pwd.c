/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:48:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 18:36:06 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//change to get_env("PWD") or both
void	builtin_pwd(t_info *info, int root)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (!root)
			child_exit("pwd", 1, info, "");
		return (parent_return("pwd", info, 1, ""));
	}
	printf("%s\n", cwd);
	free(cwd);
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}