/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-vda <jose-vda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:23:38 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/22 18:10:40 by jose-vda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	disable_ctrl_echo(void)
// {
// 	struct termios	t;

// 	if (tcgetattr(STDIN_FILENO, &t) == -1)
// 		return ;
// 	t.c_lflag &= ~ECHOCTL;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &t);
// }

// void	enable_ctrl_echo(void)
// {
// 	struct termios	t;

// 	if (tcgetattr(STDIN_FILENO, &t) == -1)
// 		return ;
// 	t.c_lflag |= ECHOCTL;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &t);
// }

void	sigint_handler(int sig)
{
	(void)sig;
	g_last_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
	g_last_signal = SIGQUIT;
}
