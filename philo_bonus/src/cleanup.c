/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:40:37 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 10:39:46 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

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
	if (data->philos != NULL)
	{
		i = 0;
		while (i < data->nbr_philos)
		{
			if (data->philos[i] != NULL)
			{
				if (data->philos[i]->sem_meal_name)
					free(data->philos[i]->sem_meal_name);
				free(data->philos[i]);
			}
			i++;
		}
		free(data->philos);
	}
	if (data->pids)
		free(data->pids);
	free(data);
	return (NULL);
}

/* sem_error_cleanup:
*	Closes and unlinks all semaphores. Used only during
*	initialization, in case a semaphore fails to be opened.
*	Returns 0 for failure.
*/
int	sem_error_cleanup(t_data *data)
{
	sem_close(data->sem_forks);
	sem_close(data->sem_write);
	sem_close(data->sem_philo_full);
	sem_close(data->sem_philo_dead);
	sem_close(data->sem_stop);
	unlink_global_sems();
	return (error_failure(STR_ERR_SEM, NULL, data));
}

/* data_cleanup:
*	Closes all semaphores and frees any allocated memory.
*	Also waits for the global main thread to terminate.
*	Returns the provided exit code.
*/
int	data_cleanup(t_data *data, int exit_code)
{
	if (data != NULL)
	{
		pthread_join(data->second_thread, NULL);
		pthread_join(data->third_thread, NULL);
		sem_close(data->sem_forks);
		sem_close(data->sem_write);
		sem_close(data->sem_philo_full);
		sem_close(data->sem_philo_dead);
		sem_close(data->sem_stop);
		unlink_global_sems();
		free_data(data);
	}
	return (exit_code);
}
