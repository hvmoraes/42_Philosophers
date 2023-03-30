/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:14:22 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/29 12:12:28 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* free_data:
*	Frees all of the memory allocated by the program.
*	Returns a NULL pointer if there is nothing to free,
*	or when all memory has been freed.
*/
void	*free_data(t_data *data)
{
	unsigned int	i;

	if (!data)
		return (NULL);
	if (data->fork_locks != NULL)
		free(data->fork_locks);
	if (data->philos != NULL)
	{
		i = 0;
		while (i < data->nbr_philos)
		{
			if (data->philos[i] != NULL)
				free(data->philos[i]);
			i++;
		}
		free(data->philos);
	}
	free(data);
	return (NULL);
}

/* destroy_mutexts:
*	Destroys every mutex created by the program: fork locks, meal locks,
*	the write and simulation stopper lock.
*/
void	destroy_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		pthread_mutex_destroy(&data->fork_locks[i]);
		pthread_mutex_destroy(&data->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->sim_stop_lock);
}

/* msg:
*	Writes a message to the console. Returns the provided exit number.
*	Used for error management.
*/
int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.
*/
int	error_failure(char *str, char *details, t_data *data)
{
	if (data != NULL)
		free_data(data);
	return (msg(str, details, 0));
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_data *data)
{
	if (data != NULL)
		free_data(data);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}
