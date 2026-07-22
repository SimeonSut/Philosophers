/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:59:15 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:45:10 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	store_time(t_list *lst)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	lst->last_eat_tm = t.tv_sec * 1000000LL + t.tv_usec;
	return ;
}

int	death_check(t_philo *node, t_list *lst)
{
	long long		tm;
	struct timeval	t;

	pthread_mutex_lock(&node->t_philo_mtx);
	if (node->death_check == DEAD)
	{
		pthread_mutex_unlock(&node->t_philo_mtx);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	gettimeofday(&t, NULL);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	pthread_mutex_lock(&node->t_philo_mtx);
	if ((tm - lst->last_eat_tm) >= node->tt_die * 1000LL)
	{
		node->death_check = DEAD;
		pthread_mutex_unlock(&node->t_philo_mtx);
		announce_death(node, lst);
		return (DEAD);
	}
	pthread_mutex_unlock(&node->t_philo_mtx);
	return (ALIVE);
}

void	wait_death_time(t_philo *node, t_list *lst)
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
	announce_death(node, lst);
}
