/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:27:17 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 23:14:55 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_pipe(t_token *tokens, int n)
{
	t_token	*cur;
	int		total;

	total = 0;
	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
			total++;
		if (total == n)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_redir_type	redir_type(t_token *redir)
{
	if (redir->type == TOKEN_REDIRECT_IN)
		return (REDIR_INPUT);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUTPUT);
	else if (redir->type == TOKEN_REDIRECT_APPEND)
		return (REDIR_APPEND);
	else
		return (REDIR_HEREDOC);
}

int	is_builtin(char *str)
{
	if (ft_strcmp (str, "echo") == 0)
		return (1);
	else if (ft_strcmp (str, "cd") == 0)
		return (1);
	else if (ft_strcmp (str, "pwd") == 0)
		return (1);
	else if (ft_strcmp (str, "export") == 0)
		return (1);
	else if (ft_strcmp (str, "unset") == 0)
		return (1);
	else if (ft_strcmp (str, "env") == 0)
		return (1);
	else if (ft_strcmp (str, "exit") == 0)
		return (1);
	return (0);
}

int	is_operator_redir_char(char c)
{
	return (c == '\'' || c == '\"' || c == '|'
		|| c == '<' || c == '>');
}

int	ft_is_valid(char *name, int i, char delim)
{
	if (!(ft_isalpha(name[i]) || name[i] == '_' || name[i] == '?'))
		return (0);
	i++;
	while (name[i] != delim)
	{
		if (!(ft_isalnum((unsigned char)name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
