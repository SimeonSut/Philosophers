/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 19:40:17 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/06 20:39:04 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	t_philo_additional_setup(t_philo *node, char **argv)
{
	int	gates_nbr;
	struct timeval	tm;

	gates_nbr = node->n_of_philos / 2;
	if (gettimeofday(&tm, NULL) == -1)
		return (ERROR);
	if (node->n_of_philos % 2 != 0)
		gates_nbr++;
	if (argv[5])
	{
		node->t_must_eat = ft_atoi(argv[5]);
		if (node->t_must_eat <= 0)
			return (write(2, "wrong must eat input\n", 22), ERROR);
	}
	node->gates_mtx = malloc(gates_nbr * sizeof(pthread_mutex_t));
	if (!node->gates_mtx)
		return (ERROR);
	while (--gates_nbr >= 0)
		if (pthread_mutex_init(&node->gates_mtx[gates_nbr], NULL) == -1)
			return (ERROR);//mutex init error, free to add here as well
	node->microstart = (tm.tv_sec * 1000000LL + tm.tv_usec);
	return (SUCCESS);
}
