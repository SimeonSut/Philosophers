/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 18:40:05 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/06/25 21:01:29 by ssutarmi         ###   ########.fr       */
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

//MAIN.C

//INPUT_CHECK.C
int		is_valid_input(char **argv);

//UTILS.C
size_t		ft_strlen(const char *s);
long long	ft_atoll(const char *nptr);
int			ft_strcmp(char *s1, char *s2);

typedef struct  s_philo
{
	int			n_of_philos;
	int			n_of_forks;
	int			tt_die;
	int			tt_eat;
	int			tt_sleep;
	int			t_must_eat;
}				t_philo;

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1
} t_exit;

#endif