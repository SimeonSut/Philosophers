/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:05 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/27 23:14:46 by ssutarmi         ###   ########.fr       */
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

typedef struct  s_philo
{
	int				n_of_philos;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				t_must_eat;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	suseconds_t		start_time;
}				t_philo;

//MAIN.C

//INPUT_CHECK.C
t_philo		*check_n_initialize(char **argv);

//UTILS.C
size_t		ft_strlen(const char *s);
int			ft_atoi(const char *nptr);
int			ft_strcmp(char *s1, char *s2);

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1
} t_exit;

#endif