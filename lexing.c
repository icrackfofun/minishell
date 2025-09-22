/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-vda <jose-vda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:44:13 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/22 17:19:25 by jose-vda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(t_info *info, int *i, char **buf, t_token **tokens)
{
	char	quote;

	quote = info->line[*i];
	if (append_char(buf, quote))
		malloc_error_lexing(tokens, buf, info);
	(*i)++;
	while (info->line[*i] && info->line[*i] != quote)
	{
		if (append_char(buf, info->line[*i]))
			malloc_error_lexing(tokens, buf, info);
		(*i)++;
	}
	if (info->line[*i] == quote)
	{
		if (append_char(buf, quote))
			malloc_error_lexing(tokens, buf, info);
		if (append_token(tokens, buf))
			malloc_error_lexing(tokens, buf, info);
		return (0);
	}
	if (buf && *buf)
		free(*buf);
	error_tokens(tokens, "newline");
	return (1);
}

static void	handle_operator(t_token **tokens, char **buf,
							t_info *info, int *i)
{
	char	op[3];

	op[0] = info->line[*i];
	op[1] = '\0';
	if ((info->line[*i] == '>' && info->line[*i + 1] == '>')
		|| (info->line[*i] == '<' && info->line[*i + 1] == '<'))
	{
		op[1] = info->line[*i + 1];
		op[2] = '\0';
		(*i)++;
	}
	if (append_token(tokens, buf))
		malloc_error_lexing(tokens, buf, info);
	if (add_token(tokens, new_token(op)))
		malloc_error_lexing(tokens, buf, info);
}

static t_token	*token_error(t_token **tokens)
{
	t_token	*current;

	if (!tokens || !*tokens)
		return (NULL);
	if (is_operator(*tokens))
	{
		if ((*tokens)->next)
			error_tokens(tokens, (*tokens)->next->value);
		else
			error_tokens(tokens, (*tokens)->value);
		return (NULL);
	}
	current = *tokens;
	while (current && current->next)
	{
		if (is_operator(current) && is_operator(current->next))
			return (error_tokens(tokens, current->next->value), NULL);
		current = current->next;
	}
	if (is_operator(current))
		return (error_tokens(tokens, "newline"), NULL);
	return (*tokens);
}

static void	handle_char_or_space(t_info *info, char **buf,
								t_token **tokens, int *i)
{
	if (!ft_isspace(info->line[*i]))
	{
		if (append_char(buf, info->line[*i]))
			malloc_error_lexing(tokens, buf, info);
		if (info->line[*i + 1] == '\'' || info->line[*i + 1] == '\"')
		{
			if (append_token(tokens, buf))
				malloc_error_lexing(tokens, buf, info);
		}
	}
	else
	{
		if (append_token(tokens, buf))
			malloc_error_lexing(tokens, buf, info);
	}
}

t_token	*lexing(t_info *info)
{
	t_token	*tokens;
	char	*buf;
	int		i;

	tokens = NULL;
	buf = NULL;
	i = 0;
	while (info->line[i])
	{
		if (info->line[i] == '\'' || info->line[i] == '\"')
		{
			if (handle_quotes(info, &i, &buf, &tokens))
				return (NULL);
		}
		else if (info->line[i] == '|' || info->line[i] == '<'
			|| info->line[i] == '>')
			handle_operator(&tokens, &buf, info, &i);
		else
			handle_char_or_space(info, &buf, &tokens, &i);
		i++;
	}
	if (append_token(&tokens, &buf))
		malloc_error_lexing(&tokens, &buf, info);
	classify_tokens(tokens);
	return (token_error(&tokens));
}
