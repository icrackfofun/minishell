/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:27:17 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/21 00:32:39 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *str)
{
	if (ft_strcmp (str, "echo") == 0)
		return (1);
	else if (ft_strcmp (str, "cd") == 0)
		return (1);
	else if (ft_strcmp (str, "pwd") == 0)
		return (1);
	else if (ft_strcmp (str, "export") == 0)
		return (1);
	else if (ft_strcmp (str, "unset") == 0)
		return (1);
	else if (ft_strcmp (str, "env") == 0)
		return (1);
	else if (ft_strcmp (str, "exit") == 0)
		return (1);
	return (0);
}
