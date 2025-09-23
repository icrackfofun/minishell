/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:33:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/23 15:44:52 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_arg(t_ast *cmd, char *value, int *argc, t_info *info)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (*argc + 2));
	if (!new_argv)
		parent_exit("malloc", info);
	i = 0;
	while (i < *argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(value);
	if (!new_argv[i])
	{
		free_array(new_argv);
		parent_exit("strdup", info);
	}
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	(*argc)++;
}

static void	add_redir(t_ast *cmd, t_token *op, t_token *target, t_info *info)
{
	t_redir	*new;
	t_redir	*last;

	new = malloc(sizeof(t_redir));
	if (!new)
		parent_exit("malloc", info);
	new->type = redir_type(op);
	new->target = ft_strdup(target->value);
	if (!new->target)
	{
		free_redirs(new);
		parent_exit("strdup", info);
	}
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

static void	parse_command(t_ast *cmd, t_token *tokens, t_info *info)
{
	int	argc;

	argc = 0;
	cmd->type = NODE_COMMAND;
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redirect(tokens) && tokens->next)
		{
			add_redir(cmd, tokens, tokens->next, info);
			tokens = tokens->next;
		}
		else
			add_arg(cmd, tokens->value, &argc, info);
		tokens = tokens->next;
	}
	cmd->is_builtin = is_builtin(cmd->argv[0]);
}

static void	build_ast(t_ast *node, t_token *tokens,
					int pipe_count, t_info *info)
{
	t_token	*pipe_tok;

	if (pipe_count > 0)
	{
		pipe_tok = find_pipe(tokens, pipe_count);
		node->type = NODE_PIPE;
		node->argv = NULL;
		node->redirs = NULL;
		node->is_builtin = 0;
		node->left = malloc(sizeof(t_ast));
		if (!node->left)
			parent_exit("malloc", info);
		build_ast(node->left, tokens, pipe_count - 1, info);
		node->right = malloc(sizeof(t_ast));
		if (!node->right)
			parent_exit("malloc", info);
		parse_command(node->right, pipe_tok->next, info);
	}
	else
		parse_command(node, tokens, info);
}

void	parsing(t_info *info, int pipe_count)
{
	if (!info->tokens)
		return ;
	info->tree = malloc(sizeof(t_ast));
	if (!info->tree)
		parent_exit("malloc", NULL);
	build_ast(info->tree, info->tokens, pipe_count, info);
}
