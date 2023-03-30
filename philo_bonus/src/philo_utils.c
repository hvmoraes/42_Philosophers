/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:19 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:19:18 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* grab_fork:
*	Wait for the fork semaphore to allow the philosopher to take a
*	fork. When the philosopher takes a fork, the semaphore is decreased
*	by 1 to represent the fact that there is one less fork available
*	in the pool of forks.
*	There is no real distinction between the first and second fork a
*	philosopher takes, the dictinction is made here only for debug purposes.
*/
void	grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nbr_forks_held <= 0)
		write_status(philo, false, GOT_FORK_1);
	if (philo->nbr_forks_held == 1)
		write_status(philo, false, GOT_FORK_2);
	philo->nbr_forks_held += 1;
	sem_post(philo->sem_meal);
}
