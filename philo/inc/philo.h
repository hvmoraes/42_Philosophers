/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:11:31 by hcorrea-          #+#    #+#             */
/*   Updated: 2023/03/29 11:47:46 by hcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

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
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

/******************************************************************************
*                                 Structures                                  *
******************************************************************************/

typedef struct s_philo	t_philo;

typedef struct s_data
{
	time_t			start_time;
	unsigned int	nbr_philos;
	pthread_t		main_thread;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}					t_data;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_data				*data;
}						t_philo;

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

//	parsing.c
int				ft_atoi(char *str);
bool			valid_input(int ac, char **av);

//	init.c
t_data			*init_general(int ac, char **av, int i);

//	philo.c
void			*philo(void *data);

//	time.c
time_t			get_time(void);
void			philo_sleep(t_data *data, time_t sleep_time);
void			sim_start_delay(time_t start_time);

//	output.c
void			write_status(t_philo *philo, bool reaper, t_status status);
void			write_outcome(t_data *data);

//	main_thread.c
bool			has_simulation_stopped(t_data *data);
void			*main_thread(void *data);

//	exit.c
void			*free_data(t_data *data);
void			destroy_mutexes(t_data *data);
int				msg(char *str, char *detail, int exit_no);
int				error_failure(char *str, char *details, t_data *table);
void			*error_null(char *str, char *details, t_data *data);

#endif