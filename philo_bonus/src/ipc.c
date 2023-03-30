/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:40:52 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:16:37 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* philo_open_global_semaphores:
*	Opens the semaphores shared between the parent and all child processes:
*		- Fork semaphore,
*		- Write semaphore,
*		- Full/philo ate enough semaphore.
*		- Dead/philo is dead semaphore.
*	Returns true if all semaphores were successfilly opened, false if
*	one semaphore failed to open.
*/
static bool	philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->data->nbr_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->data->nbr_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->data->nbr_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

/* philo_open_local_semaphores:
*	Opens the semaphores used only by this child process. These semaphores
*	are not shared with the parent or any other child processes. They are used
*	to regulate only this process and its main_thread's access to certain
*	variables. They are:
*		- Meal semaphore (protects variables holding the philosophers last
*			meal time).
*		- Dead semaphore (so the main_thread can interrupt the philosopher's
*			routine).
*	Returns true if all semaphores were successfilly opened, false if
*	one semaphore failed to open.
*/
static bool	philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

/* init_philo_ipc:
*	Initializes interprocess communication between philosopher processes.
*	Each philosopher opens the same named semaphores. These	semaphores are 
*	stored in shared memory and are not duplicated for each process.
*
*	Each philosopher process also creates its own main_thread which
*	detects if the philosopher is dead or has eaten enough.
*/
void	init_philo_ipc(t_data *data, t_philo *philo)
{
	if (data->nbr_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!philo_open_global_semaphores(philo))
		child_exit(data, CHILD_EXIT_ERR_SEM);
	if (!philo_open_local_semaphores(philo))
		child_exit(data, CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->main_thread, NULL,
			&main_thread, data) != 0)
		child_exit(data, CHILD_EXIT_ERR_PTHREAD);
	return ;
}
