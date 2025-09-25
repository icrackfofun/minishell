/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:23:38 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/25 19:00:23 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	g_last_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// static void sigint_heredoc(int sig)
// {
//     (void)sig;
//     g_last_signal = 130;
//     rl_done = 1;
// }

// void	sigquit_handler(int sig)
// {
// 	(void)sig;
// 	g_last_signal = SIGQUIT;
// }

// void	sigint_handler_child(int sig, t_info *info)
// {
// 	(void)sig;
// 	g_last_signal = SIGINT;
// 	exit
// }
