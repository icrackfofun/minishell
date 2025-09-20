/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:53:23 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/20 22:30:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_REDIRECT_APPEND
		|| token->type == TOKEN_HEREDOC);
}

int	is_redirect(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_REDIRECT_APPEND
		|| token->type == TOKEN_HEREDOC);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

void	malloc_error_lexing(t_token **tokens, char **buf, t_info *info)
{
	if (buf && *buf)
	{
		free(*buf);
		*buf = NULL;
	}
	if (tokens && *tokens)
	{
		free_tokens(*tokens);
		*tokens = NULL;
	}
	parent_exit("malloc", info);
}

int	append_char(char **buf, char c)
{
	char	*new;
	size_t	len;
	size_t	i;

	len = 0;
	if (*buf)
		len = ft_strlen(*buf);
	new = malloc(len + 2);
	if (!new)
		return (1);
	i = 0;
	while (i < len)
	{
		new[i] = (*buf)[i];
		i++;
	}
	if (*buf)
		free(*buf);
	new[len] = c;
	new[len + 1] = '\0';
	*buf = new;
	return (0);
}
