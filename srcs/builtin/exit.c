/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:55:35 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 18:35:58 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//codes are wrong, exit with letters yes, with 2 numbers yes, number + letters no (look into)
void	builtin_exit(t_ast *ast, t_info *info, int root)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (!ast->argv[1])
		status = info->last_status;
	else if (!ft_isnum(ast->argv[1]))
	{
		if (!root)
			child_exit("exit", 2, info, ft_strdup(ast->argv[1]));
		child_exit("exit", 2, info, ft_strdup(ast->argv[1]));
	}
	else if (ast->argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		if (!root)
			child_exit("", 1, info, "");
		return (parent_return("", info, 1, ""));
	}
	else
		status = ft_atoi(ast->argv[1]);
	if (!root)
		child_exit("", status, info, "");
	child_exit("", status, info, "");
}