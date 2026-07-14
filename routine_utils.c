/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/14 17:56:31 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		t;
	long long			tm;

	if (gettimeofday(&t, NULL) == -1)
		return (GETTIMEOFDAY_BUG);
	pthread_mutex_lock(&node->terminal_mtx);
	tm = (((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL);
	printf("%lld %d is %s\n", tm, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	while (((t.tv_sec * 1000000LL + t.tv_usec) - tm) < time_to_state * 1000LL)
	{
		usleep(500);
		if (gettimeofday(&t, NULL) == -1)
			return (GETTIMEOFDAY_BUG);
		if (((t.tv_sec * 1000000LL + t.tv_usec) - tm) >= (node->tt_die * 1000L))
		{
			pthread_mutex_lock(&node->t_philo_mtx);
			node->death_check = DEAD;
			pthread_mutex_unlock(&node->t_philo_mtx);
			return (DEAD);
		}
	}
	return (ALIVE);
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

void	open_close_gates(t_philo *node, t_list *lst, int phindex, int action)
{
	int	target_index;

	target_index = 0;
	if (action == LOCK)
	{
		if (phindex % 2 == 0)
			target_index = ((phindex - 2) / 2);
		else
			target_index = ((phindex - 1) / 2);
		//printf("%d locks gate %d\n", phindex, target_index);
		pthread_mutex_lock(&node->gates_mtx[target_index]);
	}
	else if (action == UNLOCK && lst->gate_count == 0)
	{
		lst->gate_count++;
		if (phindex % 2 == 0)
			target_index = ((phindex - 2) / 2);
		else
			target_index = ((phindex - 1) / 2);
		//printf("%d unlocks gate %d\n", phindex, target_index);
		pthread_mutex_unlock(&node->gates_mtx[target_index]);
	}
}
