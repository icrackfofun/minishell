/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 18:04:23 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/02 18:36:47 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*append_piece(t_info *info, const char *str, int *i, char *res)
{
	char	*expanded;
	char	*tmp;
	char	buf[2];

	if (str[*i] == '$')
	{
		expanded = expand_var_in_quotes(info, str, i);
		if (!expanded)
			return (NULL);
		tmp = ft_strjoin(res, expanded);
		free(expanded);
	}
	else
	{
		buf[0] = str[(*i)++];
		buf[1] = '\0';
		tmp = ft_strjoin(res, buf);
	}
	return (tmp);
}

int	remove_empty_tokens_from_list(t_token **tokens)
{
	t_token	*cur;
	t_token	*next;
	char	*joined;

	cur = *tokens;
	while (cur && cur->next)
	{
		next = cur->next;
		if ((cur->value[0] == 0 || next->value[0] == 0))
		{
			joined = ft_strjoin(cur->value, next->value);
			if (!joined)
				return (1);
			free(cur->value);
			if (next->type != TOKEN_WORD)
				cur->type = next->type;
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
