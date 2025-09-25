/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:44:13 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 15:36:19 by psantos-         ###   ########.fr       */
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
		if (append_token(tokens, buf, info))
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
	if (add_token(tokens, new_token(op), info))
		malloc_error_lexing(tokens, buf, info);
}

static void	handle_char(t_info *info, char **buf,
								t_token **tokens, int *i)
{
	if (append_char(buf, info->line[*i]))
		malloc_error_lexing(tokens, buf, info);
	if (is_operator_redir_char(info->line[*i + 1]) || info->line[*i + 1] == '$')
	{
		if (append_token(tokens, buf, info))
			malloc_error_lexing(tokens, buf, info);
	}
	else if (!info->line[*i + 1] || ft_isspace(info->line[*i + 1]))
	{
		if (append_token(tokens, buf, info))
			malloc_error_lexing(tokens, buf, info);
	}
}

static int	process_char(t_info *info, int *i, char **buf,
						t_token **tokens)
{
	if (info->line[*i] == '\'' || info->line[*i] == '\"')
	{
		if (handle_quotes(info, i, buf, tokens))
			return (1);
	}
	else if (info->line[*i] == '$')
		handle_variable(info, i, buf, tokens);
	else if (info->line[*i] == '|' || info->line[*i] == '<'
		|| info->line[*i] == '>')
		handle_operator(tokens, buf, info, i);
	else
		handle_char(info, buf, tokens, i);
	return (0);
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
		skip_spaces_and_mark(info->line, &i, info);
		if (!info->line[i])
			break ;
		if (process_char(info, &i, &buf, &tokens))
			return (NULL);
		i++;
	}
	return (finalize_tokens(info, &tokens));
}
