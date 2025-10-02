/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:54:45 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 19:24:52 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	builtin_env(int root, t_ast *ast, t_info *info)
{
	t_env	*node;

	(void)ast;
	node = info->env_list;
	while (node)
	{
		if (node->value)
			printf("%s=%s\n", node->key, node->value);
		node = node->next;
	}
	if (!root)
		child_exit("", 0, info, "");
	info->last_status = 0;
}
