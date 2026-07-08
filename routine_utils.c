/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/08 21:09:16 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		tm;
	int					microstart;
	long long			check_tm;
	long long			timestamp;

	pthread_mutex_lock(&node->t_philo_mtx);
	microstart = node->microstart;
	pthread_mutex_unlock(&node->t_philo_mtx);
	if (gettimeofday(&tm, NULL) == -1)
		return ;//gettimeofday error
	timestamp = (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) / 1000LL);
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d iss %s\n", timestamp, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	check_tm = (long long)time_to_state * 1000LL;
	while (((tm.tv_sec * 1000000LL + tm.tv_usec) - microstart) < check_tm)
	{
		if (gettimeofday(&tm, NULL) == -1)
			return ;//gettimeofday error
	}
	return ;
}

void	open_close_gates(t_philo *node, t_list *lst, int phindex, int action)
{
	int			gate_count;

	if (action == UNLOCK || action == LOCK_TWO)
		pthread_mutex_lock(&node->t_philo_mtx);
	lst->gate_count++;
	gate_count = lst->gate_count;
	if (action == UNLOCK || action == LOCK_TWO)
		pthread_mutex_unlock(&node->t_philo_mtx);
	if (action != UNLOCK && gate_count == 1)
	{
		if (phindex % 2 == 0)
			pthread_mutex_lock(&node->gates_mtx[((phindex - 2) / 2)]);
		else
			pthread_mutex_lock(&node->gates_mtx[((phindex - 1) / 2)]);
	}
	else if (action == UNLOCK && gate_count == 2)
	{
		if (phindex % 2 == 0)
			pthread_mutex_unlock(&node->gates_mtx[((phindex - 2) / 2)]);
		else
			pthread_mutex_unlock(&node->gates_mtx[((phindex - 1) / 2)]);
	}
}
