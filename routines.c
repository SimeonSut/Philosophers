/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simeon <simeon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/14 13:22:00 by simeon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void even_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	t;
	long long		tm;

	while (1)
	{
		take_a_fork(node, lst, phindex);
		take_a_fork(node, lst->next, phindex);
		open_close_gates(node, lst, phindex, UNLOCK);
		pthread_mutex_unlock(&node->t_philo_mtx);
		state(node, "eating", states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, "sleeping", states[SLEEP], phindex);
		if (gettimeofday(&t, NULL) == -1)
			break ;//gettimeofday error
		tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart)/ 1000LL;
		pthread_mutex_lock(&node->terminal_mtx);
		printf("%lld %d is thinking\n", tm, phindex);
		pthread_mutex_unlock(&node->terminal_mtx);
	}
}

void	odd_philos(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	t;
	int				tm;

	while (1)
	{
		take_a_fork(node, lst, phindex);
		take_a_fork(node, lst->next, phindex);
		open_close_gates(node, lst, phindex, UNLOCK);
		pthread_mutex_unlock(&node->t_philo_mtx);
		state(node, "eating", states[EAT], phindex);
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		state(node, "sleeping", states[SLEEP], phindex);
		if (gettimeofday(&t, NULL) == -1)
			break ;//gettimeofday error
		tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
		pthread_mutex_lock(&node->terminal_mtx);
		printf("%d %d is thinking\n", tm, phindex);
		pthread_mutex_unlock(&node->terminal_mtx);
	}
}

