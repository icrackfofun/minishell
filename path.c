/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:56:15 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 23:10:07 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_path(const char *dir, const char *cmd, t_info *info)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		child_exit("malloc", 1, info, "");
	full = ft_strjoin(tmp, cmd);
	if (!full)
		child_exit("malloc", 1, info, "");
	free(tmp);
	return (full);
}

static char	*next_token(char **str, char delim)
{
	char	*start;
	char	*cur;

	if (!str || !*str || !**str)
		return (NULL);
	start = *str;
	cur = *str;
	while (*cur && *cur != delim)
		cur++;
	if (*cur)
	{
		*cur = '\0';
		*str = cur + 1;
	}
	else
		*str = cur;
	return (start);
}

static char	*search_in_path(char *path_copy, const char *cmd, t_info *info)
{
	char	*dir;
	char	*full;
	char	*cursor;

	cursor = path_copy;
	dir = next_token(&cursor, ':');
	while (dir)
	{
		full = join_path(dir, cmd, info);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		dir = next_token(&cursor, ':');
	}
	return ((char *)cmd);
}

char	*get_path(t_info *info, t_ast *cmd)
{
	char	*path_env;
	char	*copy;
	char	*result;

	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.'
		|| cmd->argv[0][0] == 0)
		return (ft_strdup(cmd->argv[0]));
	path_env = get_env_value(info->env_list, "PATH");
	if (!path_env)
		return (cmd->argv[0]);
	copy = ft_strdup(path_env);
	if (!copy)
		child_exit("malloc", 1, info, "");
	result = search_in_path(copy, cmd->argv[0], info);
	free(copy);
	return (result);
}
