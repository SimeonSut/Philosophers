/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/20 22:14:57 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		pthread_mutex_lock(&node->gates_mtx[target_index]);
	}
	else if (action == UNLOCK && lst->gate_count == 0)
	{
		lst->gate_count++;
		if (phindex % 2 == 0)
			target_index = ((phindex - 2) / 2);
		else
			target_index = ((phindex - 1) / 2);
		pthread_mutex_unlock(&node->gates_mtx[target_index]);
	}
}

int		death_check(t_philo *node, struct timeval t, int phindex)
{
	long long		check_tm;
	long long		tm;

	pthread_mutex_lock(&node->t_philo_mtx);
	if (node->death_check == DEAD)
	{
		pthread_mutex_unlock(&node->t_philo_mtx);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	check_tm = (t.tv_sec * 1000000LL + t.tv_usec);
	gettimeofday(&t, NULL);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	pthread_mutex_lock(&node->t_philo_mtx);
	if ((tm - check_tm) >= node->tt_die * 1000LL)
	{
		node->death_check = DEAD;
		pthread_mutex_unlock(&node->t_philo_mtx);
		announce_death(node, phindex);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	return (ALIVE);
}

void	announce_death(t_philo *node, int phindex)
{
	pthread_mutex_lock(&node->terminal_mtx);
	printf("Oh nooo, philosopher %d has died!\n", phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
}

void	announce_fork_taken(t_philo *node, int phindex)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld philosopher %d has taken a fork!\n", tm, phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
}

void	announce_thinking(t_philo *node, int phindex)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d is thinking\n", tm, phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
}
