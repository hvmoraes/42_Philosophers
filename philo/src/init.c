/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:11:54 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/29 11:48:25 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* intit_forks:
*	Allocates memory and initializes fork mutexes.
*	Returns a pointer to the fork mutex array, or NULL if an error occured. 
*/
static pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philos);
	if (!forks)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < data->nbr_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

/* assign_forks:
*	Assigns two fork ids to each philosopher. Even-numbered philosophers
*	get their fork order switched. This is because the order in which
*	philosophers take their forks matters.
*
*	For example with 3 philos:
*		Philo #1 (id: 0) will want fork 0 and fork 1
*		Philo #2 (id: 1) will want fork 1 and fork 2
*		Philo #3 (id: 2) will want fork 2 and fork 0
*	If philo #1 takes fork 0, philo #2 takes fork 1 and philo #3 takes fork 2,
*	there is a deadlock. Each will be waiting for their second fork which is
*	in use by another philo.
*
*	Making even id philos "left-handed" helps:
*		Philo #1 (id: 0) takes fork 1 and then fork 0
*		Philo #2 (id: 1) takes fork 1 and then fork 2
*		Philo #3 (id: 2) takes fork 0 and then fork 2
*	Now, philo #1 takes fork 1, philo #3 takes fork 0 and philo #2 waits patiently.
*	Fork 2 is free for philo #3 to take, so he eats. When he is done philo #1 can
*	take fork 0 and eat. When he is done, philo #2 can finally get fork 1 and eat.
*/
static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->data->nbr_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->data->nbr_philos;
		philo->fork[1] = philo->id;
	}
}

/* init_philosophers:
*	Allocates memory for each philosopher and initializes their values.
*	Returns a pointer to the array of philosophers or NULL if
*	initialization failed.
*/
static t_philo	**init_philosophers(t_data *data)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * data->nbr_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < data->nbr_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		philos[i]->data = data;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

/* init_global_mutexes:
*	Initializes mutex locks for forks, writing and the stop simulation
*	flag.
*	Returns true if the initalizations were successful, false if
*	initilization failed.
*/
static bool	init_global_mutexes(t_data *data)
{
	data->fork_locks = init_forks(data);
	if (!data->fork_locks)
		return (false);
	if (pthread_mutex_init(&data->sim_stop_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, data));
	if (pthread_mutex_init(&data->write_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, data));
	return (true);
}

/* init:
*	Initializes the "dining table", the data structure containing
*	all of the program's parameters.
*	Returns a pointer to the allocated data structure, or NULL if
*	an error occured during initialization.
*/
t_data	*init_general(int argc, char **argv, int i)
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	data->nbr_philos = ft_atoi(argv[i++]);
	data->time_to_die = ft_atoi(argv[i++]);
	data->time_to_eat = ft_atoi(argv[i++]);
	data->time_to_sleep = ft_atoi(argv[i++]);
	data->eat_count = -1;
	if (argc - 1 == 5)
		data->eat_count = ft_atoi(argv[i]);
	data->philos = init_philosophers(data);
	if (!data->philos)
		return (NULL);
	if (!init_global_mutexes(data))
		return (NULL);
	data->sim_stop = false;
	return (data);
}
