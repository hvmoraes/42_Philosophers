/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:10 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 10:52:51 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* print_status_debug:
*	Prints the philosopher's status in an easier to read,
*	colorful format to help with debugging. For fork-taking
*	statuses, extra information is displayed to show which fork
*	the philosopher has taken.
*/
void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #1\n",
			get_time() - philo->data->start_time,
			color, philo->id + 1, str);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #2\n",
			get_time() - philo->data->start_time,
			color, philo->id + 1, str);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time() - philo->data->start_time,
			color, philo->id + 1, str);
}

/* write_status_debug:
*	Redirects the status writing for debug mode. For this option,
*	the DEBUG_FORMATTING option must be set to 1 in philo_bonus.h.
*/
static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

/* print_status:
*	Prints a philosopher's status in plain text as required by the project
*	subject:
*		timestamp_in_ms X status
*/
void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time() - philo->data->start_time,
		philo->id + 1, str);
}

/* write_status:
*	Prints the status of a philosopher. Decrements the write semaphore to
*	avoid intertwined messages from different processes. If the message comes
*	from a main_thread, the write semaphore is not increased again after
*	the message is displayed in the terminal, to avoid any status messages
*	being displayed after a philosopher has died.
*
*	If DEBUG_FORMATTING is set to 1 in philo_bonus.h, the status will
*	be formatted with colors and extra information to help with debugging.
*	Otherwise the output will be the regular format required by the project
*	subject.
*/
void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		if (!reaper_report)
			sem_post(philo->sem_write);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}

/* write_outcome:
*	Prints the outcome of the simulation if a number of times to
*	eat was specified. Only used for debug purposes.
*/
void	write_outcome(t_data *data)
{
	sem_post(data->sem_write);
	sem_wait(data->sem_write);
	printf("%d/%d philosophers had at least %d meals.\n",
		data->philo_full_count, data->nbr_philos, data->eat_count);
	sem_post(data->sem_write);
	return ;
}
