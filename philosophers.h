/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:05 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/30 19:00:35 by ssutarmi         ###   ########.fr       */
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
	int				mtx_state;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
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
	t_time			*start_time;
	t_list			*list;
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
t_philo			*check_n_initialize(char **argv);

//---INPUT_CHECK_UTILS.C
size_t			ft_strlen(const char *s);
int				ft_atoi(const char *nptr);
int				ft_strcmp(char *s1, char *s2);
int				set_time(t_philo *node);

//---ROUTINE_UTILS.C
char			*ft_freejoin(char *s1, char *s2);
char			*ft_itoa(int n);

//---ROUTINE.C---
void			thread_setup(t_philo *node);

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1,
	UNLOCKED = 0,
	LOCKED = 1,
	LEFT = 0,
	RIGHT = 1,
	NOT_TAKEN = 0,
	TAKEN = 1
} t_exit;

#endif