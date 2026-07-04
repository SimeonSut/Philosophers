/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:05 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/05 00:08:22 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_D
# define PHILOSOPHERS_D

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct timeval t_time;

typedef struct	s_list
{
	int				i;
	pthread_t		*thread;
	pthread_mutex_t	*fork_mtx;
	struct	s_list	*prev;
	struct	s_list	*next;
}				t_list;

typedef struct  s_philo
{
	int				n_of_philos;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				t_must_eat;
	long long		microstart;
	t_list			*list;
	pthread_mutex_t	**gates_mtx;
	pthread_mutex_t	*i_mtx;
}				t_philo;

//---MAIN.C---

//---INPUT_CHECK.C---

/*IS_VALID_INPUT
 * 
 * 	Input :
 * 		The argv argument provided with main
 * 
 * 	Behaviour :
 * 		Checks the validity of input and initiates the data structure
 * 
 * 	Return :
 * 		The data strucure on success
 * 		NULL on failure
 * 
 * 	Description :
 * 		Central function of the parsing of philosophers
 * 		Checks, by calling its helpers, if all inputs are positive
 * 		non-zero integers, which represent valid input for
 * 		the project.
 */
t_philo	*check_n_initialize(char **argv);

//---INPUT_CHECK_UTILS.C---
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *nptr);
int		ft_strcmp(char *s1, char *s2);
int		set_time(t_philo *node);
int		t_philo_additional_setup(t_philo *node, char **argv);

//---ROUTINE_UTILS.C---
char	*ft_freejoin(char *s1, char *s2);
char	*ft_itoa(int n);

//---ROUTINE.C---
void			thread_setup(t_philo *node);

//---TIME_AND_STATES.C---
void	fill_states_times(t_philo *node, int *states);
void	state(t_philo *node, char *state, int time_to_state, int time_to_die);

//---CYCLES.C---
/*void uneven_uneven_cycle(t_list *lst);
void uneven_even_cycle(t_list *lst);
void even_uneven_cycle(t_list *lst);
void even_even_cycle(t_list *lst);*/

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1,
	UNLOCKED = 0,
	LOCKED = 1,
	LEFT = 0,
	RIGHT = 1,
	NOT_TAKEN = 0,
	TAKEN = 1,
	DIE = 0,
	EAT = 1,
	SLEEP = 0
} t_exit;

#endif