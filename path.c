/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:56:15 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/04 23:33:03 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
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

static char	*search_in_path(char *path_copy, const char *cmd)
{
	char	*dir;
	char	*full;
	char	*cursor;

	cursor = path_copy;
	dir = next_token(&cursor, ':');
	while (dir)
	{
		full = join_path(dir, cmd);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		dir = next_token(&cursor, ':');
	}
	return (NULL);
}

char	*get_path(t_command *cmd)
{
	char	*path_env;
	char	*copy;
	char	*result;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (NULL);
	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
		return (ft_strdup(cmd->argv[0]));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	copy = ft_strdup(path_env);
	if (!copy)
		return (NULL);
	result = search_in_path(copy, cmd->argv[0]);
	free(copy);
	return (result);
}
