/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 11:21:32 by jose-vda          #+#    #+#             */
/*   Updated: 2025/09/30 14:59:47 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_terminal	*terminal(void)
{
	static t_terminal	t;

	return (&t);
}

void	kill_all_children(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->child_count)
	{
		if (kill(info->child_pids[i], 0) == 0 || errno == EPERM)
			kill(info->child_pids[i], SIGTERM);
		i++;
	}
}

void	prompt_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	child_sigint_handler(int sig)
{
	(void)sig;
	child_exit("", 2, terminal()->info, "");
}
