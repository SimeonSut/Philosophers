/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/16 19:55:47 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	state(t_philo *node, char *action, int time_to_state, int phindex)
{
	struct timeval		t;
	long long			tm;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&node->terminal_mtx);
	tm = (((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL);
	printf("%lld %d is %s\n", tm, phindex, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	while (((t.tv_sec * 1000000LL + t.tv_usec) - tm) < time_to_state * 1000LL)
	{
		usleep(500);
		gettimeofday(&t, NULL);
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

int	take_a_fork(t_philo *node, t_list *lst, int phindex)
{
	struct timeval	t;
	long long		tm;

	pthread_mutex_lock(&lst->fork_mtx);
	gettimeofday(&t, NULL);
	death_check(node, t);
	pthread_mutex_lock(&node->t_philo_mtx);
	if (node->death_check == DEAD)
	{
		pthread_mutex_unlock(&node->t_philo_mtx);
		pthread_mutex_unlock(&lst->fork_mtx);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d has taken a fork\n", tm, phindex);
	pthread_mutex_unlock(&node->terminal_mtx);
	gettimeofday(&t, NULL);
	death_check(node, t);
	if (node->death_check == DEAD)
		return (pthread_mutex_unlock(&lst->fork_mtx), DEAD);
	return (ALIVE);
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

int		death_check(t_philo *node, struct timeval t)
{
	long long		check_tm;
	long long		tm;
	long long		printed;
	long long		start_print;

	check_tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart);
	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart);
	pthread_mutex_lock(&node->t_philo_mtx);
	printed = tm - check_tm;
	start_print = node->tt_die *1000LL;
	//printf("current time is %lld and start time is %lld\n", printed, start_print);
	if ((tm - check_tm) >= node->tt_die * 1000LL)
	{
		node->death_check = DEAD;
		pthread_mutex_unlock(&node->t_philo_mtx);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	return (ALIVE);
}
	