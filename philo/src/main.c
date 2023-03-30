/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:15:11 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/29 12:15:13 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* start_simulation:
*	Launches the simulation by creating a main thread as well as
*	one thread for each philosopher.
*	Returns true if the simulation was successfully started, false if there
*	was an error. 
*/
static bool	start_simulation(t_data *data)
{
	unsigned int	i;

	data->start_time = get_time() + (data->nbr_philos * 2 * 10);
	i = 0;
	while (i < data->nbr_philos)
	{
		if (pthread_create(&data->philos[i]->thread, NULL,
				&philo, data->philos[i]) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, data));
		i++;
	}
	if (data->nbr_philos > 1)
	{
		if (pthread_create(&data->main_thread, NULL,
				&main_thread, data) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, data));
	}
	return (true);
}

/* stop_simulation:
*	Waits for all threads to be joined then destroys mutexes and frees
*	allocated memory.
*/
static void	stop_simulation(t_data	*data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		pthread_join(data->philos[i]->thread, NULL);
		i++;
	}
	if (data->nbr_philos > 1)
		pthread_join(data->main_thread, NULL);
	if (DEBUG_FORMATTING == true && data->eat_count != -1)
		write_outcome(data);
	destroy_mutexes(data);
	free_data(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	data = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!valid_input(argc, argv))
		return (EXIT_FAILURE);
	data = init_general(argc, argv, 1);
	if (!data)
		return (EXIT_FAILURE);
	if (!start_simulation(data))
		return (EXIT_FAILURE);
	stop_simulation(data);
	return (EXIT_SUCCESS);
}
