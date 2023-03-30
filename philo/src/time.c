/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:12:52 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/29 12:27:20 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* get_time:
*	Gets the current time in miliseconds.
*	Returns the time value.
*/
time_t	get_time(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/* philo_sleep:
*	Pauses the philosopher thread for a certain amount of time in miliseconds.
*	Periodically checks to see if the simulation has ended during the sleep
*	time and cuts the sleep short if it has.
*/
void	philo_sleep(t_data *data, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time() + sleep_time;
	while (get_time() < wake_up)
	{
		if (has_simulation_stopped(data))
			break ;
		usleep(100);
	}
}

/* sim_start_delay:
*	Waits for a small delay at the beginning of each threads execution
*	so that all threads start at the same time with the same start time
*	reference. This ensures the grim reaper thread is synchronized with
*	the philosopher threads.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time() < start_time)
		continue ;
}
