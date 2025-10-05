/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:55:35 by psantos-          #+#    #+#             */
/*   Updated: 2025/10/05 17:38:42 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_llong(const char *str)
{
	const char	*limit;
	int			sign;
	size_t		len;

	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str == '0')
		str++;
	len = ft_strlen(str);
	if (len == 0)
		return (1);
	if (sign > 0)
		limit = "9223372036854775807";
	else
		limit = "9223372036854775808";
	if (len > 19)
		return (0);
	if (len == 19 && ft_strcmp(str, limit) > 0)
		return (0);
	return (1);
}

void	builtin_exit(t_ast *ast, t_info *info, int root)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (!ast->argv[1])
		status = info->last_status;
	else if (!ft_isnum(ast->argv[1]) || !is_valid_llong(ast->argv[1]))
		child_exit("exit", 2, info, ft_strdup(ast->argv[1]));
	else if (ast->argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		if (!root)
			child_exit("", 1, info, "");
		return (parent_return("", info, 1, ""));
	}
	else
		status = (unsigned char)ft_atoll(ast->argv[1]);
	clean_loop(info);
	clean_shell(info);
	(close(STDIN_FILENO), close(STDOUT_FILENO), close(STDERR_FILENO));
	exit(status);
}
