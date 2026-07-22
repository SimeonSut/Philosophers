/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssutarmi <ssutarmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:34:46 by ssutarmi          #+#    #+#             */
/*   Updated: 2026/07/22 21:43:06 by ssutarmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	take_a_fork(t_philo *node, t_list *lst, int side);
static int	state(t_philo *node, t_list *lst, char *action, int time_to_state);
static int	try_fork(t_list *lst, int side);
static int	drop_fork(t_list *lst);

void	routine(t_philo *node, t_list *lst, int *states)
{
	store_time(lst);
	while (death_check(node, lst) == ALIVE)
	{
		if (take_a_fork(node, lst, LEFT) == DEAD)
			break ;
		if (!lst->next || take_a_fork(node, lst, RIGHT) == DEAD)
			break ;
		open_close_gates(node, lst, UNLOCK);
		store_time(lst);
		if (state(node, lst, "eating", states[EAT]) == DEAD)
			break ;
		if (node->count != -1 && is_eat_count_ok(node, lst, ACT) == NO)
			break ;
		if (death_check(node, lst) == DEAD)
			break ;
		drop_fork(lst);
		if (state(node, lst, "sleeping", states[SLEEP]) == DEAD)
			break ;
		if (death_check(node, lst) == DEAD)
			break ;
		announce_thinking(node, lst);
	}
	if (!lst->next)
		wait_death_time(node, lst);
}

static int	state(t_philo *node, t_list *lst, char *action, int time_to_state)
{
	struct timeval		t;
	struct timeval		death_count_t;
	long long			tm;

	gettimeofday(&t, NULL);
	tm = (((t.tv_sec * 1000000LL + t.tv_usec) - node->ustart) / 1000LL);
	pthread_mutex_lock(&node->terminal_mtx);
	printf("%lld %d is %s\n", tm, lst->i, action);
	pthread_mutex_unlock(&node->terminal_mtx);
	tm = (t.tv_sec * 1000000LL + t.tv_usec);
	death_count_t = t;
	while (((t.tv_sec * 1000000LL + t.tv_usec) - tm) < time_to_state * 1000LL)
	{
		usleep(500);
		if (death_check(node, lst) == DEAD)
		{
			return (DEAD);
		}
		gettimeofday(&t, NULL);
	}
	return (ALIVE);
}

static int	take_a_fork(t_philo *node, t_list *lst, int side)
{
	struct timeval	t;

	if (node->count != -1 && is_eat_count_ok(node, lst, CHECK) == NO)
		return (DEAD);
	gettimeofday(&t, NULL);
	while (death_check(node, lst) == ALIVE)
	{
		if (node->count != -1 && is_eat_count_ok(node, lst, CHECK) == NO)
			return (DEAD);
		if (try_fork(lst, side) == YES)
		{
			announce_fork_taken(node, lst);
			break ;
		}
		usleep(10);
	}
	if (death_check(node, lst) == DEAD)
		return (DEAD);
	return (ALIVE);
}

static int	try_fork(t_list *lst, int side)
{
	if (side == LEFT)
	{
		pthread_mutex_lock(&lst->fork_mtx);
		if (lst->fork_state == UNLOCKED)
		{
			lst->fork_state = LOCKED;
			pthread_mutex_unlock(&lst->fork_mtx);
			return (YES);
		}
		pthread_mutex_unlock(&lst->fork_mtx);
	}
	else if (side == RIGHT)
	{
		pthread_mutex_lock(&lst->next->fork_mtx);
		if (lst->next->fork_state == UNLOCKED)
		{
			lst->next->fork_state = LOCKED;
			pthread_mutex_unlock(&lst->next->fork_mtx);
			return (YES);
		}
		pthread_mutex_unlock(&lst->next->fork_mtx);
	}
	return (NO);
}

static int	drop_fork(t_list *lst)
{
	pthread_mutex_lock(&lst->fork_mtx);
	lst->fork_state = UNLOCKED;
	pthread_mutex_unlock(&lst->fork_mtx);
	pthread_mutex_lock(&lst->next->fork_mtx);
	lst->next->fork_state = UNLOCKED;
	pthread_mutex_unlock(&lst->next->fork_mtx);
	return (ALIVE);
}
