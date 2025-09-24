/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:23:38 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/24 15:16:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
