/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:40:43 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 10:41:07 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

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

/* child_exit:
*	Exits a child philosopher process with the appropriate exit code.
*	Prints an error message if the child encountered an error.
*/
void	child_exit(t_data *data, int exit_code)
{
	sem_post(data->this_philo->sem_meal);
	pthread_join(data->this_philo->main_thread, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(data->this_philo->sem_forks);
	sem_close(data->this_philo->sem_philo_full);
	sem_close(data->this_philo->sem_philo_dead);
	sem_close(data->this_philo->sem_write);
	sem_close(data->this_philo->sem_meal);
	free_data(data);
	exit(exit_code);
}
