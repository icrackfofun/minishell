/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expanded.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 09:37:28 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/05 10:51:16 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*tokenize(const char *val, t_info *info)
{
	t_token	*new_list;
	t_token	*empty;
	char	*buf;
	int		i;
	int		added;

	new_list = NULL;
	buf = NULL;
	i = 0;
	added = 0;
	while (val[i])
	{
		skip_spaces_and_mark(val, &i, info);
		if (ft_isspace(val[i]) && !val[i + 1])
			break ;
		while (val[i] && !ft_isspace(val[i]))
		{
			if (append_char(&buf, val[i]))
				malloc_error_lexing(&new_list, &buf, info);
			i++;
		}
		append_token(&new_list, &buf, info);
		added = 1;
	}
	if (!added)
	{
		empty = new_token("");
		if (!empty)
			malloc_error_lexing(&new_list, &buf, info);
		add_token(&new_list, empty, info);
	}
	return (new_list);
}

void	lex_expanded(t_token **head, t_token *token, t_info *info)
{
	t_token	*new_list;
	t_token	*tail;
	t_token	*prev;
	t_token	*tmp;

	tail = token->next;
	new_list = tokenize(token->value, info);
	free(token->value);
	free(token);
	tmp = new_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = tail;
	if (*head == token)
		*head = new_list;
	else
	{
		prev = *head;
		while (prev && prev->next != token)
			prev = prev->next;
		if (prev)
			prev->next = new_list;
	}
}
