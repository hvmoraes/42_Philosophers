/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:29 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:20:17 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* get_time:
*	Gets the current time in miliseconds.
*	Returns the time value.
*/
time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* philo_sleep:
*	Pauses the philosopher process for a certain amount of time in miliseconds.
*/
void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time() + sleep_time;
	while (get_time() < wake_up)
	{
		usleep(100);
	}
}

/* sim_start_delay:
*	Waits for a small delay at the beginning of each process execution
*	so that all processes start at the same time with the same start time
*	reference. This ensures all processes and the main thread are
*	synchronized.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time() < start_time)
		continue ;
}
