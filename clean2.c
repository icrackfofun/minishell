/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:16:22 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/26 15:00:21 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = tmp;
	}
}

void	close_parent_fds(t_info *info)
{
	if (info->leftover_read_fd != -1)
	{
		close(info->leftover_read_fd);
		info->leftover_read_fd = -1;
	}
	if (info->last_pipe_read_fd != -1)
	{
		close(info->last_pipe_read_fd);
		info->last_pipe_read_fd = -1;
	}
	if (info->last_pipe_write_fd != -1)
	{
		close(info->last_pipe_write_fd);
		info->last_pipe_write_fd = -1;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	clean_loop(t_info *info)
{
	if (info && info->tree)
	{
		free_ast(info->tree);
		info->tree = NULL;
	}
	if (info && info->cmds)
	{
		free(info->cmds);
		info->cmds = NULL;
	}
	if (info && info->child_pids)
	{
		free(info->child_pids);
		info->child_pids = NULL;
	}
	if (info && info->tokens)
		free_tokens(info->tokens);
	if (info && info->line)
	{
		free(info->line);
		info->line = NULL;
	}
	close_parent_fds(info);
}
