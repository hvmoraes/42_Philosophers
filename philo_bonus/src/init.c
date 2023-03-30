/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:40:47 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 11:17:52 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* set_local_sem_name:
*	Creates a unique semaphore name to create a mutex that protects
*	a philosopher's own meal variables. The name must be unique, otherwise
*	all philosopher processes would share the same meal semaphore.
*	In order to create this semaphore name, simply concatenate the standard
*	semaphore meal name with the philosopher's ID.
*	Returns the unique semaphore name with the given ID. NULL if memory
*	allocation fails.
*/
static char	*set_local_sem_name(const char *str, unsigned int id)
{
	unsigned int	i;
	unsigned int	count;
	char			*sem_name;
	char			*temp;

	count = 0;
	i = id;
	while (i)
	{
		count++;
		i /= 10;
	}
	i = ft_strlen(str) + count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	temp = ft_itoa(id, count);
	sem_name = ft_strcat(sem_name, temp);
	free(temp);
	return (sem_name);
}

/* set_philo_sem_names:
*	Creates unique semaphore names for semaphores that will only be shared
*	between a philo process and its main_thread. No other
*	process will open the semaphores belonging to a different philo process.
*/
static bool	set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (false);
	return (true);
}

/* init_philosophers:
*	Allocates memory for each philosopher and initializes their values.
*	Also creates specific semaphore names to protect their own meal-related
*	variables. The semaphore names for sem_meal must be unique to each
*	philosopher because the semaphore shouldn't be accessed by a different
*	philosopher process.
*	Returns a pointer to the array of philosophers or NULL if
*	initialization failed.
*/
static t_philo	**init_philosophers(t_data *data)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (data->nbr_philos + 1));
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < data->nbr_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->data = data;
		philos[i]->id = i;
		if (!set_philo_sem_names(philos[i]))
			return (error_null(STR_ERR_MALLOC, NULL, data));
		philos[i]->times_ate = 0;
		philos[i]->nbr_forks_held = 0;
		philos[i]->ate_enough = false;
		i++;
	}
	return (philos);
}

/* init_global_semaphores:
*	Initializes semaphores for forks, writing, fullness and death detection.
*	These sempahores are first opened in the parent process, but each child
*	process will open the same named semaphores, which will allow interprocess
*	communication through these semaphores.
*
*	Returns true if the initalizations were successful, false if
*	initilization failed.
*/
static bool	init_global_semaphores(t_data *data)
{
	unlink_global_sems();
	data->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, data->nbr_philos);
	if (data->sem_forks == SEM_FAILED)
		return (sem_error_cleanup(data));
	data->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (data->sem_write == SEM_FAILED)
		return (sem_error_cleanup(data));
	data->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, data->nbr_philos);
	if (data->sem_philo_full == SEM_FAILED)
		return (sem_error_cleanup(data));
	data->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, data->nbr_philos);
	if (data->sem_philo_dead == SEM_FAILED)
		return (sem_error_cleanup(data));
	data->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (data->sem_stop == SEM_FAILED)
		return (sem_error_cleanup(data));
	return (true);
}

/* init_data:
*	Initializes the data structure containing
*	all of the program's parameters.
*	Returns a pointer to the allocated data structure, or NULL if
*	an error occured during initialization.
*/
t_data	*init_data(int argc, char **argv, int i)
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
	data->philo_full_count = 0;
	data->stop_sim = false;
	if (argc - 1 == 5)
		data->eat_count = ft_atoi(argv[i]);
	if (!init_global_semaphores(data))
		return (NULL);
	data->philos = init_philosophers(data);
	if (!data->philos)
		return (NULL);
	data->pids = malloc(sizeof * data->pids * data->nbr_philos);
	if (!data->pids)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	return (data);
}
