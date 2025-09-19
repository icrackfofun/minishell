/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:46:47 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/15 02:04:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	sign_counter;
	int	number;

	sign_counter = 0;
	number = 0;
	while ((*str == 32 || (*str >= 9 && *str <= 13)) && *str)
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign_counter++;
		str++;
	}
	while ((*str >= '0' && *str <= '9') && *str)
		number = (number * 10) + (*str++ - '0');
	if (!((*str >= '0' && *str <= '9') || *str == 0))
		return (255);
	if (sign_counter == 1)
		return (-number);
	return (number);
}
