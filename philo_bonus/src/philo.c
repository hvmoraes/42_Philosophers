/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:41:24 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/30 14:23:26 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

/* eat_sleep_routine:
*	When a philosopher is ready to eat, he will try to acquire fork semaphores.
*	Then the philosopher will eat for a certain amount of time. The time of
*	the last meal is recorded at the beginning of the meal, not at the end,
*	as per the subject's requirements.
*/
static void	eat_sleep_routine(t_philo *philo)
{
	grab_fork(philo);
	grab_fork(philo);
	write_status(philo, false, EATING);
	sem_wait(philo->sem_meal);
	philo->last_meal = get_time();
	sem_post(philo->sem_meal);
	philo_sleep(philo->data->time_to_eat);
	write_status(philo, false, SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->nbr_forks_held -= 2;
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	philo_sleep(philo->data->time_to_sleep);
}

/* think_routine:
*	Once a philosopher is done sleeping, he will think for a certain
*	amount of time before starting to eat again.
*	The time_to_think is calculated depending on how long it has been
*	since the philosopher's last meal, the time_to_eat and the time_to_die
*	to determine when the philosopher will be hungry again.
*	This helps stagger philosopher's eating routines to avoid forks being
*	needlessly monopolized by one philosopher to the detriment of others.
*/
static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->data->time_to_die
			- (get_time() - philo->last_meal)
			- philo->data->time_to_eat) / 2;
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(time_to_think);
}

/* lone_philo_routine:
*	This routine is invoked when there is only a single philosopher.
*	A single philosopher only has one fork, and so cannot eat. The
*	philosopher will pick up that fork, wait as long as time_to_die and die.
*	This is a separate routine to make sure that the process does not get
*	stuck waiting for a fork or a writing semaphore that will never unlock.
*/
static void	lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->data->nbr_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(CHILD_EXIT_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	sim_start_delay(philo->data->start_time);
	if (philo->data->eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(CHILD_EXIT_PHILO_FULL);
	}
	if (DEBUG_FORMATTING == true)
		print_status_debug(philo, PURPLE, "has taken a fork", GOT_FORK_1);
	else
		print_status(philo, "has taken a fork");
	philo_sleep(philo->data->time_to_die);
	if (DEBUG_FORMATTING == true)
		print_status_debug(philo, RED, "died", DIED);
	else
		print_status(philo, "died");
	free_data(philo->data);
	exit(CHILD_EXIT_PHILO_DEAD);
}

/* philosopher_routine:
*	Runs the philosopher process' routine as long as the philosopher
*	is alive. The philosopher will eat, sleep and think.
*/
static void	philosopher_routine(t_philo *philo)
{
	if (philo->id % 2)
		think_routine(philo, true);
	while (1)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
}

/* philosopher:
*	The philosopher thread routine. The philosopher must eat, sleep
*	and think. In order to avoid conflicts between philosopher processes,
*	philosophers with an even id start by thinking, which delays their
*	meal time by a small margin. This allows odd-id philosophers to
*	grab both of their forks first, avoiding deadlocks.
*/
void	philo(t_data *data)
{
	t_philo	*philo;

	philo = data->this_philo;
	if (philo->data->nbr_philos == 1)
		lone_philo_routine(philo);
	init_philo_ipc(data, philo);
	if (philo->data->eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		child_exit(data, CHILD_EXIT_PHILO_FULL);
	}
	if (philo->data->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		child_exit(data, CHILD_EXIT_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->data->start_time;
	sem_post(philo->sem_meal);
	sim_start_delay(philo->data->start_time);
	philosopher_routine(philo);
}
