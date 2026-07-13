/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/13 14:12:27 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		tm;
	long long			mstart;
	long long			check;
	long long			timestamp;

	mstart = node->microstart;
	if (gettimeofday(&tm, NULL) == -1)
		return ;//gettimeofday error
	pthread_mutex_lock(&node->terminal_mtx);
	timestamp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - mstart) / 1000LL);
	printf("%lld %d is %s\n", timestamp, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	timestamp = (tm.tv_sec * 1000000LL + tm.tv_usec);
	check = time_to_state * 1000LL;
	while (((tm.tv_sec * 1000000LL + tm.tv_usec) - timestamp) < check)
	{
		usleep(500);
		if (gettimeofday(&tm, NULL) == -1)
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
