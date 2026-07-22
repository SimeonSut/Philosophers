/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:43:33 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	open_close_gates(t_philo *node, t_list *lst, int action)
{
	int	target_index;

	target_index = 0;
	if (action == LOCK)
	{
		if (lst->i % 2 == 0)
			target_index = ((lst->i - 2) / 2);
		else
			target_index = ((lst->i - 1) / 2);
		pthread_mutex_lock(&node->gates_mtx[target_index]);
	}
	else if (action == UNLOCK && lst->gate_count == 0)
	{
		lst->gate_count++;
		if (lst->i % 2 == 0)
			target_index = ((lst->i - 2) / 2);
		else
			target_index = ((lst->i - 1) / 2);
		pthread_mutex_unlock(&node->gates_mtx[target_index]);
	}
}

void	announce_death(t_philo *node, t_list *lst)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld Oh nooo, philosopher %d has died!\n", tm, lst->i);
	pthread_mutex_unlock(&node->terminal_mtx);
}

void	announce_fork_taken(t_philo *node, t_list *lst)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld philosopher %d has taken a fork!\n", tm, lst->i);
	pthread_mutex_unlock(&node->terminal_mtx);
}

void	announce_thinking(t_philo *node, t_list *lst)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d is thinking\n", tm, lst->i);
	pthread_mutex_unlock(&node->terminal_mtx);
}

int	is_eat_count_ok(t_philo *node, t_list *lst, int mode)
{
	t_list	*check_lst;

	if (mode == ACT)
	{
		pthread_mutex_lock(&node->t_philo_mtx);
		lst->eat_count++;
		pthread_mutex_unlock(&node->t_philo_mtx);
	}
	check_lst = lst->next;
	while (check_lst->i != lst->i)
	{
		pthread_mutex_lock(&node->t_philo_mtx);
		if (check_lst->eat_count < node->count)
		{
			pthread_mutex_unlock(&node->t_philo_mtx);
			return (YES);
		}
		pthread_mutex_unlock(&node->t_philo_mtx);
		check_lst = check_lst->next;
	}
	if (lst->eat_count < node->count)
		return (YES);
	return (NO);
}
