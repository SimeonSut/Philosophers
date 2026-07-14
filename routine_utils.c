/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simeon <simeon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/14 13:20:23 by simeon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		t;
	long long			ustart;
	long long			check;
	long long			tm;

	ustart = node->ustart;
	if (gettimeofday(&t, NULL) == -1)
		return ;//gettimeofday error
	pthread_mutex_lock(&node->terminal_mtx);
	tm = (((t.tv_sec * 1000000LL + t.tv_usec) - ustart) / 1000LL);
	printf("%lld %d is %s\n", tm, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	check = time_to_state * 1000LL;
	while (((t.tv_sec * 1000000LL + t.tv_usec) - tm) < check)
	{
		usleep(500);
		if (gettimeofday(&t, NULL) == -1)
			return ;//gettimeofday error
	}
	return ;
}

void	open_close_gates(t_philo *node, t_list *lst, int phindex, int action)
{
	if (action == LOCK)
	{
		if (phindex % 2 == 0)
			pthread_mutex_lock(&node->gates_mtx[((phindex - 2) / 2)]);
		else
			pthread_mutex_lock(&node->gates_mtx[((phindex - 1) / 2)]);
	}
	else if (action == UNLOCK && lst->gate_count == 0)
	{
		lst->gate_count++;
		if (phindex % 2 == 0)
			pthread_mutex_unlock(&node->gates_mtx[((phindex - 2) / 2)]);
		else
			pthread_mutex_unlock(&node->gates_mtx[((phindex - 1) / 2)]);
	}
}

void	take_a_fork(t_philo *node, t_list *lst, int phindex)
{
	struct timeval	t;
	long long		tm;

	pthread_mutex_lock(&lst->fork_mtx);
	if (gettimeofday(&t, NULL) == -1)
		return ;//gettimeofday error
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d has taken a fork\n", tm, phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
}
