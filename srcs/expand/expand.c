/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:45:32 by jose-vda          #+#    #+#             */
/*   Updated: 2025/10/04 21:16:41 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_var_value(t_info *info, const char *key)
{
	char	*value;

	if (!key || !*key)
		return (ft_strdup("$"));
	if (key[0] == '?')
		return (ft_itoa(info->last_status));
	value = get_env_value(info->env_list, key);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_var_in_quotes(t_info *info, const char *str, int *i)
{
	int		start;
	char	*key;
	char	*val;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
		(*i)++;
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		(*i)++;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
			(*i)++;
	}
	key = ft_strndup(str + start, *i - start);
	if (!key)
		return (NULL);
	val = expand_var_value(info, key);
	free(key);
	return (val);
}

char	*expand_inside_quotes(t_info *info, const char *str)
{
	char	*res;
	char	*tmp;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		tmp = append_piece(info, str, &i, res);
		if (!tmp)
			return (free(res), NULL);
		free(res);
		res = tmp;
	}
	return (res);
}

static int	expand_token(t_info *info, t_token *cur)
{
	char	*exp;
	char	*inner;

	if ((!cur || cur->type == TOKEN_WORD || is_pipe(cur) || is_redirect(cur))
		|| (cur->type == TOKEN_VARIABLE && cur->has_heredoc_before == 1))
		return (0);
	exp = NULL;
	if (cur->type == TOKEN_VARIABLE)
		exp = expand_var_value(info, cur->value + 1);
	else if (cur->type == TOKEN_DOUBLE_QUOTED && cur->has_heredoc_before == 0)
	{
		inner = ft_strndup(cur->value + 1, ft_strlen(cur->value) - 2);
		if (!inner)
			return (1);
		exp = expand_inside_quotes(info, inner);
		free(inner);
	}
	else if (cur->type == TOKEN_SINGLE_QUOTED || cur->type == TOKEN_DOUBLE_QUOTED)
		exp = ft_strndup(cur->value + 1, ft_strlen(cur->value) - 2);
	if (!exp)
		return (1);
	free(cur->value);
	cur->value = exp;
	cur->type = TOKEN_WORD;
	return (0);
}

void	expand_variables(t_info *info)
{
	t_token	*cur;
	int		count;

	cur = info->tokens;
	while (cur)
	{
		if (expand_token(info, cur) == 1)
			return (parent_exit("malloc", info));
		cur = cur->next;
	}
	cur = info->tokens;
	count = 0;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
			count++;
		cur = cur->next;
	}
	info->pipe_count = count;
	if (join_non_operator_tokens(&info->tokens))
		return (parent_exit("malloc", info));
	remove_empty_tokens_from_list(&info->tokens);
	token_check(&info->tokens);
}
