/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:07:09 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/02 10:52:22 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_variable(t_info *info, int *i, char **buf, t_token **tokens)
{
	if (append_char(buf, info->line[(*i)++]))
		malloc_error_lexing(tokens, buf, info);
	if (info->line[*i] == '?')
	{
		if (append_char(buf, info->line[*i]))
			malloc_error_lexing(tokens, buf, info);
		if (append_token(tokens, buf, info))
			malloc_error_lexing(tokens, buf, info);
		return ;
	}
	else if (ft_isalpha(info->line[*i]) || info->line[*i] == '_')
	{
		if (append_char(buf, info->line[(*i)++]))
			malloc_error_lexing(tokens, buf, info);
		while (ft_isalnum(info->line[*i]) || info->line[*i] == '_')
		{
			if (append_char(buf, info->line[(*i)++]))
				malloc_error_lexing(tokens, buf, info);
		}
	}
	if (append_token(tokens, buf, info))
		malloc_error_lexing(tokens, buf, info);
	(*i)--;
}

int	join_non_operator_tokens(t_token **tokens)
{
	t_token	*cur;
	t_token	*next;
	char	*joined;

	cur = *tokens;
	while (cur && cur->next)
	{
		next = cur->next;
		if ((!is_pipe(cur) && !is_redirect(cur)) && (!is_pipe(next)
				&& !is_redirect(next)) && next->has_space_before == 0)
		{
			joined = ft_strjoin(cur->value, next->value);
			if (!joined)
				return (1);
			free(cur->value);
			cur->value = joined;
			cur->next = next->next;
			free(next->value);
			free(next);
			continue ;
		}
		cur = cur->next;
	}
	return (0);
}

void	skip_spaces_and_mark(const char *line, int *i, t_info *info)
{
	while (line[*i] && ft_isspace(line[*i]) && line [*i + 1])
	{
		info->had_space = 1;
		(*i)++;
	}
}

t_token	*token_check(t_token **tokens)
{
	t_token	*current;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_PIPE)
	{
		if ((*tokens)->next)
			error_tokens(tokens, (*tokens)->next->value);
		else
			error_tokens(tokens, "newline");
		return (NULL);
	}
	current = *tokens;
	while (current && current->next)
	{
		if ((is_pipe((current)) && is_pipe(current->next))
			|| (is_redirect(current) && is_redirect(current->next))
			|| (is_redirect(current) && is_pipe(current->next)))
			return (error_tokens(tokens, current->next->value), NULL);
		current = current->next;
	}
	if (is_pipe(current) || is_redirect(current))
		return (error_tokens(tokens, "newline"), NULL);
	return (*tokens);
}

t_token	*finalize_tokens(t_info *info, t_token **tokens)
{
	info->had_space = 0;
	classify_tokens(*tokens);
	return (token_check(tokens));
}
