/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/16 20:06:39 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void routine(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	while (death_check(node, t) == ALIVE)
	{
		if (take_a_fork(node, lst, phindex) == DEAD)
			break ;
		if (take_a_fork(node, lst->next, phindex) == DEAD)
			break ;
		open_close_gates(node, lst, phindex, UNLOCK);
		if (state(node, "eating", states[EAT], phindex) == DEAD)
			break ;
		pthread_mutex_unlock(&lst->fork_mtx);
		pthread_mutex_unlock(&lst->next->fork_mtx);
		if (state(node, "sleeping", states[SLEEP], phindex) == DEAD)
			break ;
		gettimeofday(&t, NULL);
		tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
		pthread_mutex_lock(&node->terminal_mtx);
		printf("%lld %d is thinking\n", tm, phindex);
		pthread_mutex_unlock(&node->terminal_mtx);
	}
}
