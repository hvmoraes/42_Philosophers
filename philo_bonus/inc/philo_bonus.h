/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:01 by mcombeau          #+#    #+#             */
/*   Updated: 2023/03/30 11:02:14 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

/******************************************************************************
*                                     Macros                                  *
******************************************************************************/

# define MAX_PHILOS	250
# define STR_MAX_PHILOS "250"

# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING 0
# endif

# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_SEM	"%s error: Could not create semaphore.\n"
# define STR_ERR_FORK	"%s error: Could not fork child.\n"

# define SEM_NAME_FORKS "/philo_global_forks"
# define SEM_NAME_WRITE "/philo_global_write"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"
# define SEM_NAME_STOP	"/philo_global_stop"
# define SEM_NAME_MEAL	"/philo_local_meal_"

# define CHILD_EXIT_ERR_PTHREAD	40
# define CHILD_EXIT_ERR_SEM		41
# define CHILD_EXIT_PHILO_FULL	42
# define CHILD_EXIT_PHILO_DEAD	43

/******************************************************************************
*                                 Structures                                  *
******************************************************************************/

typedef struct s_philo	t_philo;

typedef struct s_data
{
	time_t			start_time;
	unsigned int	nbr_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				eat_count;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_philo_full;
	unsigned int	philo_full_count;
	sem_t			*sem_philo_dead;
	sem_t			*sem_stop;
	bool			stop_sim;
	t_philo			**philos;
	t_philo			*this_philo;
	pid_t			*pids;
	pthread_t		third_thread;
	pthread_t		second_thread;
}	t_data;

typedef struct s_philo
{
	pthread_t			main_thread;
	sem_t				*sem_forks;
	sem_t				*sem_write;
	sem_t				*sem_philo_full;
	sem_t				*sem_philo_dead;
	sem_t				*sem_meal;
	char				*sem_meal_name;
	unsigned int		nbr_forks_held;
	unsigned int		id;
	unsigned int		times_ate;
	bool				ate_enough;
	time_t				last_meal;
	t_data				*data;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/******************************************************************************
*                           Function Prototypes                               *
******************************************************************************/

//	main.c
bool			has_simulation_stopped(t_data *data);

//	parsing.c
int				ft_atoi(char *str);
bool			valid_input(int argc, char **argv);

//	init.c
t_data			*init_data(int argc, char **argv, int i);

// ipc.c
void			init_philo_ipc(t_data *data, t_philo *philo);

//	philo.c
void			philo(t_data *data);

// philo_utils.c
void			grab_fork(t_philo *philo);

//	time.c
time_t			get_time(void);
void			philo_sleep(time_t sleep_time);
void			sim_start_delay(time_t start_time);

//	output.c
void			print_status_debug(t_philo *philo, char *color, char *str,
					t_status status);
void			print_status(t_philo *philo, char *str);
void			write_status(t_philo *philo, bool main_thread, t_status status);
void			write_outcome(t_data *data);

//	main_thread.c
int				kill_all_philos(t_data *data, int exit_code);
void			*third_thread(void *data);
void			*second_thread(void *data);
void			*main_thread(void *data);

// utils.c
size_t			ft_strlen(const char *str);
char			*ft_strcat(char	*dst, const char *src);
char			*ft_itoa(unsigned int nbr, size_t len);
void			unlink_global_sems(void);
bool			start_main_thread(t_data *data);

// cleanup.c
void			*free_data(t_data *data);
int				sem_error_cleanup(t_data *data);
int				data_cleanup(t_data *data, int exit_code);

//	exit.c
int				msg(char *str, char *detail, int exit_no);
int				error_failure(char *str, char *details, t_data *data);
void			*error_null(char *str, char *details, t_data *data);
void			child_exit(t_data *data, int exit_code);

#endif