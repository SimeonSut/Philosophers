/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/19 19:48:48 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	wait_death_time(t_philo *node);

void routine(t_philo *node, t_list *lst, int phindex, int *states)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	while (death_check(node, t, phindex) == ALIVE)
	{
		if (take_a_fork(node, lst, phindex) == DEAD)
			break ;
		if (!lst->next || take_a_fork(node, lst->next, phindex) == DEAD)
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
	if (!lst->next)
		wait_death_time(node);
}

static void	wait_death_time(t_philo *node)
{
	struct timeval	t;
	long long		udeath;

	udeath = node->tt_die * 1000LL;
	gettimeofday(&t, NULL);
	while (((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) < udeath)
	{
		usleep(100);
		gettimeofday(&t, NULL);
	}
	announce_death(node, 1);
}
