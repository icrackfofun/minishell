/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 00:48:34 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/06 14:32:39 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*build_error_msg(char *a, char *b, char *c, char *d)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	if (a == NULL)
		a = "";
	if (b == NULL)
		b = "";
	if (c == NULL)
		c = "";
	if (d == NULL)
		d = "";
	tmp1 = ft_strjoin(a, b);
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strjoin(tmp1, c);
	free(tmp1);
	if (!tmp2)
		return (NULL);
	tmp3 = ft_strjoin(tmp2, d);
	free(tmp2);
	if (!tmp3)
		return (NULL);
	return (tmp3);
}

int	print_error(char *a, char *b, char *c, char *d)
{
	char	*msg;

	msg = build_error_msg(a, b, c, d);
	if (!msg)
		return (1);
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		free(msg);
	}
	return (0);
}

void	malloc_fail_exit(t_info *info)
{
	perror("malloc");
	clean_loop(info);
	clean_shell(info);
	close_std_fds();
	exit(1);
}

void	close_std_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	clean_all(t_info *info)
{
	clean_loop(info);
	clean_shell(info);
}