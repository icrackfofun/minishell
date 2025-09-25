/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:51:34 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 23:15:26 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// void	remove_empty_tokens_from_list(t_token **tokens)
// {
//     t_token *cur;
//     t_token *prev;
//     t_token *tmp;

// 	cur = *tokens;
// 	prev = NULL;
//     while (cur)
//     {
//         if (!cur->value || cur->value[0] == '\0')
//         {
//             tmp = cur;
//             if (prev)
//                 prev->next = cur->next;
//             else
//                 *tokens = cur->next;
//             cur = cur->next;
//             free(tmp->value);
//             free(tmp);
//         }
//         else
//         {
//             prev = cur;
//             cur = cur->next;
//         }
//     }
// }
