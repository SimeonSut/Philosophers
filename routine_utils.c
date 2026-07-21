/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:24:03 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/21 17:56:16 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	wait_death_time(t_philo *node)
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

void	announce_death(t_philo *node, int phindex)
{
	struct timeval	t;
	long long		tm;

	gettimeofday(&t, NULL);
	tm = ((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL;
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld Oh nooo, philosopher %d has died!\n", tm, phindex);
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
