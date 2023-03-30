/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:05 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:26:52 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* has_simulation_stopped:
*	Checks if the simulation is about to end.
*	Returns true if the simulation must stop, false if not.
*/
bool	has_simulation_stopped(t_data *data)
{
	bool	result;

	sem_wait(data->sem_stop);
	result = data->stop_sim;
	sem_post(data->sem_stop);
	return (result);
}

/* start_simulation:
*	Launches the simulation by creating a new child process for each
*	philosopher. The process ids are recorded to be able to wait for each
*	child in turn, and send them a kill signal if need be.
*	Also creates a main thread to monitor philosophers and detect
*	if everyone has eaten enough.
*
*	Returns true if the simulation was successfully started, false if there
*	was an error.
*/
static bool	start_simulation(t_data *data)
{
	unsigned int	i;
	pid_t			pid;

	data->start_time = get_time() + ((data->nbr_philos * 2) * 10);
	i = -1;
	while (++i < data->nbr_philos)
	{
		pid = fork();
		if (pid == -1)
			return (error_failure(STR_ERR_FORK, NULL, data));
		else if (pid > 0)
			data->pids[i] = pid;
		else if (pid == 0)
		{
			data->this_philo = data->philos[i];
			philo(data);
		}
	}
	if (start_main_thread(data) == false)
		return (false);
	return (true);
}

/* get_child_philo:
*	Waits for a philo process to exit. If the philo process
*	exits with an error or a dead philosopher, sends the signal to
*	kill all other child processes.
*/
static int	get_child_philo(t_data *data, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (kill_all_philos(data, 1));
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(data, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				data->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

/* stop_simulation:
*	Waits for each child process and analyses its exit code
*	as long as the simulation is still ongoing.
*	Ends the simulation when one of the end conditions are fulfilled:
*	when a philosopher dies or when all philosophers have eaten enough.
*/
static int	stop_simulation(t_data	*data)
{
	unsigned int	i;
	int				exit_code;

	sim_start_delay(data->start_time);
	while (has_simulation_stopped(data) == false)
	{
		i = 0;
		while (i < data->nbr_philos)
		{
			exit_code = get_child_philo(data, &data->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(data->sem_stop);
				data->stop_sim = true;
				sem_post(data->sem_philo_full);
				sem_post(data->sem_philo_dead);
				sem_post(data->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

/* main:
*	Main philosophers function.
*/
int	main(int argc, char **argv)
{
	t_data	*data;

	data = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!valid_input(argc, argv))
		return (EXIT_FAILURE);
	data = init_data(argc, argv, 1);
	if (!data)
		return (EXIT_FAILURE);
	if (!start_simulation(data))
		return (EXIT_FAILURE);
	if (stop_simulation(data) == -1)
		return (data_cleanup(data, EXIT_FAILURE));
	if (DEBUG_FORMATTING == 1 && data->eat_count >= 0)
		write_outcome(data);
	return (data_cleanup(data, EXIT_SUCCESS));
}
