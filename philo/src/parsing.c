/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:11:42 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/23 11:41:41 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* contains_only_digits:
*	Checks if a string contains only digits 0 - 9.
*	Returns true if the string only contains digits.
*	Returns false if the string contains a character that is not a digit.
*/
static bool	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

/* integer_atoi:
*	Converts a digit-only string into a positive integer.
*	Returns the converted number between 0 and INT MAX.
*	Returns -1 if the converted number exceeds INT MAX.
*/
int	ft_atoi(char *str)
{
	unsigned long long int	nbr;
	int						i;

	i = 0;
	nbr = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (nbr > INT_MAX)
		return (-1);
	return ((int)nbr);
}

/* is_valid_input:
*	Checks if all required arguments are valid, i.e. is a string of
*	digits only, which does not exceed INT MAX. Also checks if the number
*	of philosophers is valid (between 1 and MAX_PHILOS).
*	Returns true if all arguments are valid, false if one of them is invalid.
*/
bool	valid_input(int argc, char **argv)
{
	int	i;
	int	nbr;

	i = 1;
	while (i < argc)
	{
		if (!contains_only_digits(argv[i]))
			return (msg(STR_ERR_INPUT_DIGIT, argv[i], false));
		nbr = ft_atoi(argv[i]);
		if (i == 1 && (nbr <= 0 || nbr > MAX_PHILOS))
			return (msg(STR_ERR_INPUT_POFLOW, STR_MAX_PHILOS, false));
		if (i != 1 && nbr == -1)
			return (msg(STR_ERR_INPUT_DIGIT, argv[i], false));
		i++;
	}
	return (true);
}
