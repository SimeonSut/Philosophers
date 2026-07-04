/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 19:40:17 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/04 23:34:27 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	set_time(t_philo *node);

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while ((*nptr > 8 && *nptr < 14) || *nptr == 32)
		nptr++;
	if (*nptr == 43 || *nptr == 45)
	{
		if (*nptr == 45)
			sign = -1;
		nptr++;
	}
	while (*nptr > 47 && *nptr < 58)
	{
		result *= 10;
		result += (*nptr - 48);
		nptr++;
	}
	return ((result * sign));
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((*s1 - *s2));
}
/*
void	state_change_msg(int timestamp, int philo_n, char *state)
{
	return ;
}*/

int	t_philo_additional_setup(t_philo *node, char **argv)
{
	int	gates_nbr;

	gates_nbr = node->n_of_philos / 2;
	if (node->n_of_philos % 2 != 0)
		gates_nbr++;
	if (argv[5])
	{
		node->t_must_eat = ft_atoi(argv[5]);
		if (node->t_must_eat <= 0)
			return (write(2, "wrong must eat input\n", 22), ERROR);
	}
	if (set_time(node) == ERROR)
		return (ERROR);
	node->gates_mtx = malloc(gates_nbr * sizeof(pthread_mutex_t *));
	if (!node->gates_mtx)
		return (ERROR);
	node->i_mtx = malloc(sizeof(pthread_mutex_t));
	if (!node->i_mtx)
		return (free(node->gates_mtx), ERROR);
	return (SUCCESS);
}

static int	set_time(t_philo *node)
{
	t_time	*tm;

	tm = malloc(sizeof(t_time));
	if (!tm)
		return (ERROR);
	if (gettimeofday(tm, NULL) == -1)
		return (ERROR);
	node->microstart = (tm->tv_sec * 1000000 + tm->tv_usec);
	free(tm);
}
