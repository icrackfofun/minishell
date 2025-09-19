/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:44:13 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/19 00:21:57 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(const char *line, int *i, char **buf, t_token **tokens)
{
	char	quote;

	quote = line[*i];
	if (append_char(buf, quote))
		return (malloc_error_lexing(tokens, buf), 0);
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (append_char(buf, line[*i]))
			return (malloc_error_lexing(tokens, buf), 0);
		(*i)++;
	}
	if (line[*i] == quote)
	{
		if (append_char(buf, quote))
			return (malloc_error_lexing(tokens, buf), 1);
		if (append_token(tokens, buf))
            return (malloc_error_lexing(tokens, buf), 0);
		return (1);
	}
	if (buf && *buf)
		free(*buf);
	error_tokens(tokens, "newline");
	return (0);
}

static void	handle_operator(t_token **tokens, char **buf,
							const char *line, int *i)
{
	char	op[3];

	op[0] = line[*i];
	op[1] = '\0';
	if ((line[*i] == '>' && line[*i + 1] == '>')
		|| (line[*i] == '<' && line[*i + 1] == '<'))
	{
		op[1] = line[*i + 1];
		op[2] = '\0';
		(*i)++;
	}
	if (append_token(tokens, buf))
	{
		while (line[*i + 1])
				i++;
		return (malloc_error_lexing(tokens, buf));
	}
	if (add_token(tokens, new_token(op)))
	{
		while (line[*i + 1])
				(*i)++;
		return (malloc_error_lexing(tokens, buf));
	}
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

static void	handle_char_or_space(char *line, char **buf,
								t_token **tokens, int *i)
{
	if (!ft_isspace(line[*i]))
	{
		if (append_char(buf, line[*i]))
		{
			while (line[*i + 1])
				(*i)++;
			return (malloc_error_lexing(tokens, buf));
		}
	}
	else
	{
		if (append_token(tokens, buf))
		{
			while (line[*i + 1])
				(*i)++;
			return (malloc_error_lexing(tokens, buf));
		}
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
			if (!handle_quotes(info->line, &i, &buf, &tokens))
				return (NULL);
		}
		else if (info->line[i] == '|' || info->line[i] == '<'
			|| info->line[i] == '>')
			handle_operator(&tokens, &buf, info->line, &i);
		else
			handle_char_or_space(info->line, &buf, &tokens, &i);
		i++;
	}
	if (append_token(&tokens, &buf))
		malloc_error_lexing(&tokens, &buf);
	classify_tokens(tokens);
	return (token_error(&tokens));
}
