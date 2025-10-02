/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:50:26 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 18:36:09 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	builtin_unset(int root, t_ast *ast, t_info *info)
{
	int		i;
	char	*key;

	i = 1;
	while (ast->argv[i])
	{
		if (ft_is_valid(ast->argv[i], 0))
		{
			key = ast->argv[i];
			unset_env(&info->env_list, key);
		}
		else
		{
			write(2, "unset: `", 9);
			write(2, ast->argv[i], ft_strlen(ast->argv[i]));
			write(2, "\': not a valid identifier\n", 26);
			if (!root)
				child_exit("", 1, info, "");
			return (parent_return("", info, 1, ""));
		}
		i++;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}