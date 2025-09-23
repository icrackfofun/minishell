/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 10:27:56 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/23 22:41:02 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_non_operator_tokens(t_token **tokens)
{
	t_token	*cur;
	t_token	*next;
	char	*joined;

	cur = *tokens;
	while (cur && cur->next)
	{
		next = cur->next;
		if (!is_operator(cur) && !is_operator(next)
			&& next->has_space_before == 0)
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
	while (line[*i] && ft_isspace(line[*i]))
	{
		info->had_space = 1;
		(*i)++;
	}
}

t_token	*token_error(t_token **tokens)
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
	// if (is_operator(*tokens))
	// {
	// 	if ((*tokens)->next)
	// 		error_tokens(tokens, (*tokens)->next->value);
	// 	else
	// 		error_tokens(tokens, "newline");
	// 	return (NULL);
	// }
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

t_token	*finalize_tokens(t_info *info, t_token **tokens)
{
	info->had_space = 0;
	classify_tokens(*tokens);
	return (token_error(tokens));
}
