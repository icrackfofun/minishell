/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flatten.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 13:09:37 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 14:29:43 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_commands(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (1);
	if (node->type == NODE_PIPE)
		return (count_commands(node->left) + count_commands(node->right));
	return (0);
}

static void	flatten_pipeline_rec(t_ast *node, t_ast **cmds, int *count)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		flatten_pipeline_rec(node->left, cmds, count);
		flatten_pipeline_rec(node->right, cmds, count);
	}
	else if (node->type == NODE_COMMAND)
		cmds[(*count)++] = node;
}

t_ast	**flatten_pipeline(t_ast *root, int *out_count)
{
	t_ast	**cmds;
	int		i;

	*out_count = count_commands(root);
	cmds = malloc(sizeof(t_ast *) * (*out_count));
	if (!cmds)
		return (NULL);
	i = 0;
	flatten_pipeline_rec(root, cmds, &i);
	return (cmds);
}
