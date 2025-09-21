/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:32:59 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/21 22:44:33 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (NULL);
	token->next = NULL;
	return (token);
}

int	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;
	int		len;

	if (!new)
		return (1);
	if (new && new->value)
	{
		len = ft_strlen(new->value);
		while (len > 0 && new->value[len - 1] == ' ')
			new->value[--len] = '\0';
	}
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}

int	append_token(t_token **tokens, char **buf)
{
	t_token	*token;

	if (*buf && **buf)
	{
		token = new_token(*buf);
		if (!token)
			return (1);
		add_token(tokens, token);
		free(*buf);
		*buf = NULL;
	}
	return (0);
}

void	classify_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "|") == 0)
			tokens->type = TOKEN_PIPE;
		else if (ft_strcmp(tokens->value, "<") == 0)
			tokens->type = TOKEN_REDIRECT_IN;
		else if (ft_strcmp(tokens->value, ">") == 0)
			tokens->type = TOKEN_REDIRECT_OUT;
		else if (ft_strcmp(tokens->value, ">>") == 0)
			tokens->type = TOKEN_REDIRECT_APPEND;
		else if (ft_strcmp(tokens->value, "<<") == 0)
			tokens->type = TOKEN_HEREDOC;
		else if (tokens->value[0] == '\"')
			tokens->type = TOKEN_DOUBLE_QUOTED;
		else if (tokens->value[0] == '\'')
			tokens->type = TOKEN_SINGLE_QUOTED;
		else if (ft_strncmp(tokens->value, "$", 1) == 0)
			tokens->type = TOKEN_VARIABLE;
		else
			tokens->type = TOKEN_WORD;
		tokens = tokens->next;
	}
}

void	error_tokens(t_token **tokens, const char *value)
{
	if (value[0] != 0)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			value);
	}
	if (*tokens)
		free_tokens(*tokens);
}
