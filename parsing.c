/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:33:27 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/21 00:57:40 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*find_last_pipe(t_token *tokens)
{
	t_token	*last;

	last = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			last = tokens;
		tokens = tokens->next;
	}
	return (last);
}

static void	add_arg(t_ast *cmd, char *value, int *argc)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (*argc + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (i < *argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(value);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	(*argc)++;
}

static void	add_redir(t_ast *cmd, t_token *op, t_token *target)
{
	t_redir	*new;
	t_redir	*last;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	if (op->type == TOKEN_REDIRECT_IN)
		new->type = REDIR_INPUT;
	else if (op->type == TOKEN_REDIRECT_OUT)
		new->type = REDIR_OUTPUT;
	else if (op->type == TOKEN_REDIRECT_APPEND)
		new->type = REDIR_APPEND;
	else
		new->type = REDIR_HEREDOC;
	new->target = ft_strdup(target->value);
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

static t_ast	*parse_command(t_token *tokens)
{
	t_ast	*cmd;
	int		argc;

	cmd = malloc(sizeof(t_ast));
	if (!cmd)
		return (NULL);
	cmd->type = NODE_COMMAND;
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	argc = 0;
	while (tokens)
	{
		if (is_redirect(tokens) && tokens->next)
		{
			add_redir(cmd, tokens, tokens->next);
			tokens = tokens->next;
		}
		else
			add_arg(cmd, tokens->value, &argc);
		tokens = tokens->next;
	}
	cmd->is_builtin = is_builtin(cmd->argv[0]);
	return (cmd);
}

t_ast *parsing(t_token *tokens)
{
    t_token	*pipe_tok;
    t_ast	*node;
    t_token	*tmp;

    if (!tokens)
        return NULL;
    pipe_tok = find_last_pipe(tokens);
    if (pipe_tok)
    {
        node = malloc(sizeof(t_ast));
        if (!node)
            return (NULL);
        node->type = NODE_PIPE;
        node->argv = NULL;
        node->redirs = NULL;
        node->is_builtin = 0;

        // Split list at the pipe
        tmp = tokens;
        while (tmp && tmp->next != pipe_tok)
            tmp = tmp->next;
        if (tmp)
            tmp->next = NULL; // terminate left list

        node->left = parsing(tokens);
        node->right = parsing(pipe_tok->next);

        if (tmp)
            tmp->next = pipe_tok; // restore list if needed
        return node;
    }
    return parse_command(tokens);
}
