/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:00 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:14:59 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* kill_all_philos:
*	Sends the kill signal to all philosopher processes. Used to send the
*	simulation if a philosopher has died or if a philosopher process has
*	encountered a fatal error.
*	Returns the given exit code.
*/
int	kill_all_philos(t_data *data, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		kill(data->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

/* third_thread:
*	Kills all philosophers if each one has eaten enough. Each philosopher
*	process decrements the sem_philo_full semaphore. This thread registers
*	those decrementations to count how many philosophers have eaten enough.
*	If all philos have eaten enough, kills all philos to end simulation.
*/
void	*third_thread(void *data2)
{
	t_data	*data;

	data = (t_data *)data2;
	if (data->eat_count < 0 || data->time_to_die == 0
		|| data->nbr_philos == 1)
		return (NULL);
	sim_start_delay(data->start_time);
	while (data->philo_full_count < data->nbr_philos)
	{
		if (has_simulation_stopped(data) == true)
			return (NULL);
		sem_wait(data->sem_philo_full);
		if (has_simulation_stopped(data) == false)
			data->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(data->sem_stop);
	data->stop_sim = true;
	kill_all_philos(data, EXIT_SUCCESS);
	sem_post(data->sem_philo_dead);
	sem_post(data->sem_stop);
	return (NULL);
}

/* second_thread:
*	Kills all philosophers if one has died. Each philosopher
*	process decrements the sem_philo_dead semaphore upon philo death.
*	This thread registers the first decrementation and kills all philos
*	immediately.
*/
void	*second_thread(void *data2)
{
	t_data	*data;

	data = (t_data *)data2;
	if (data->nbr_philos == 1)
		return (NULL);
	sim_start_delay(data->start_time);
	if (has_simulation_stopped(data) == true)
		return (NULL);
	sem_wait(data->sem_philo_dead);
	if (has_simulation_stopped(data) == true)
		return (NULL);
	sem_wait(data->sem_stop);
	data->stop_sim = true;
	kill_all_philos(data, EXIT_SUCCESS);
	sem_post(data->sem_philo_full);
	sem_post(data->sem_stop);
	return (NULL);
}

/* end_condition_reached:
*	Checks this philosopher to see if one of two end conditions
*	has been reached. If the philosopher has died, decrements a semaphore
*	that will trigger "second_thread" to kill all philos.
*	If the philosopher has eaten enough, decrements a semaphore that
*	will trigger "third_thread" to increase philo_full_count.
*	Returns false if the philosopher is alive.
*/
static bool	end_condition_reached(t_data *data, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time() - philo->last_meal >= data->time_to_die)
	{
		write_status(philo, true, DIED);
		sem_post(data->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (data->eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= (unsigned int)data->eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

/* main_thread:
*	The main thread's routine. Checks if this philosopher must
*	be killed and if he ate enough.
*/
void	*main_thread(void *data2)
{
	t_data			*data;

	data = (t_data *)data2;
	if (data->eat_count == 0)
		return (NULL);
	sem_wait(data->this_philo->sem_philo_dead);
	sem_wait(data->this_philo->sem_philo_full);
	sim_start_delay(data->start_time);
	while (!end_condition_reached(data, data->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}
