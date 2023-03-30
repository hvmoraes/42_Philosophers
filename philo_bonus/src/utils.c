/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:33 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 10:57:33 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* ft_strlen:
*	Measures the length of a string.
*/
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

/* ft_strcat:
*	Concatenates two strings.
*/
char	*ft_strcat(char	*dst, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}

/* ft_utoa:
*	Turns a unsigned integer into a string of characters. The length of
*	the string must be calculated in advance and specified to this function.
*/
char	*ft_itoa(unsigned int nbr, size_t len)
{
	char	*result;

	result = malloc(sizeof * result * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	len--;
	while (nbr % 10)
	{
		result[len--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (result);
}

void	unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}

bool	start_main_thread(t_data *data)
{
	if (pthread_create(&data->third_thread, NULL,
			&third_thread, data) != 0)
		return (error_failure(STR_ERR_THREAD, NULL, data));
	if (pthread_create(&data->second_thread, NULL,
			&second_thread, data) != 0)
		return (error_failure(STR_ERR_THREAD, NULL, data));
	return (true);
}
